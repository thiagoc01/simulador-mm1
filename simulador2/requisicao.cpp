#include "simulador2/requisicao.hpp"

Requisicao::Requisicao(double tempoRequisicao, const TIPO_REQUISICAO tipo)
{
    this->tempoRequisicao = tempoRequisicao;
    this->tipo = tipo;
}

double Requisicao::retornaTempoRequisicao() const
{
    return this->tempoRequisicao;
}

TIPO_REQUISICAO Requisicao::retornaTipoRequisicao() const
{
    return this->tipo;
}