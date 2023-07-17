#ifndef METRICAS_HPP
#define METRICAS_HPP

#include <unordered_map>

struct ContagensTemposSistema
{
    std::unordered_map<double, int> contagensTemposSistema;
    std::unordered_map<int, int> contagensNumeroProcessos;
    int totalOcorrenciasTemposSistema = 0;
    int totalOcorrenciasNumeroProcessos = 0;
};

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
        struct ContagensTemposSistema relacoesQuantidadesTemposNumerosProcessos;
        

    public:

        Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila, double periodoOcupadoGeneralizadoMedio,
                        double tempoMedioUmCliente, struct ContagensTemposSistema contagens);

        double retornaNumeroMedioProcessosSistema() const;

        double retornaNumeroMedioFilaSistema() const;

        double retornaTempoMedioSistema() const;

        double retornaTempoMedioFila() const;

        double retornaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaTempoMedioUmCliente() const;

        struct ContagensTemposSistema retornaQuantidadesTemposNumsProcessos() const;
};

#endif