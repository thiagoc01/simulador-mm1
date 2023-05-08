#include <iostream>
#include <random>

#include "geracao_tempos.hpp"

static int n = 0;
static double tempoSimulacao = 0.0;

void simulaFilaProbabilisticaMM1(int numIteracoes, double mediaChegada, double mediaServico)
{

    for (int i = 0 ; i < numIteracoes ; i++)
    {
        double tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
        double tempoSaida = retornaTempoExponencial(1.0 / mediaServico);

        if (n == 0)
        {
            tempoSimulacao += tempoChegada;

            std::cout << "Tempo: " << tempoSimulacao << " - CHEGADA " << n << "->" << n + 1 << std::endl;

            n++;
        }

        else
        {
            if (tempoChegada < tempoSaida)
            {
                tempoSimulacao += tempoChegada;

                std::cout << "Tempo: " << tempoSimulacao << " - CHEGADA " << n << "->" << n + 1 << std::endl;

                n++;
            }

            else
            {
                tempoSimulacao += tempoSaida;

                std::cout << "Tempo: " << tempoSimulacao << " - SAÍDA " << n << "->" << n - 1 << std::endl;

                n--;
            }
        }
    }

        
}