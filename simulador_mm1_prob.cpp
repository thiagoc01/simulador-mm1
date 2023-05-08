#include "geracao_tempos.hpp"

#include "simulador_mm1_prob.hpp"

#include <queue>
#include <limits>

template <typename T>
void adicionaEntradaVetor(std::vector<T>& v, T entrada)
{
    v.push_back(entrada);
}

double calculaMedia(const std::vector<double>& v)
{
    if (v.empty())
        return 0.0;

    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();    
}

void calculaMetricas(const std::vector<Requisicao>& requisicoes, const std::vector<Evento>& eventos)
{
    double mediaChegadasSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoChegada));
    double taxaMediaChegadasSimulada  = 1.0 / mediaChegadasSimulada;

    double mediaServicosSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoServico));
    double taxaMediaServicosSimulada = 1.0 / mediaServicosSimulada;

    double mediaTempoEsperaSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoFila));

    double mediaTempoRespostaSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoSistema));
    
    std::cout << "Média tempos de chegada/taxa : " << mediaChegadasSimulada << " " << taxaMediaChegadasSimulada << std::endl;
    std::cout << "Média tempos de serviços/taxa : " << mediaServicosSimulada << " " << taxaMediaServicosSimulada << std::endl;
    std::cout << "Média tempos de espera : " << mediaTempoEsperaSimulada << std::endl;
    std::cout << "Média tempos de resposta : " << mediaTempoRespostaSimulada << std::endl;
    std::cout << "Média número de processos no sistema : " << taxaMediaChegadasSimulada * mediaTempoRespostaSimulada << std::endl;
    std::cout << "Média número de processos na fila : " << taxaMediaChegadasSimulada * mediaTempoEsperaSimulada << std::endl;
}

double retornaTempoAtendimentoSistema(const std::vector<Requisicao>& requisicoes)
{
    double ultimoTempoSaida = requisicoes.at(requisicoes.size() - 1).retornaTempoSaidaSistema();
    double primeiroTempoChegada = requisicoes.at(0).retornaTempoSaidaSistema();

    return ultimoTempoSaida - primeiroTempoChegada;
}

void imprimeEventos(const std::vector<Evento>& eventos)
{
    std::cout << "Tempo \t N na fila \t N no sistema \t Tipo \t Ultimo Inicio\n";

    for (const auto& e : eventos)
    {
        if (e.retornaUltimoInicio() > 0.0)
            std::cout << e.retornaTempoEvento() << "\t\t" << e.retornaNumeroElementosFila() << "\t\t" << e.retornaNumeroElementosSistema() << "\t" << e.retornaTipoEvento() << "\t\t" << e.retornaUltimoInicio() << std::endl;
        else
            std::cout << e.retornaTempoEvento() << "\t\t" << e.retornaNumeroElementosFila() << "\t\t" << e.retornaNumeroElementosSistema() << "\t" << e.retornaTipoEvento() << "\t\t" << std::endl;
    }
}

std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta)
{
    std::vector<Requisicao> ret;
    double tempoChegada = retornaTempoPoisson(lambda);
    double tempoServico = retornaTempoExponencial(beta);
    double tempoChegadaAcumulativo = tempoChegada;
    double tempoInicioServico = tempoChegada;
    double tempoSaidaSistema = tempoChegada + tempoServico;
    double tempoSistema = tempoSaidaSistema - tempoChegadaAcumulativo;
    double tempoFila = tempoInicioServico - tempoChegadaAcumulativo;
    Requisicao novo = Requisicao(tempoChegada, tempoServico, tempoChegadaAcumulativo, tempoInicioServico,
                    tempoSaidaSistema, tempoSistema, tempoFila);

    adicionaEntradaVetor(ret, novo);

    for (int i = 1 ; i < n ; i++)
    {
        tempoChegada = retornaTempoPoisson(lambda);
        tempoServico = retornaTempoExponencial(beta);
        tempoChegadaAcumulativo += tempoChegada;

        tempoInicioServico = std::max(tempoChegadaAcumulativo, ret[i - 1].retornaTempoSaidaSistema());
        tempoSaidaSistema = tempoInicioServico + tempoServico;

        tempoSistema = tempoSaidaSistema - tempoChegadaAcumulativo;
        tempoFila = tempoInicioServico - tempoChegadaAcumulativo;
        
        novo = Requisicao(tempoChegada, tempoServico, tempoChegadaAcumulativo, tempoInicioServico,
                    tempoSaidaSistema, tempoSistema, tempoFila);

        adicionaEntradaVetor(ret, novo);
    }

    return ret;
}

template <typename T, typename U>
std::vector<U> retornaTemposEspecifico(const std::vector<T>& v, U (T::*retornaTempo)() const)
{
    std::vector<U> tempos;

    for (const auto& e: v)
        tempos.push_back((const_cast<T &>(e).*retornaTempo)());

    return tempos;
}

double retornaTempoEspecifico(const std::vector<double>& tempos, const int& indice, const int& n)
{
    if (indice < n)
        return tempos[indice];
    else
        return std::numeric_limits<double>::max();
}

std::vector<Evento> geraEventosSimulador(const int& n, const std::vector<Requisicao>& requisicoes)
{
    std::vector<Evento> ret;

    std::vector<double> temposChegada = retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoChegadaAcumulativo);
    std::vector<double> temposInicio = retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoInicioServico);
    std::vector<double> temposSaida = retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoSaidaSistema);

    double tempoEvento = 0.0;
    int numeroElementosFila = 0;
    int numeroElementosSistema = 0;
    int temposChegadaIndice = 0, temposInicioIndice = 0, temposSaidaIndice = 0;
    std::string tipoEvento;
    double ultimoInicio = 0.0;

    while (temposSaidaIndice < n)
    {
        double tempoChegadaAtual = retornaTempoEspecifico(temposChegada, temposChegadaIndice, n);
        double tempoInicioAtual = retornaTempoEspecifico(temposInicio, temposInicioIndice, n);
        double tempoSaidaAtual = retornaTempoEspecifico(temposSaida, temposSaidaIndice, n);
        int mudancaElementosFila, mudancaElementosSistema;

        if (tempoChegadaAtual <= tempoInicioAtual && tempoChegadaAtual <= tempoSaidaAtual)
        {
            tempoEvento = tempoChegadaAtual;
            mudancaElementosSistema = mudancaElementosFila = 1;
            tipoEvento = "CHEGADA";
            temposChegadaIndice++;

            Evento novo = Evento(tempoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento, 0.0);

            adicionaEntradaVetor(ret, novo);
        }

        else if (tempoSaidaAtual <= tempoChegadaAtual  && tempoSaidaAtual <= tempoInicioAtual)
        {
            tempoEvento = tempoSaidaAtual;
            mudancaElementosSistema = -1;
            mudancaElementosFila = 0;
            tipoEvento = "SAÍDA";
            temposSaidaIndice++;

            Evento novo = Evento(tempoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento, ultimoInicio);

            adicionaEntradaVetor(ret, novo);           
        }

        else
        {
            tempoEvento = tempoInicioAtual;
            mudancaElementosSistema = 0;
            mudancaElementosFila = -1;
            tipoEvento = "INÍCIO";
            ultimoInicio = tempoEvento;
            temposInicioIndice++;

            Evento novo = Evento(tempoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento, 0.0);

            adicionaEntradaVetor(ret, novo);
        }

        numeroElementosSistema += mudancaElementosSistema;
        numeroElementosFila += mudancaElementosFila;
    }

    return ret;
}

void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros)
{
    int n = static_cast<int>(parametros.find("n")->second);
    double taxaMediaChegada = parametros.find("mediaChegada")->second;
    double taxaServico = parametros.find("servico")->second;

    std::vector<Requisicao> requisicoes = geraTemposEventos(n, taxaMediaChegada, taxaServico);
    std::vector<Evento> eventos = geraEventosSimulador(n, requisicoes);
    
    imprimeEventos(eventos);

    std::cout << "\n\nTempo de atendimento do sistema: " << retornaTempoAtendimentoSistema(requisicoes) << std::endl;

    calculaMetricas(requisicoes, eventos);
}

void simulaFilaProbabilisticaMM1(int numIteracoes, double mediaChegada, double mediaServico)
{
    std::unordered_map<std::string, double> parametros = std::unordered_map<std::string, double>();

    parametros.insert({"n", numIteracoes });
    parametros.insert({"mediaChegada", mediaChegada});
    parametros.insert({"servico", mediaServico});

    iniciaSimulacao(parametros);
}