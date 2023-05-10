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

static bool(*comparaTemposRequisicao)(Requisicao r1, Requisicao r2)=[](Requisicao r1, Requisicao r2){ return r1.retornaTempoRequisicao() < r2.retornaTempoRequisicao();};

double tempoSimulacao = 0.0;
double ultimoEvento = 0.0;

int numPessoasFila = 0;
int numPessoasSistema = 0;

std::vector<std::pair<double, int>> numeroProcessosSistemaPeriodo;
std::vector<std::pair<double, int>> numeroProcessosFilaPeriodo;

std::priority_queue<Requisicao, std::vector<Requisicao>, ComparadorTemposRequisicao> filaRequisicoes;

void atualizaSistema(const double& tempoChegada, const double& tempoServico)
{
    Requisicao cabecaFila = filaRequisicoes.top();
    filaRequisicoes.pop();

    if (cabecaFila.retornaTipoRequisicao() == CHEGADA)
    {
        tempoSimulacao = cabecaFila.retornaTempoRequisicao();

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
        tempoSimulacao = cabecaFila.retornaTempoRequisicao();

        numeroProcessosSistemaPeriodo.push_back({tempoSimulacao - ultimoEvento, numPessoasSistema});
        numeroProcessosFilaPeriodo.push_back({tempoSimulacao - ultimoEvento, numPessoasFila});

        std::cout << "Tempo: " << tempoSimulacao << " - SAÍDA " << numPessoasSistema << "->" << numPessoasSistema - 1 << std::endl;

        numPessoasSistema--;

        ultimoEvento = tempoSimulacao;

        if (numPessoasSistema > 0)
            filaRequisicoes.push(Requisicao(tempoSimulacao + tempoServico, SAIDA));
    }
}

void simulaFilaMM1Probabilistica(int numIteracoes, double mediaChegada, double mediaServico)
{
    double tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
    double tempoServico = retornaTempoExponencial(1.0 / mediaServico);

    Requisicao requisicao = Requisicao(tempoChegada, CHEGADA);

    filaRequisicoes.push(requisicao);

    for (int i = 0 ; i < numIteracoes && !filaRequisicoes.empty() ; i++)
    {
        

        tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
        tempoServico = retornaTempoExponencial(1.0 / mediaServico);

        atualizaSistema(tempoChegada, tempoServico);
    }
            


}
            