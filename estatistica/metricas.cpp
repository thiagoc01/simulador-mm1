#include "estatistica/metricas.hpp"

Metricas::Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila)
{
    this->numeroMedioProcessosSistema = numeroMedioProcessosSistema;
    this->numeroMedioFilaSistema = numeroMedioFilaSistema;
    this->tempoMedioSistema = tempoMedioSistema;
    this->tempoMedioFila = tempoMedioFila;
}

double Metricas::retornaNumeroMedioProcessosSistema() const
{
    return this->numeroMedioProcessosSistema;
}

double Metricas::retornaNumeroMedioFilaSistema() const
{
    return this->numeroMedioFilaSistema;
}

double Metricas::retornaTempoMedioSistema() const
{
    return this->tempoMedioSistema;
}

double Metricas::retornaTempoMedioFila() const
{
    return this->tempoMedioFila;
}