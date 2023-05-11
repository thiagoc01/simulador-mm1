#include <iostream>
#include <vector>
#include <queue>

#include "simulador2/requisicao.hpp"
#include "simulador2/simulador2_mm1.hpp"
#include "geracao_tempos.hpp"

struct ComparadorTemposRequisicao
{
    bool operator()(Requisicao& r1, Requisicao& r2)
    {
      return r1.retornaTempoRequisicao() > r2.retornaTempoRequisicao();
    }
};

static double (*geraTempoServicoDeterministico)(double mediaServico) = [](double mediaServico){return mediaServico; };
static double (*geraTempoServicoProbabilistico)(double mediaServico) = [](double mediaServico){return retornaTempoExponencial(mediaServico); };

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

void calculaMetricas()
{
    double mediaProcessosSistema = retornaSomaPessoas(numeroProcessosSistemaPeriodo) / tempoSimulacao;
    double mediaProcessosFila = retornaSomaPessoas(numeroProcessosFilaPeriodo) / tempoSimulacao;
    double tempoMedioSistema;
    double tempoMedioEspera;

    if (temposSistema.empty())
        tempoMedioSistema = 0.0;

    else
        tempoMedioSistema = std::accumulate(temposSistema.begin(), temposSistema.end(), 0.0) / temposSistema.size();

    if (temposEspera.empty())
        tempoMedioEspera = 0.0;

    else
        tempoMedioEspera = std::accumulate(temposEspera.begin(), temposEspera.end(), 0.0) / temposEspera.size();
    
    std::cout << "\n\nNúmero médio de processos no sistema (E(N)): " << mediaProcessosSistema << std::endl;
    std::cout << "Número médio de processos na fila (E(N_q)): " << mediaProcessosSistema << std::endl;     
    std::cout << "Tempo médio no sistema (E(T)): " << tempoMedioSistema << std::endl; 
    std::cout << "Tempo médio na fila (E(W)): " << tempoMedioEspera << std::endl; 
}

void atualizaSistema(const double& tempoChegada, const double& tempoServico)
{
    Requisicao cabecaFila = filaRequisicoes.top();
    filaRequisicoes.pop();

    if (cabecaFila.retornaTipoRequisicao() == CHEGADA)
    {
        tempoSimulacao = cabecaFila.retornaTempoRequisicao();
        temposChegada.push_back(tempoSimulacao);

        numeroProcessosSistemaPeriodo.push_back({tempoSimulacao - ultimoEvento, numPessoasSistema});
        numeroProcessosFilaPeriodo.push_back({tempoSimulacao - ultimoEvento, numPessoasFila});

        std::cout << "Tempo: " << tempoSimulacao << " - CHEGADA " << numPessoasSistema << "->" << numPessoasSistema + 1 << std::endl;

        numPessoasSistema++;
        numPessoasFila++;

        ultimoEvento = tempoSimulacao;

        filaRequisicoes.push(Requisicao(tempoSimulacao + tempoChegada, CHEGADA));

        if (numPessoasSistema == 1)
            filaRequisicoes.push(Requisicao(tempoSimulacao + tempoServico, SAIDA));
    }

    else
    {
        numeroProcessosFilaPeriodo.push_back({tempoSimulacao - ultimoEvento, numPessoasFila});

        numeroProcessosFilaPeriodo.push_back({cabecaFila.retornaTempoRequisicao() - tempoSimulacao, numPessoasFila - 1});

        temposEspera.push_back(tempoSimulacao - temposChegada[temposSaida.size()]);

        tempoSimulacao = cabecaFila.retornaTempoRequisicao();

        temposSaida.push_back(tempoSimulacao);

        temposSistema.push_back(tempoSimulacao - temposChegada[temposSaida.size() - 1]);

        numeroProcessosSistemaPeriodo.push_back({tempoSimulacao - ultimoEvento, numPessoasSistema});

        numPessoasFila--;                

        std::cout << "Tempo: " << tempoSimulacao << " - SAÍDA " << numPessoasSistema << "->" << numPessoasSistema - 1 << std::endl;

        numPessoasSistema--;

        ultimoEvento = tempoSimulacao;

        if (numPessoasSistema > 0)
            filaRequisicoes.push(Requisicao(tempoSimulacao + tempoServico, SAIDA));
    }
}



void simulaFilaMM1(int numIteracoes, double mediaChegada, double mediaServico, bool eDeterministico)
{
    double (*geraTempoServico)(double mediaServico);

    if (eDeterministico)
        geraTempoServico = geraTempoServicoDeterministico;
    else
        geraTempoServico = geraTempoServicoProbabilistico;

    double tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
    double tempoServico = geraTempoServico(1.0 / mediaServico);

    Requisicao requisicao = Requisicao(tempoChegada, CHEGADA);

    filaRequisicoes.push(requisicao);

    for (int i = 0 ; i < numIteracoes && !filaRequisicoes.empty() ; i++)
    {
        tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
        tempoServico = geraTempoServico(1.0 / mediaServico);

        atualizaSistema(tempoChegada, tempoServico);
    }

    calculaMetricas();
}
            