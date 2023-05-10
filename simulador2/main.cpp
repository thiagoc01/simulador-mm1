#include "simulador2/simulador2_mm1.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "Uso: ./<nome do programa> <número de requisições> <lambda> <mi>\n";
        exit(1);
    }

    simulaFilaMM1Probabilistica(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]));

    return 0;
}