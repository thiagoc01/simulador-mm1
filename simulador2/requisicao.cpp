#include "simulador2/requisicao.hpp"

Requisicao::Requisicao(double tempoRequisicao, const TIPO_CHEGADA tipo)
{
    this->tempoRequisicao = tempoRequisicao;
    this->tipo = tipo;
}

double Requisicao::retornaTempoRequisicao() const
{
    return this->tempoRequisicao;
}

TIPO_CHEGADA Requisicao::retornaTipoRequisicao() const
{
    return this->tipo;
}