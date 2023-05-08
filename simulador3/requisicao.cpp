#include "simulador3/requisicao.hpp"

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

double Requisicao::retornaTempoChegada() const
{
    return this->tempoChegada;
}

double Requisicao::retornaTempoServico() const
{
    return this->tempoServico;
}

double Requisicao::retornaTempoChegadaAcumulativo() const
{
    return this->tempoChegadaAcumulativo;
}

double Requisicao::retornaTempoInicioServico() const
{
    return this->tempoInicioServico;
}

double Requisicao::retornaTempoSaidaSistema() const
{
    return this->tempoSaidaSistema;
}

double Requisicao::retornaTempoSistema() const
{
    return this->tempoSistema;
}

double Requisicao::retornaTempoFila() const
{
    return this->tempoFila;
}