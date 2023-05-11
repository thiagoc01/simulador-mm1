#include "simulador2/simulador2_mm1.hpp"
#include "estatistica/estatisticas.hpp"

#include <iostream>
#include <thread>
#include <vector>

#define NUM_THREADS 5

extern Estatisticas estatisticas; // Obtém a definição em simulador2_mm1.cpp

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
    {
        threads.push_back(std::thread(executaSimulacoes, numIteracoes, taxaChegada, taxaSaida, eDeterministico));
        threads[i].join();
    } 
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "Uso: ./<nome do programa> <número de requisições> <lambda> <mi> [D (deterministico)]\n";
        exit(1);
    }

    if (argc >= 5 && std::string(argv[4]) == "D")
        inicializaThreads(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), true);

    else
        inicializaThreads(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), false);
    
    estatisticas.calculaVarianciasDesviosPadroesAmostrais();
    estatisticas.calculaIntervalosConfianca(1.96);
    estatisticas.imprimeAnaliseAmostral();
    
    return 0;
}