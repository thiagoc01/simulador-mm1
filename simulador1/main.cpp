#include "simulador1/simulador1_mm1.hpp"

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "Uso: ./<nome do programa> <número de requisições> <lambda> <mi> [D (deterministico)]\n";
        exit(1);
    }

    if (argc >= 5 && argv[4] == "D")
        simulaFilaMM1(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), true);

    else
        simulaFilaMM1(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), false);

    return 0;
}