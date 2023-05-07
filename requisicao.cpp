#include "requisicao.hpp"

Requisicao::Requisicao(double tempoChegada, double tempoServico, double tempoChegadaAcumulativo,
            double tempoInicioServico,
            double tempoSaidaSistema,
            double tempoSistema,
            double tempoFila)
{
    this->tempoChegada = tempoChegada;
    this->tempoServico = tempoServico;
    this->tempoChegadaAcumulativo = tempoChegadaAcumulativo;
    this->tempoInicioServico = tempoInicioServico;
    this->tempoSaidaSistema = tempoSaidaSistema;
    this->tempoSistema = tempoSistema;
    this->tempoFila = tempoFila;
}

double Requisicao::retornaTempoChegada()
{
    return this->tempoChegada;
}

double Requisicao::retornaTempoServico()
{
    return this->tempoServico;
}

double Requisicao::retornaTempoChegadaAcumulativo()
{
    return this->tempoChegadaAcumulativo;
}

double Requisicao::retornaTempoInicioServico()
{
    return this->tempoInicioServico;
}

double Requisicao::retornaTempoSaidaSistema()
{
    return this->tempoSaidaSistema;
}

double Requisicao::retornaTempoSistema()
{
    return this->tempoSistema;
}

double Requisicao::retornaTempoFila()
{
    return this->tempoFila;
}