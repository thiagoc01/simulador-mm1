#ifndef METRICAS_HPP
#define METRICAS_HPP

#include <unordered_map>

/* Guarda as quantidades de ocorrências dos tempos de clientes calculados e as quantidades de processos no sistema */ 

struct ContagensTemposSistema 
{
    std::unordered_map<double, int> contagensTemposSistema;
    std::unordered_map<int, double> contagemTemposQtdProcessos;
    int totalOcorrenciasTemposSistema;
    double tempoSimulacao;
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
        double probabilidadeExtincao;
        struct ContagensTemposSistema relacoesQuantidadesTemposNumerosProcessos;
        

    public:

        #ifdef SIMULADOR_2

        Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila, double periodoOcupadoGeneralizadoMedio,
                        double tempoMedioUmCliente, double probabilidadeExtincao);

        #else

        Metricas(double numeroMedioProcessosSistema, double numeroMedioFilaSistema,
                        double tempoMedioSistema, double tempoMedioFila);


        #endif
        
        double retornaNumeroMedioProcessosSistema() const;

        double retornaNumeroMedioFilaSistema() const;

        double retornaTempoMedioSistema() const;

        double retornaTempoMedioFila() const;

        double retornaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaTempoMedioUmCliente() const;

        double retornaProbabilidadeExtincao() const;

        struct ContagensTemposSistema retornaQuantidadesTemposNumsProcessos() const;
};

#endif