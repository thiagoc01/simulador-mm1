#ifndef ESTATISTICAS_HPP
#define ESTATISTICAS_HPP

#include "metricas.hpp"

#include <vector>
#include <unordered_map>
#include <string>

/* Classe que guarda um conjunto de amostras e as estatística total. */

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

        double varianciaProcessosSistema;
        double varianciaFilaSistema;
        double varianciaTempoSistema;
        double varianciaTempoFila;
        double varianciaPeriodoOcupadoGeneralizadoMedio;

        double desvioPadraoProcessosSistema;
        double desvioPadraoFilaSistema;
        double desvioPadraoTempoSistema;
        double desvioPadraoTempoFila;
        double desvioPadraoPeriodoOcupadoGeneralizadoMedio;
        
        /* As strings são "inferior" e "superior" para acessarmos os valores. */
        
        std::unordered_map<std::string, double> intervaloConfiancaProcessosSistema;
        std::unordered_map<std::string, double> intervaloConfiancaFilaSistema;
        std::unordered_map<std::string, double> intervaloConfiancaTempoSistema;
        std::unordered_map<std::string, double> intervaloConfiancaTempoFila;
        std::unordered_map<std::string, double> intervaloConfiancaPeriodoOcupadoGeneralizadoMedio;

    public:
        void adicionaAmostra(const Metricas& amostra);

        std::vector<Metricas> retornaMetricas() const;

        void calculaVarianciasDesviosPadroesAmostrais();

        void calculaIntervalosConfianca(const double& probabilidadeTaxaConfianca);

        double retornaNumeroMedioProcessosSistema() const;

        double retornaNumeroMedioFilaSistema() const;

        double retornaTempoMedioSistema() const;

        double retornaTempoMedioFila() const;

        double retornaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaVarianciaProcessosSistema() const;

        double retornaVarianciaFilaSistema() const;

        double retornaVarianciaTempoSistema() const;

        double retornaVarianciaTempoFila() const;

        double retornaVarianciaPeriodoOcupadoGeneralizadoMedio() const;

        double retornaDesvioPadraoProcessosSistema() const;

        double retornaDesvioPadraoFilaSistema() const;

        double retornaDesvioPadraoTempoSistema() const;

        double retornaDesvioPadraoTempoFila() const;

        double retornaDesvioPadraoPeriodoOcupadoGeneralizadoMedio() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaProcessosSistema() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaFilaSistema() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaTempoSistema() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaTempoFila() const;

        std::unordered_map<std::string, double> retornaIntervaloConfiancaPeriodoOcupadoGeneralizadoMedio() const;

        void imprimeAnaliseAmostral(const double& taxaChegada, const double& taxaServico);
};

#endif