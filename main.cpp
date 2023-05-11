#include "estatistica/estatisticas.hpp"
#include "thread/thread.hpp"
#include "grafico/grafico.hpp"

#include <iostream>

extern Estatisticas estatisticas; // Obtém a definição em simulador<numero>_mm1.cpp

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

    plotaGrafico(estatisticas);
    
    return 0;
}