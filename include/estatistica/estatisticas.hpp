#ifndef ESTATISTICAS_HPP
#define ESTATISTICAS_HPP

#include "metricas.hpp"

#include <vector>
#include <unordered_map>
#include <string>

struct EstatisticaTemposSistema
{
    std::unordered_map<double, double> estatisticaTemposSistema;
    std::unordered_map<int, double> estatisticaNumeroProcessos;
};

/* Classe que guarda um conjunto de amostras e a estatística total. */

class Estatisticas
{
    private:
        std::vector<Metricas> amostras;

        /* Equivale às esperanças/médias amostrais */

        double numeroMedioProcessosSistema;
        double numeroMedioFilaSistema;
        double tempoMedioSistema;
        double tempoMedioFila;
        double periodoOcupadoGeneralizadoMedio;
        double tempoMedioUmCliente;
        struct EstatisticaTemposSistema mediaTemposNumerosProcessos;
        std::vector<double> temposSistema, numerosProcessos, densidadeTempos, densidadeNumeros;

        double varianciaProcessosSistema;
        double varianciaFilaSistema;
        double varianciaTempoSistema;
        double varianciaTempoFila;
        double varianciaPeriodoOcupadoGeneralizadoMedio;
        double varianciaTempoMedioUmCliente;
        struct EstatisticaTemposSistema varianciasTemposNumerosProcessos;

        double desvioPadraoProcessosSistema;
        double desvioPadraoFilaSistema;
        double desvioPadraoTempoSistema;
        double desvioPadraoTempoFila;
        double desvioPadraoPeriodoOcupadoGeneralizadoMedio;
        double desvioPadraoTempoMedioUmCliente;
        struct EstatisticaTemposSistema desviosPadroesTemposNumerosProcessos;
        
        /* As strings são "inferior" e "superior" para acessarmos os valores. */
        
        std::unordered_map<std::string, double> intervaloConfiancaProcessosSistema;
        std::unordered_map<std::string, double> intervaloConfiancaFilaSistema;
        std::unordered_map<std::string, double> intervaloConfiancaTempoSistema;
        std::unordered_map<std::string, double> intervaloConfiancaTempoFila;
        std::unordered_map<std::string, double> intervaloConfiancaPeriodoOcupadoGeneralizadoMedio;
        std::unordered_map<std::string, double> intervaloConfiancaTempoMedioUmCliente;
        std::unordered_map<std::string, struct EstatisticaTemposSistema> intervalosConfiancasTemposNumerosProcessos;

    public:
        void adicionaAmostra(const Metricas& amostra);

        void calculaMediaAmostralTempoSistemaQtdProcessos(const Metricas& amostra,
                                    struct EstatisticaTemposSistema& esperancaQuadrado, const bool& eUmaIteracao);

        std::vector<Metricas> retornaMetricas() const;

        void calculaVarianciasDesviosPadroesAmostrais();

        void calculaIntervalosConfianca(const double& probabilidadeTaxaConfianca);

        double retornaNumeroMedioProcessosSistema() const;

        double retornaNumeroMedioFilaSistema() const;

        double retornaTempoMedioSistema() const;

        double retornaTempoMedioFila() const;

        double retornaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaTempoMedioUmCliente() const;

        struct EstatisticaTemposSistema retornaMediasTemposNumerosProcessos() const;

        double retornaVarianciaProcessosSistema() const;

        double retornaVarianciaFilaSistema() const;

        double retornaVarianciaTempoSistema() const;

        double retornaVarianciaTempoFila() const;

        double retornaVarianciaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaVarianciaTempoMedioUmCliente() const;

        struct EstatisticaTemposSistema retornaVarianciasTemposNumerosProcessos() const;

        double retornaDesvioPadraoProcessosSistema() const;

        double retornaDesvioPadraoFilaSistema() const;

        double retornaDesvioPadraoTempoSistema() const;

        double retornaDesvioPadraoTempoFila() const;

        double retornaDesvioPadraoPeriodoOcupadoGeneralizadoMedio() const;

        double retornaDesvioPadraoTempoMedioUmCliente() const;

        struct EstatisticaTemposSistema retornaDesviosPadroesTemposNumerosProcessos() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaProcessosSistema() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaFilaSistema() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaTempoSistema() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaTempoFila() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaPeriodoOcupadoGeneralizadoMedio() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaTempoMedioUmCliente() const;

        std::unordered_map<std::string, struct EstatisticaTemposSistema> retornaIntervalosConfiancasTemposNumerosProcessos() const;

        void calculaVarianciaDesvioPadraoTemposNumerosProcessos(const struct EstatisticaTemposSistema& esperancaQuadradoTemposNumerosProcessos);

        void imprimeAnaliseAmostral(const double& taxaChegada, const double& taxaServico);
};

#endif