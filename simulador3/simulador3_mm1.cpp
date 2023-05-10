#include "geracao_tempos.hpp"

#include "simulador3/simulador3_mm1.hpp"

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

double calculaEsperanca(const std::vector<double>& v, const std::vector<double>& u, double durTotal)
{
    double soma = 0.0;

    if (v.empty())
        return 0.0;

    for (int i = 0 ; i < v.size() ; i++)
        soma += (v[i] * u[i]);

    return soma / durTotal;
}

void calculaMetricas(const std::vector<Requisicao>& requisicoes, const std::vector<Evento>& eventos, const double& tempoAtendimentoTotal)
{
    double taxaMediaChegadasSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoChegada));
    double mediaChegadasSimulada  = 1.0 / taxaMediaChegadasSimulada;

    double taxaMediaServicosSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoServico));
    double mediaServicosSimulada = 1.0 / taxaMediaServicosSimulada;

    double mediaTempoEsperaSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoFila));
    
    double mediaTempoRespostaSimulada = calculaMedia(retornaTemposEspecifico(requisicoes, &Requisicao::retornaTempoSistema));
    
    double mediaNumeroProcessosSistema = calculaEsperanca(retornaTemposEspecifico(eventos, &Evento::retornaNumeroElementosSistema),
                                                            retornaTemposEspecifico(eventos, &Evento::retornaDelayUltimoEvento), tempoAtendimentoTotal);

    double mediaNumeroProcessosFila = calculaEsperanca(retornaTemposEspecifico(eventos, &Evento::retornaNumeroElementosFila), 
                                            retornaTemposEspecifico(eventos, &Evento::retornaDelayUltimoEvento), tempoAtendimentoTotal);
    std::cout << "Média tempos de chegada/taxa (Lambda e 1/Lambda): " << mediaChegadasSimulada << " " << taxaMediaChegadasSimulada << std::endl;
    std::cout << "Média tempos de serviços/taxa (Mi e 1/Mi): " << mediaServicosSimulada << " " << taxaMediaServicosSimulada << std::endl;
    std::cout << "Média tempos de espera (E(W)): " << mediaTempoEsperaSimulada << std::endl;
    std::cout << "Média tempos de resposta (E(T)): " << mediaTempoRespostaSimulada << std::endl;
    std::cout << "Média número de processos no sistema (E(N)) : " << mediaNumeroProcessosSistema << std::endl;

    std::cout << "Média número de processos no sistema (E(N)) (Lei de Little): " << mediaChegadasSimulada * mediaTempoRespostaSimulada << std::endl;

    std::cout << "Média número de processos na fila (E(N_q)): " << mediaNumeroProcessosFila << std::endl;

    std::cout << "Média número de processos na fila (E(N_q)) (Lei de Little): " << mediaChegadasSimulada * mediaTempoEsperaSimulada << std::endl;
}

double retornaTempoAtendimentoSistema(const std::vector<Requisicao>& requisicoes)
{
    double ultimoTempoSaida = requisicoes.at(requisicoes.size() - 1).retornaTempoSaidaSistema();
    double primeiroTempoChegada = requisicoes.at(0).retornaTempoChegadaAcumulativo();

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

static double (*geraTempoServicoDeterministico)(double mediaServico) = [](double mediaServico){return mediaServico; };
static double (*geraTempoServicoProbabilistico)(double mediaServico) = [](double mediaServico){return retornaTempoExponencial(mediaServico); };


std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta, const bool& eDeterministico)
{
    double (*geraTempoServico)(double mediaServico);

    std::vector<Requisicao> ret;
    double tempoChegada = retornaTempoPoisson(lambda);
    double tempoServico;

    if (eDeterministico)
        geraTempoServico = geraTempoServicoDeterministico;
    else
        geraTempoServico = geraTempoServicoProbabilistico;

    tempoServico = geraTempoServico(beta);
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
        tempoServico = geraTempoServico(beta);
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

template <typename T, typename U, eInteiro<T, U> V>
std::vector<U> retornaTemposEspecifico(const std::vector<T>& v, V (T::*retornaTempo)() const)
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
    double delayUltimoEvento = 0.0;
    double ultimoEvento = 0.0;
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
            delayUltimoEvento = tempoEvento - ultimoEvento;
            mudancaElementosSistema = mudancaElementosFila = 1;
            tipoEvento = "CHEGADA";
            temposChegadaIndice++;

            Evento novo = Evento(tempoEvento, delayUltimoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento, 0.0);

            adicionaEntradaVetor(ret, novo);
        }

        else if (tempoSaidaAtual <= tempoChegadaAtual  && tempoSaidaAtual <= tempoInicioAtual)
        {
            tempoEvento = tempoSaidaAtual;
            delayUltimoEvento = tempoEvento - ultimoEvento;
            mudancaElementosSistema = -1;
            mudancaElementosFila = 0;
            tipoEvento = "SAÍDA";
            temposSaidaIndice++;

            Evento novo = Evento(tempoEvento, delayUltimoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento, ultimoInicio);

            adicionaEntradaVetor(ret, novo);           
        }

        else
        {
            tempoEvento = tempoInicioAtual;
            delayUltimoEvento = tempoEvento - ultimoEvento;
            mudancaElementosSistema = 0;
            mudancaElementosFila = -1;
            tipoEvento = "INÍCIO";
            ultimoInicio = tempoEvento;
            temposInicioIndice++;

            Evento novo = Evento(tempoEvento, delayUltimoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento, 0.0);

            adicionaEntradaVetor(ret, novo);
        }

        numeroElementosSistema += mudancaElementosSistema;
        numeroElementosFila += mudancaElementosFila;

        ultimoEvento = tempoEvento;
    }

    return ret;
}

void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros, const bool& eDeterministico)
{
    int n = static_cast<int>(parametros.find("n")->second);
    double taxaMediaChegada = parametros.find("mediaChegada")->second;
    double taxaServico = parametros.find("mediaServico")->second;
    double tempoAtendimentoTotal;

    std::vector<Requisicao> requisicoes = geraTemposEventos(n, taxaMediaChegada, taxaServico, eDeterministico);
    std::vector<Evento> eventos = geraEventosSimulador(n, requisicoes);
    
    imprimeEventos(eventos);

    tempoAtendimentoTotal = retornaTempoAtendimentoSistema(requisicoes);
    std::cout << "\n\nTempo de atendimento do sistema: " << tempoAtendimentoTotal << std::endl;


    calculaMetricas(requisicoes, eventos, tempoAtendimentoTotal);
}

void simulaFilaMM1(int numIteracoes, double taxaChegada, double taxaServico, bool eDeterministico)
{
    std::unordered_map<std::string, double> parametros = std::unordered_map<std::string, double>();

    parametros.insert({"n", numIteracoes });
    parametros.insert({"taxaChegada", taxaChegada});
    parametros.insert({"taxaServico", taxaServico});
    parametros.insert({"mediaChegada", 1.0 / taxaChegada});
    parametros.insert({"mediaServico", 1.0 / taxaServico});

    iniciaSimulacao(parametros, eDeterministico);
}