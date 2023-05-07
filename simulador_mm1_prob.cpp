#include "geracao_tempos.hpp"

#include "simulador_mm1_prob.hpp"

#include <queue>
#include <limits>

template <typename T>
void adicionaEntradaVetor(std::vector<T>& v, T entrada)
{
    v.push_back(entrada);
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

std::vector<double> retornaTemposEspecifico(const std::vector<Requisicao>& requisicoes, double (Requisicao::*retornaTempo)())
{
    std::vector<double> tempos;

    for (const auto& requisicao: requisicoes)
        tempos.push_back((const_cast<Requisicao &>(requisicao).*retornaTempo)());

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
        }

        else if (tempoSaidaAtual <= tempoChegadaAtual  && tempoSaidaAtual <= tempoInicioAtual)
        {
            tempoEvento = tempoSaidaAtual;
            mudancaElementosSistema = -1;
            mudancaElementosFila = 0;
            tipoEvento = "SAÍDA";
            temposSaidaIndice++; 
            
        }

        else
        {
            tempoEvento = tempoInicioAtual;
            mudancaElementosSistema = 0;
            mudancaElementosFila = -1;
            tipoEvento = "INÍCIO";
            temposInicioIndice++; 
        }

        Evento novo = Evento(tempoEvento, numeroElementosFila, numeroElementosSistema, tipoEvento);

        adicionaEntradaVetor(ret, novo);

        numeroElementosSistema += mudancaElementosSistema;
        numeroElementosFila += mudancaElementosFila;
    }

    return ret;
}

void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros)
{
    int n = static_cast<int>(parametros.find("n")->second);
    double taxaMediaChegada = parametros.find("taxaMediaChegada")->second;
    double taxaServico = parametros.find("taxaServico")->second;

    std::vector<Requisicao> requisicoes = geraTemposEventos(n, taxaMediaChegada, taxaServico);
    std::vector<Evento> eventos = geraEventosSimulador(n, requisicoes);
    
    std::cout << "Chegada \t chegada_acc \t inicio \t duracao \t saida\n";
    for (auto& e : requisicoes)
    {
        std::cout << e.retornaTempoChegada() << "\t" << e.retornaTempoChegadaAcumulativo() << "\t" <<
        e.retornaTempoInicioServico() << "\t" << e.retornaTempoServico() << "\t" << e.retornaTempoSaidaSistema() << std::endl;

    }
    std::cout << "\n\n\n\n\nTempo \t N na fila \t N no sistema \t Tipo\n";
    for (auto& e : eventos)
    {
       std::cout << e.retornaTempoEvento() << "\t" << e.retornaNumeroElementosFila() << "\t" << e.retornaNumeroElementosSistema() << "\t" << e.retornaTipoEvento() << std::endl;
    }
}

void simulaFilaProbabilisticaMM1(int numIteracoes, double mediaChegada, double mediaServico)
{
    std::unordered_map<std::string, double> parametros = std::unordered_map<std::string, double>();

    parametros.insert({"n", numIteracoes });
    parametros.insert({"mediaChegada", mediaChegada});
    parametros.insert({"servico", mediaServico});
    parametros.insert({"taxaMediaChegada", 1.0 / mediaChegada});
    parametros.insert({"taxaServico", 1.0 / mediaServico});

    iniciaSimulacao(parametros);
}