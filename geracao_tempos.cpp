#include "geracao_tempos.hpp"

static std::random_device aleatorio;

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