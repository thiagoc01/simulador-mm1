#ifndef METRICAS_HPP
#define METRICAS_HPP

/* Guarda as métricas de uma rodada do simulador. Ou seja, é um elemento da amostra. */

class Metricas
{
    private:

        double numeroMedioProcessosSistema;
        double numeroMedioFilaSistema;
        double tempoMedioSistema;
        double tempoMedioFila;
        double periodoOcupadoGeneralizadoMedio;
        double tempoMedioUmCliente;

    public:

        Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila, double periodoOcupadoGeneralizadoMedio, double tempoMedioUmCliente);

        double retornaNumeroMedioProcessosSistema() const;

        double retornaNumeroMedioFilaSistema() const;

        double retornaTempoMedioSistema() const;

        double retornaTempoMedioFila() const;

        double retornaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaTempoMedioUmCliente() const;
};

#endif