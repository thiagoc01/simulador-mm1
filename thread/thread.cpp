#include "thread/thread.hpp"
#include "simulador_mm1.hpp"

const int tamanhoAmostra = 10000; // Rodaremos 10000 vezes o simulador

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