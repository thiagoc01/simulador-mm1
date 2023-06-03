#include "estatistica/estatisticas.hpp"
#include "thread/thread.hpp"
#include "grafico/grafico.hpp"

#include <iostream>

extern Estatisticas estatisticas; // Obtém a definição em simulador<numero>_mm1.cpp
int numClientes;

#ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

const std::string usoAjuda = "Uso: ./<nome do programa> <número de requisições> <lambda> <mi> <número de clientes> [D (deterministico)]\n";
int numArgsMinimo = 5;

#else

const std::string usoAjuda = "Uso: ./<nome do programa> <número de requisições> <lambda> <mi> [D (deterministico)]\n";
int numArgsMinimo = 4;

#endif

int main(int argc, char** argv)
{
    if (argc < numArgsMinimo)
    {
        std::cout << usoAjuda;
        exit(1);
    }

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    numClientes = std::atoi(argv[4]);

    if (numClientes < 2)
    {
        std::cout << "Número de clientes deve ser maior que 1." << std::endl;
        exit(2);
    }

    #else

    numClientes = 0;

    #endif

    if (argc >= numArgsMinimo + 1 && std::string(argv[numArgsMinimo]) == "D")
        inicializaThreads(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), true);

    else
        inicializaThreads(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), false);

    #if TAMANHO_AMOSTRA != 1

    estatisticas.calculaVarianciasDesviosPadroesAmostrais();
    estatisticas.calculaIntervalosConfianca(1.96);
    estatisticas.imprimeAnaliseAmostral(std::atof(argv[2]), std::atof(argv[3]));

    plotaGrafico(estatisticas);

    #endif
    
    return 0;
}