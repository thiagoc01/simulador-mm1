#include "estatistica/metricas.hpp"

Metricas::Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila, double periodoOcupadoGeneralizadoMedio,
                        double tempoMedioUmCliente, double probabilidadeExtincao)
{
    this->numeroMedioProcessosSistema = numeroMedioProcessosSistema;
    this->numeroMedioFilaSistema = numeroMedioFilaSistema;
    this->tempoMedioSistema = tempoMedioSistema;
    this->tempoMedioFila = tempoMedioFila;
    this->periodoOcupadoGeneralizadoMedio = periodoOcupadoGeneralizadoMedio;
    this->tempoMedioUmCliente = tempoMedioUmCliente;
    this->probabilidadeExtincao = probabilidadeExtincao;
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

double Metricas::retornaTempoMedioUmCliente() const
{
    return this->tempoMedioUmCliente;
}

double Metricas::retornaProbabilidadeExtincao() const
{
    return this->probabilidadeExtincao;
}

struct ContagensTemposSistema Metricas::retornaQuantidadesTemposNumsProcessos() const
{
    return this->relacoesQuantidadesTemposNumerosProcessos;
}