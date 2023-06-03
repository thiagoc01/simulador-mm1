#include "grafico/grafico.hpp"

#include <array>
#include <algorithm>

namespace plt = matplotlibcpp;

void realizaAcoesPlotagem(const std::vector<double>& medias, const double& media, std::array<std::string, 3> strings)
{
    plt::hist(medias, 10, "darkmagenta", 1.0, false, {{"edgecolor", "black"}});
    
    /* Printa a linha que representa a média */
    
    plt::axvline(media, 0.0, std::numeric_limits<float>::max(), {{"color", "#74ff74"}, {"linestyle", "--"}, {"label", "Média"}});
    
    plt::xlabel(strings[0]);
    plt::ylabel(strings[1]);

    plt::title(strings[2]);

    plt::legend();

    plt::show();
}

void plotaGrafico(const Estatisticas &estatisticas)
{
    std::vector<Metricas> amostras = estatisticas.retornaMetricas();

    std::vector<double> numerosMediosProcessosSistema;
    std::vector<double> numerosMediosProcessosFila;
    std::vector<double> temposMediosProcessosSistema;
    std::vector<double> temposMediosProcessosFila;
    std::vector<double> periodosOcupadosGeneralizadosMedios;

    std::array<std::string, 3> legendas;

    for (const auto &amostra : amostras)
    {
        numerosMediosProcessosSistema.push_back(amostra.retornaNumeroMedioProcessosSistema());
        numerosMediosProcessosFila.push_back(amostra.retornaNumeroMedioFilaSistema());
        temposMediosProcessosSistema.push_back(amostra.retornaTempoMedioSistema());
        temposMediosProcessosFila.push_back(amostra.retornaTempoMedioFila());
        periodosOcupadosGeneralizadosMedios.push_back(amostra.retornaPeriodoOcupadoGeneralizadoMedio());
    }

    legendas[0] = "Número médio de processos no sistema";
    legendas[1] = "Número de ocorrências";
    legendas[2] = "Histograma do número médio de processos no sistema";

    realizaAcoesPlotagem(numerosMediosProcessosSistema, estatisticas.retornaNumeroMedioProcessosSistema(), legendas);

    legendas[0] = "Número médio de processos na fila";
    legendas[2] = "Histograma do número médio de processos na fila";

    realizaAcoesPlotagem(numerosMediosProcessosFila, estatisticas.retornaNumeroMedioFilaSistema(), legendas);

    legendas[0] = "Tempo médio de processos no sistema";
    legendas[2] = "Histograma do tempo médio de processos no sistema";

    realizaAcoesPlotagem(temposMediosProcessosSistema, estatisticas.retornaTempoMedioSistema(), legendas);

    legendas[0] = "Tempo médio de processos na fila";
    legendas[2] = "Histograma do tempo médio de processos na fila";

    realizaAcoesPlotagem(temposMediosProcessosFila, estatisticas.retornaTempoMedioFila(), legendas);

    legendas[0] = "Período ocupado generalizado médio";
    legendas[2] = "Histograma do período ocupado generalizado médio";

    realizaAcoesPlotagem(periodosOcupadosGeneralizadosMedios, estatisticas.retornaPeriodoOcupadoGeneralizadoMedio(), legendas);
}