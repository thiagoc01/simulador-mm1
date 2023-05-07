#include "geracao_tempos.hpp"

//static std::random_device aleatorio("40");
static std::mt19937 aleatorio(42);

double retornaTempoPoisson(const double& lambda)
{
    std::exponential_distribution<double> geradorPoisson(lambda);

    return geradorPoisson(aleatorio);
}

double retornaTempoExponencial(const double& beta)
{
    std::exponential_distribution<double> geradorExponencial(beta);
    
    return geradorExponencial(aleatorio);
}