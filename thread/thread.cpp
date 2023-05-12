#include "thread/thread.hpp"
#include "simulador_mm1.hpp"

#ifdef TAMANHO_AMOSTRA

#if TAMANHO_AMOSTRA <= 0 || TAMANHO_AMOSTRA > 50000

#define TAMANHO_AMOSTRA 10000

#endif

#else

#define TAMANHO_AMOSTRA 10000

#endif

const int tamanhoAmostra = TAMANHO_AMOSTRA; // Rodaremos 10000 vezes o simulador

void executaSimulacoes(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico)
{
    for (int i = 0 ; i < tamanhoAmostra / NUM_THREADS ; i++)
        simulaFilaMM1(numIteracoes, taxaChegada, taxaSaida, eDeterministico);
}

void inicializaThreads(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico)
{
    std::vector<std::thread> threads;

    for (int i = 0 ; i < NUM_THREADS ; i++)
        threads.push_back(std::thread(executaSimulacoes, numIteracoes, taxaChegada, taxaSaida, eDeterministico));

    for (int i = 0 ; i < NUM_THREADS ; i++)
        threads[i].join();
}