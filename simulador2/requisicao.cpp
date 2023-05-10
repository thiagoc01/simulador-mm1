#include "simulador2/requisicao.hpp"

Requisicao::Requisicao(double tempoRequisicao, const std::string& tipo)
{
    this->tempoRequisicao = tempoRequisicao;
    this->tipo = tipo;
}

double Requisicao::retornaTempoRequisicao() const
{
    return this->tempoRequisicao;
}

std::string Requisicao::retornaTipoRequisicao() const
{
    return this->tipo;
}