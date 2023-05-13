#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <vector>

#ifdef TAMANHO_AMOSTRA

#if TAMANHO_AMOSTRA <= 0 || TAMANHO_AMOSTRA > 50000

#define TAMANHO_AMOSTRA 10000

#endif

#else

#define TAMANHO_AMOSTRA 10000

#endif

#if !defined(NUM_THREADS) || (defined(NUM_THREADS) && NUM_THREADS <= 0)

#define NUM_THREADS 5

#endif

#if NUM_THREADS > TAMANHO_AMOSTRA

#define NUM_THREADS 1

#endif

void executaSimulacoes(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico);

void inicializaThreads(int numIteracoes, double taxaChegada, double taxaSaida, bool eDeterministico);


#endif