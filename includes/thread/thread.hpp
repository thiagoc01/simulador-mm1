#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <vector>

#if !defined(NUM_THREADS) || defined(NUM_THREADS) && NUM_THREADS <= 0

#define NUM_THREADS 5

#endif

void executaSimulacoes(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico);

void inicializaThreads(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico);


#endif