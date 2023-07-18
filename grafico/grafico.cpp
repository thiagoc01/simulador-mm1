#include "grafico/grafico.hpp"

#include <array>
#include <algorithm>

namespace plt = matplotlibcpp;

void plotaCDFTemposDeSistemaNumerosProcessos(const Estatisticas& estatisticas)
{
    std::vector<double> funcaoAcumuladaTempos(estatisticas.retornaDensidadeTempos().size()),
                    funcaoAcumuladaNumeros(estatisticas.retornaDensidadeNumeros().size()),
                    funcaoAcumuladaTemposInferior(estatisticas.retornaDensidadeTemposInferior().size()),
                    funcaoAcumuladaNumerosInferior(estatisticas.retornaDensidadeNumerosInferior().size()),
                    funcaoAcumuladaTemposSuperior(estatisticas.retornaDensidadeTemposSuperior().size()),
                    funcaoAcumuladaNumerosSuperior(estatisticas.retornaDensidadeNumerosSuperior().size());

    std::partial_sum(estatisticas.retornaDensidadeTempos().begin(), estatisticas.retornaDensidadeTempos().end(), funcaoAcumuladaTempos.begin());
    std::partial_sum(estatisticas.retornaDensidadeNumeros().begin(), estatisticas.retornaDensidadeNumeros().end(), funcaoAcumuladaNumeros.begin());

    std::partial_sum(estatisticas.retornaDensidadeTemposInferior().begin(), estatisticas.retornaDensidadeTemposInferior().end(), funcaoAcumuladaTemposInferior.begin());
    std::partial_sum(estatisticas.retornaDensidadeNumerosInferior().begin(), estatisticas.retornaDensidadeNumerosInferior().end(), funcaoAcumuladaNumerosInferior.begin());

    std::partial_sum(estatisticas.retornaDensidadeTemposSuperior().begin(),estatisticas.retornaDensidadeTemposSuperior().end(), funcaoAcumuladaTemposSuperior.begin());
    std::partial_sum(estatisticas.retornaDensidadeNumerosSuperior().begin(), estatisticas.retornaDensidadeNumerosSuperior().end(), funcaoAcumuladaNumerosSuperior.begin());
    
    
    plt::plot(estatisticas.retornaTemposSistema(), funcaoAcumuladaTempos ,{{"label" , "CDF simulada"}});
    plt::plot(estatisticas.retornaTemposSistema(), funcaoAcumuladaTemposInferior, {{"label" , "CDF simulada do limite inferior"}});
    plt::plot(estatisticas.retornaTemposSistema(), funcaoAcumuladaTemposSuperior, {{"label" , "CDF simulada do limite superior"}});

    plt::xlabel("Tempo de sistema");
    plt::ylabel("Probabilidade de ocorrência do tempo de sistema");

    plt::title("CDF do tempo de clientes no sistema");

    plt::legend();
    
    plt::show();
    
    plt::plot(estatisticas.retornaNumerosProcessos(), funcaoAcumuladaNumeros, {{"drawstyle", "steps-post"}, {"label" , "CDF simulada"}});
    plt::plot(estatisticas.retornaNumerosProcessos(), funcaoAcumuladaNumerosInferior, {{"drawstyle", "steps-post"}, {"label" , "CDF simulada do limite inferior"}});
    plt::plot(estatisticas.retornaNumerosProcessos(), funcaoAcumuladaNumerosSuperior, {{"drawstyle", "steps-post"}, {"label" , "CDF simulada do limite superior"}});


    plt::xlabel("Número de processos na fila");
    plt::ylabel("Probabilidade de ocorrência");

    plt::title("CDF do número de processos na fila");

    plt::legend();

    plt::show();
}

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
    std::vector<double> temposMediosUmCliente;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    std::vector<double> periodosOcupadosGeneralizadosMedios;

    #endif

    std::array<std::string, 3> legendas;

    for (const auto &amostra : amostras)
    {
        numerosMediosProcessosSistema.push_back(amostra.retornaNumeroMedioProcessosSistema());
        numerosMediosProcessosFila.push_back(amostra.retornaNumeroMedioFilaSistema());
        temposMediosProcessosSistema.push_back(amostra.retornaTempoMedioSistema());
        temposMediosProcessosFila.push_back(amostra.retornaTempoMedioFila());

        #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

        periodosOcupadosGeneralizadosMedios.push_back(amostra.retornaPeriodoOcupadoGeneralizadoMedio());
        temposMediosUmCliente.push_back(amostra.retornaTempoMedioUmCliente());

        #endif
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

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    legendas[0] = "Período ocupado generalizado médio";
    legendas[2] = "Histograma do período ocupado generalizado médio";

    realizaAcoesPlotagem(periodosOcupadosGeneralizadosMedios, estatisticas.retornaPeriodoOcupadoGeneralizadoMedio(), legendas);

    legendas[0] = "Tempo médio até atingir um cliente dado C clientes";
    legendas[2] = "Histograma do tempo médio até atingir um cliente";

    realizaAcoesPlotagem(temposMediosUmCliente, estatisticas.retornaTempoMedioUmCliente(), legendas);

    #endif

    plotaCDFTemposDeSistemaNumerosProcessos(estatisticas);
}