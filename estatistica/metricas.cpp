#include "estatistica/metricas.hpp"

Metricas::Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila, double periodoOcupadoGeneralizadoMedio)
{
    this->numeroMedioProcessosSistema = numeroMedioProcessosSistema;
    this->numeroMedioFilaSistema = numeroMedioFilaSistema;
    this->tempoMedioSistema = tempoMedioSistema;
    this->tempoMedioFila = tempoMedioFila;
    this->periodoOcupadoGeneralizadoMedio = periodoOcupadoGeneralizadoMedio;
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

double Metricas::retornaPeriodoOcupadoGeneralizadoMedio() const
{
    return this->periodoOcupadoGeneralizadoMedio;
}