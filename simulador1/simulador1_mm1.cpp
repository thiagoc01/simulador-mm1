#include <iostream>
#include <random>
#include <vector>
#include <mutex>

#include "geracao_tempos.hpp"
#include "simulador_mm1.hpp"
#include "estatistica/metricas.hpp"
#include "estatistica/estatisticas.hpp"
#include "thread/thread.hpp"

/* Ponteiros de função para a geração de tempos de serviço de forma determínistica ou probabilística */

static double (*geraTempoServicoDeterministico)(double taxaServico) = [](double taxaServico){return 1.0 / taxaServico; };
static double (*geraTempoServicoProbabilistico)(double taxaServico) = [](double taxaServico){return retornaTempoExponencial(taxaServico); };

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

    int& numProcessosFila;
    int& numProcessosSistema;

    std::vector<std::pair<double, int>>& numerosProcessosSistemaPeriodo; // Quantidade de processos no sistema em um período de tempo
    std::vector<std::pair<double, int>>& numerosProcessosFilaPeriodo; // Quantidade de processos na fila em um período de tempo
    std::vector<double>& temposChegadaAcumulativo; // Todos os tempos de chegada das requisições
    std::vector<double>& temposSaida; // Análogo ao anterior para a saída
    std::vector<double>& temposSistema; // Tempo de serviço mais tempo na fila
    std::vector<double>& temposEspera; // Análogo ao anterior, porém somente na fila

} ParametrosSimulador;

Estatisticas estatisticas; // Objeto para guardar as métricas para criarmos uma amostra

void calculaMetricas(const ParametrosSimulador& parametros, const double& taxaChegada, const double& taxaServico)
{
    double tempoSistemaRespostaMedia; // Armazena E(T)
    double tempoSistemaEsperaMedia; // Armazena E(W)
    double numProcessosSistemaTotal = 0.0; // Somatório do número de processos no sistema
    double numProcessosFilaTotal = 0.0; // Somatório do número de processos na fila
    /*
        Realiza o cálculo análogo a esperança. Multiplica cada número de processos 
        pela taxa relativa de tempo que esse número ocorreu no sistema.
        A divisão pelo tempo total gera o que seria a probabilidade da definição da esperança.
    */

    for (const auto& numeroProcessosPeriodo : parametros.numerosProcessosSistemaPeriodo)
    {
        auto periodo = numeroProcessosPeriodo.first;
        auto numeroProcessos = numeroProcessosPeriodo.second;
        numProcessosSistemaTotal += periodo * numeroProcessos;
    }

    /* Análogo ao anterior, mas para processos na fila */

    for (const auto& numeroProcessosPeriodo : parametros.numerosProcessosFilaPeriodo)
    {
        auto periodo = numeroProcessosPeriodo.first;
        auto numeroProcessos = numeroProcessosPeriodo.second;
        numProcessosFilaTotal += periodo * numeroProcessos;
    }

    if (parametros.temposSistema.empty()) // Se nenhuma requisição saiu do sistema, o tempo médio é 0.
        tempoSistemaRespostaMedia = 0.0;
    else
        tempoSistemaRespostaMedia = std::accumulate(parametros.temposSistema.begin(), parametros.temposSistema.end(), 0.0) / parametros.temposSistema.size();

    if (parametros.temposEspera.empty()) // Análogo ao anterior.
        tempoSistemaEsperaMedia = 0.0;
    else
        tempoSistemaEsperaMedia = std::accumulate(parametros.temposEspera.begin(), parametros.temposEspera.end(), 0.0) / parametros.temposEspera.size();

    mutexEstatisticas.lock();

    estatisticas.adicionaAmostra(Metricas(numProcessosSistemaTotal / parametros.tempoSimulacao, numProcessosFilaTotal / parametros.tempoSimulacao,
                        tempoSistemaRespostaMedia, tempoSistemaEsperaMedia));
    
    mutexEstatisticas.unlock();

    #if NUM_THREADS == 1 && TAMANHO_AMOSTRA == 1
    
    std::cout << "\n\nNúmero médio de processos no sistema (E(N)): " << numProcessosSistemaTotal / parametros.tempoSimulacao << std::endl;
    std::cout << "Número médio de processos na fila (E(N_q)): " << numProcessosFilaTotal / parametros.tempoSimulacao << std::endl;
    std::cout << "Tempo médio no sistema (E(T)): " <<  tempoSistemaRespostaMedia << std::endl;
    std::cout << "Tempo médio na fila (E(W)): " <<  tempoSistemaEsperaMedia << std::endl;
    std::cout << "Comparativo E(T) simulado x E(T) analítico: " << tempoSistemaRespostaMedia << " " << (1 / taxaServico) / (1 - (taxaChegada / taxaServico)) << std::endl;
    std::cout << std::endl;

    #endif
}

/* Função que recebe os tempos de chegada e de serviço e atualiza esses tempos */
void atualizaSistema(const double& tempoChegada, const double& tempoSaida, ParametrosSimulador& parametros)
{
    /* Quando o sistema está vazio ou o tempo que o processo leva para chegar é menor do que o de serviço, o processo entra na fila de espera e é atendido */
    if (parametros.numProcessosSistema == 0 || tempoChegada < tempoSaida)
    {
        /* O tempo de chegada do processo é somado ao tempo total passado */
        parametros.tempoSimulacao += tempoChegada;
        
        /* Guarda o tempo da chegada */
        parametros.temposChegadaAcumulativo.push_back(parametros.tempoSimulacao);

        /* Guarda os números de processos respectivos e o período que ocorreram */
        parametros.numerosProcessosSistemaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numProcessosSistema});
        parametros.numerosProcessosFilaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numProcessosSistema});
        
        /* Instante que o evento chega */
        parametros.ultimoEvento = parametros.tempoSimulacao;
        
        /* O processo entra na fila */
        parametros.numProcessosFila++;
        
        #if NUM_THREADS == 1 && TAMANHO_AMOSTRA == 1

        /* Imprimindo o tempo de simulação, chegada e o número de processos atuais no sistema */
        std::cout << "Tempo: " << parametros.tempoSimulacao << " - CHEGADA " << parametros.numProcessosSistema << "->" << parametros.numProcessosSistema + 1 << std::endl;
        
        #endif

        parametros.numProcessosSistema++;
    }
    
    else
    {
        /* A requisição antes de ser atendida estava na fila. Guarda o período de tempo e a quantidade antes do atendimento */
        parametros.numerosProcessosFilaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numProcessosFila});
        parametros.numProcessosFila--;

        /* Nos dois casos a seguir, acessamos o vetor de tempos de chegada utilizando o vetor de tempos de saída.
           A i-ésima requisição a sair foi a i-ésima a chegar por ser um sistema com um servidor.
           Logo, esse acesso obtém o tempo respectivo. */

        /* Guarda o tempo de espera antes do atendimento */
        parametros.temposEspera.push_back(parametros.tempoSimulacao - parametros.temposChegadaAcumulativo[parametros.temposSaida.size()]);
        
         /* O tempo de saída do processo é somado ao tempo total passado */
        parametros.tempoSimulacao += tempoSaida;

        /* Guarda o tempo de saída e o tempo de resposta no sistema. */
        parametros.temposSaida.push_back(tempoSaida);
        parametros.temposSistema.push_back(parametros.tempoSimulacao - parametros.temposChegadaAcumulativo[parametros.temposSaida.size() - 1]);

         /* Durante o serviço ocorreu a quantidade anterior menos um */
        parametros.numerosProcessosFilaPeriodo.push_back({tempoSaida, parametros.numProcessosFila});
        parametros.numerosProcessosSistemaPeriodo.push_back({parametros.tempoSimulacao - parametros.ultimoEvento, parametros.numProcessosSistema});
        
        parametros.ultimoEvento = parametros.tempoSimulacao;
        
        #if NUM_THREADS == 1 && TAMANHO_AMOSTRA == 1

        /* Imprimindo o tempo de simulação, saída e o número de processos atuais no sistema */
        std::cout << "Tempo: " << parametros.tempoSimulacao << " - SAÍDA " << parametros.numProcessosSistema << "->" << parametros.numProcessosSistema - 1 << std::endl;

        #endif

        parametros.numProcessosSistema--;
    }
}

/* Função que recebe o λ, o μ e o número de iterações do simulador e roda o simulador do tipo M/M/1 */
void simulaFilaMM1(int numIteracoes, double taxaChegada, double taxaServico, bool eDeterminismo)
{
    int numProcessosSistema = 0;
    int numProcessosFila = 0;

    std::vector<double> temposChegadaAcumulativo; // Guarda os tempos de chegada de cada requisição
    std::vector<double> temposSaida; // Guarda os tempos de saída de cada requisição
    std::vector<double> temposEspera; // Guarda os tempos de espera de cada requisição
    std::vector<double> temposSistema; // Guarda os tempos no sistema de cada requisição
    std::vector<std::pair<double, int>> numerosProcessosFilaPeriodo; // Guarda o número de processos no sistema em um intervalo de tempo
    std::vector<std::pair<double, int>> numerosProcessosSistemaPeriodo; // Guarda o número de processos na fila em um intervalo de tempo

    double tempoSimulacao = 0.0; // Tempo do simulador (snapshots de eventos)
    double ultimoEvento = 0.0; // Ocorrência da última chegada ou saída

    ParametrosSimulador parametros =
    {
        .tempoSimulacao = tempoSimulacao,
        .ultimoEvento = ultimoEvento,
        .numProcessosFila = numProcessosFila,
        .numProcessosSistema = numProcessosSistema,
        .numerosProcessosSistemaPeriodo = numerosProcessosSistemaPeriodo,
        .numerosProcessosFilaPeriodo = numerosProcessosFilaPeriodo,
        .temposChegadaAcumulativo = temposChegadaAcumulativo,
        .temposSaida = temposSaida,
        .temposSistema = temposSistema,
        .temposEspera = temposEspera
    };

    double (*geraTempoServico)(double taxaServico);

    if (eDeterminismo)
        geraTempoServico = geraTempoServicoDeterministico;
    
    else
        geraTempoServico = geraTempoServicoProbabilistico;
    
    /* Loop que roda o simulador até o número de iterações desejado */
    for (int i = 0 ; i < numIteracoes ; i++)
    {
        /* Gera um tempo de chegada com distribuição de Poisson e um tempo de serviço pela distribuição Exponencial */
        double tempoChegada = retornaTempoPoisson(taxaChegada);
        double tempoSaida = geraTempoServico(taxaServico);
        
        /* Faz a atualização dos tempos para a próxima iteração */
        atualizaSistema(tempoChegada, tempoSaida, parametros);     
    }
    
    /* Função que calcula o número de processos na fila, no sistema e a o tempo médio na fila e no sistema */
    calculaMetricas(parametros, taxaChegada, taxaServico);  
}
