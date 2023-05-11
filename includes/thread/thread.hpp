#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <vector>

#ifndef NUM_THREADS

#define NUM_THREADS 5

#endif

void executaSimulacoes(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico);

void inicializaThreads(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico);


#endif