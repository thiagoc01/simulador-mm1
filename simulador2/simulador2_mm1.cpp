#include <iostream>
#include <vector>
#include <queue>
#include <mutex>

#include "simulador2/requisicao.hpp"
#include "simulador2/simulador2_mm1.hpp"
#include "geracao_tempos.hpp"
#include "estatistica/metricas.hpp"
#include "estatistica/estatisticas.hpp"

struct ComparadorTemposRequisicao // Utilizado para ser a função de comparação na fila, já que queremos o menor tempo sempre.
{
    bool operator()(Requisicao& r1, Requisicao& r2)
    {
        return r1.retornaTempoRequisicao() > r2.retornaTempoRequisicao();
    }
};

/* Ponteiros de função para o caso determínistico e probabilístico */

static double (*geraTempoServicoDeterministico)(double mediaServico) = [](double mediaServico){return mediaServico; };
static double (*geraTempoServicoProbabilistico)(double mediaServico) = [](double mediaServico){return retornaTempoExponencial(mediaServico); };

static std::mutex mutexEstatisticas; // Mutex para acesso concorrente das threads ao objeto estatisticas

/* 
    Todas as variáveis que são utilizadas no simulador. Por serem muitas a serem passadas como argumento,
    a criação de uma estrutura para guardar as referências é mais viável.
    Caso contrário, seria necessário usar diversos locks, o que também é inviável.
*/

typedef struct
{
    double& tempoSimulacao;
    double& ultimoEvento;

    int& numPessoasFila;
    int& numPessoasSistema;

    std::vector<std::pair<double, int>>& numeroProcessosSistemaPeriodo; // Quantidade de processos no sistema em um período de tempo
    std::vector<std::pair<double, int>>& numeroProcessosFilaPeriodo; // Quantidade de processos na fila em um período de tempo
    std::vector<double>& temposChegada; // Todos os tempos de chegada das requisições
    std::vector<double>& temposSaida; // Análogo ao anterior para a saída
    std::vector<double>& temposSistema; // Tempo de serviço mais tempo na fila
    std::vector<double>& temposEspera; // Análogo ao anterior, porém somente na fila
    std::priority_queue<Requisicao, std::vector<Requisicao>, ComparadorTemposRequisicao>& filaRequisicoes; // Fila com as requisições

} ParametrosSimulador;

Estatisticas estatisticas; // Objeto para guardar as métricas para criarmos uma amostra

/* Realiza a soma das multiplicações dos períodos em que ocorreu uma certa quantidade. Análogo ao simulador 1. */

double retornaSomaPessoas(const std::vector<std::pair<double, int>>& numeroProcessosPeriodo)
{
    double soma = 0.0;

    for (const auto& numeroProcessos: numeroProcessosPeriodo)
    {
        double periodo = numeroProcessos.first;
        int qtdProcessos = numeroProcessos.second;

        soma += (periodo * qtdProcessos);
    }

    return soma;
}

/* Calcula todas as métricas desejadas: E(N), E(N_q), E(W) e E(T) */

void calculaMetricas(const std::vector<std::pair<double, int>>& numeroProcessosSistemaPeriodo,
                    const std::vector<std::pair<double, int>>& numeroProcessosFilaPeriodo, const ParametrosSimulador& parametros)
{
    double mediaProcessosSistema = retornaSomaPessoas(numeroProcessosSistemaPeriodo) / parametros.tempoSimulacao;
    double mediaProcessosFila = retornaSomaPessoas(numeroProcessosFilaPeriodo) / parametros.tempoSimulacao;
    double tempoMedioSistema;
    double tempoMedioEspera;

    if (parametros.temposSistema.empty()) // Se o vetor está vazio, considera média 0
        tempoMedioSistema = 0.0;

    else
        tempoMedioSistema = std::accumulate(parametros.temposSistema.begin(), parametros.temposSistema.end(), 0.0) / parametros.temposSistema.size();

    if (parametros.temposEspera.empty())
        tempoMedioEspera = 0.0;

    else
        tempoMedioEspera = std::accumulate(parametros.temposEspera.begin(), parametros.temposEspera.end(), 0.0) / parametros.temposEspera.size();
    
    // std::cout << "\n\nNúmero médio de processos no sistema (E(N)): " << mediaProcessosSistema << std::endl;
    // std::cout << "Número médio de processos na fila (E(N_q)): " << mediaProcessosSistema << std::endl;     
    // std::cout << "Tempo médio no sistema (E(T)): " << tempoMedioSistema << std::endl; 
    // std::cout << "Tempo médio na fila (E(W)): " << tempoMedioEspera << std::endl;

    mutexEstatisticas.lock();

    estatisticas.adicionaAmostra(Metricas(mediaProcessosSistema, mediaProcessosFila, tempoMedioSistema, tempoMedioEspera));

    mutexEstatisticas.unlock();
}

void trataEventoChegada(const Requisicao& cabelaFila, const double& tempoChegada, const double& tempoSaida, ParametrosSimulador& parametros)
{
    parametros.tempoSimulacao = cabecaFila.retornaTempoRequisicao(); // Atualiza o tempo para esse evento

    parametros.temposChegada.push_back(parametros.tempoSimulacao); // Guarda o tempo de chegada

    /* Guarda o período em que tivemos uma certa quantidade de requisições no sistema e na fila */

    parametros.numeroProcessosSistemaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numPessoasSistema});
    parametros.numeroProcessosFilaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numPessoasFila});

    // std::cout << "Tempo: " << tempoSimulacao << " - CHEGADA " << numPessoasSistema << "->" << numPessoasSistema + 1 << std::endl;

    /* Mais uma pessoa na fila e no sistema */

    parametros.numPessoasSistema++;
    parametros.numPessoasFila++;

    parametros.ultimoEvento = parametros.tempoSimulacao; // Guarda que esse foi o último evento

    parametros.filaRequisicoes.push(Requisicao(parametros.tempoSimulacao + tempoChegada, CHEGADA)); // Agenda uma nova chegada

    if (parametros.numPessoasSistema == 1) /// Se tivermos somente essa requisição, precisaremos tratá-la
        parametros.filaRequisicoes.push(Requisicao(parametros.tempoSimulacao + tempoServico, SAIDA)); // Agenda o tratamento dessa saída
}

void trataEventoSaida(const Requisicao& cabelaFila, const double& tempoChegada, const double& tempoSaida, ParametrosSimulador& parametros)
{
    /* Análogo ao do evento da chegada */

    parametros.numeroProcessosFilaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numPessoasFila});

    /* Quando a requisição entrar em serviço, o período em que ela ficou no sistema indica que houve a quantidade de pessoas na fila menos essa requisição */

    parametros.numeroProcessosFilaPeriodo.push_back({cabecaFila.retornaTempoRequisicao() - parametros.tempoSimulacao, parametros.numPessoasFila - 1});

    /* Guarda o tempo que a requisição aguardou. Análogo ao simulador 1. */

    parametros.temposEspera.push_back(parametros.tempoSimulacao - parametros.temposChegada[parametros.temposSaida.size()]);

    parametros.numPessoasFila--;  

    parametros.tempoSimulacao = cabecaFila.retornaTempoRequisicao(); // Atualiza o tempo para esse evento

    parametros.temposSaida.push_back(parametros.tempoSimulacao); // Guarda o tempo de saída

    /* Guarda o tempo que a requisição ficou no sistema. Análogo ao simulador 1. */

    parametros.temposSistema.push_back(parametros.tempoSimulacao - parametros.temposChegada[parametros.temposSaida.size() - 1]);

    /* Guarda o número de processos no sistema desde o último evento. */

    parametros.numeroProcessosSistemaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numPessoasSistema});              

    //std::cout << "Tempo: " << tempoSimulacao << " - SAÍDA " << numPessoasSistema << "->" << numPessoasSistema - 1 << std::endl;

    parametros.numPessoasSistema--; // Requisição tratada

    parametros.ultimoEvento = parametros.tempoSimulacao; // Informa que esse foi o último evento.

    if (parametros.numPessoasSistema > 0) // Se houver mais requisições, agenda os tratamentos delas.
        parametros.filaRequisicoes.push(Requisicao(parametros.tempoSimulacao + tempoServico, SAIDA));
}

void atualizaSistema(const double& tempoChegada, const double& tempoServico, ParametrosSimulador& parametros)
{
    Requisicao cabecaFila = parametros.filaRequisicoes.top(); // Obtém a requisição no começo da fila
    parametros.filaRequisicoes.pop();

    if (cabecaFila.retornaTipoRequisicao() == CHEGADA)
        trataEventoChegada(cabecaFila, tempoChegada, tempoServico, parametros);

    else
        trataEventoSaida(cabecaFila, tempoChegada, tempoServico, parametros);
}

void simulaFilaMM1(int numIteracoes, double mediaChegada, double mediaServico, bool eDeterministico)
{
    double tempoSimulacao = 0.0;
    double ultimoEvento = 0.0;

    int numPessoasFila = 0;
    int numPessoasSistema = 0;

    std::vector<std::pair<double, int>> numeroProcessosSistemaPeriodo;
    std::vector<std::pair<double, int>> numeroProcessosFilaPeriodo;
    std::vector<double> temposChegada;
    std::vector<double> temposSaida;
    std::vector<double> temposSistema;
    std::vector<double> temposEspera;

    std::priority_queue<Requisicao, std::vector<Requisicao>, ComparadorTemposRequisicao> filaRequisicoes;

    /* Guarda as referências para as variáveis anteriores para modificação nas funções. */

    ParametrosSimulador parametros =
    {
        .tempoSimulacao = tempoSimulacao,
        .ultimoEvento = ultimoEvento,
        .numPessoasFila = numPessoasFila,
        .numPessoasSistema = numPessoasSistema,
        .numeroProcessosSistemaPeriodo = numeroProcessosSistemaPeriodo,
        .numeroProcessosFilaPeriodo = numeroProcessosFilaPeriodo,
        .temposChegada = temposChegada,
        .temposSaida = temposSaida,
        .temposSistema = temposSistema,
        .temposEspera = temposEspera,
        .filaRequisicoes = filaRequisicoes
    };

    double (*geraTempoServico)(double mediaServico); // Ponteiro de função para tornar o simulador genérico a determinismo

    if (eDeterministico)
        geraTempoServico = geraTempoServicoDeterministico;
    else
        geraTempoServico = geraTempoServicoProbabilistico;

    double tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
    double tempoServico = geraTempoServico(1.0 / mediaServico);

    Requisicao requisicao = Requisicao(tempoChegada, CHEGADA); // Gera a primeira requisição

    filaRequisicoes.push(requisicao);

    for (int i = 0 ; i < numIteracoes && !parametros.filaRequisicoes.empty() ; i++)
    {
        tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
        tempoServico = geraTempoServico(1.0 / mediaServico);

        atualizaSistema(tempoChegada, tempoServico, parametros);
    }

    /* Calcula as métricas dessa amostra */

    calculaMetricas(parametros.numeroProcessosSistemaPeriodo, parametros.numeroProcessosFilaPeriodo, parametros);
}
            