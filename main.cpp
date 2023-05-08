#include "simulador_mm1_prob.hpp"

int main(int argc, char** argv)
{
    simulaFilaProbabilisticaMM1(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]));

    return 0;
}