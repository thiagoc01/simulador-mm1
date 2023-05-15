#include "geracao_tempos.hpp"

#include "simulador3/simulador3_mm1.hpp"
#include "estatistica/estatisticas.hpp"
#include "estatistica/metricas.hpp"
#include "thread/thread.hpp"

#include <queue>
#include <limits>
#include <mutex>

/*
* Trabalho de Simulação 3 - Extra
* Integrantes: Augusto Guimarães, Luiz Rodrigo Lace, Livia Fonseca, Pedro Henrique Bernardino e Thiago Figueiredo 
*
* Este Trabalho consiste em utilizar uma nova abordagem para simular a chegada de requisições num sistema, dadas a
* taxa de serviço e a de chegada. O simulador gera uma distribuição para as chegadas e serviços, roda uma simulação de gerenciamento
* da fila do sistema com esses dados, e por fim gera métricas de saída com base nos resultados obtidos.
*/

Estatisticas estatisticas;

static std::mutex mutexEstatisticas;

//Função responsável por inserir um elemento em um vetor. Utilizada tanto para requisições como para eventos
template <typename T>
void adicionaEntradaVetor(std::vector<T>& v, T entrada)
{
    v.push_back(entrada);
}

//Função auxiliar responsável por calcular a média dos valores de um vetor. Utilizada no cálculo das métricas
double calculaMedia(const std::vector<double>& v)
{
    if (v.empty())
        return 0.0;

    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();    
}

//Função utilizada no cálculo das métricas. Calcula a Esperança de uma váriável aleatória com base na sua distribuição
double calculaEsperanca(const std::vector<double>& v, const std::vector<double>& u, double durTotal)
{
    double soma = 0.0;

    if (v.empty())
        return 0.0;

    for (int i = 0 ; i < v.size() ; i++)
        soma += (v[i] * u[i]);

    return soma / durTotal;
}

//Função principal para calcular as métricas após a rodada da simulação , dada a lista de requisições, de eventos e do tempo total de atendimento
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
    
    #if NUM_THREADS == 1 && TAMANHO_AMOSTRA == 1
    
    std::cout << "Média tempos de chegada/taxa (Lambda e 1/Lambda): " << mediaChegadasSimulada << " " << taxaMediaChegadasSimulada << std::endl;
    std::cout << "Média tempos de serviços/taxa (Mi e 1/Mi): " << mediaServicosSimulada << " " << taxaMediaServicosSimulada << std::endl;
    std::cout << "Média tempos de espera (E(W)): " << mediaTempoEsperaSimulada << std::endl;
    std::cout << "Média tempos de resposta (E(T)): " << mediaTempoRespostaSimulada << std::endl;
    std::cout << "Média número de processos no sistema (E(N)) : " << mediaNumeroProcessosSistema << std::endl;

    std::cout << "Média número de processos no sistema (E(N)) (Lei de Little): " << mediaChegadasSimulada * mediaTempoRespostaSimulada << std::endl;

    std::cout << "Média número de processos na fila (E(N_q)): " << mediaNumeroProcessosFila << std::endl;

    std::cout << "Média número de processos na fila (E(N_q)) (Lei de Little): " << mediaChegadasSimulada * mediaTempoEsperaSimulada << std::endl;

    std::cout << std::endl;

    #endif

    mutexEstatisticas.lock();

    estatisticas.adicionaAmostra(Metricas(mediaChegadasSimulada * mediaTempoRespostaSimulada, mediaChegadasSimulada * mediaTempoEsperaSimulada, mediaTempoRespostaSimulada, mediaTempoEsperaSimulada));

    mutexEstatisticas.unlock();
}

//retorna o tempo de serviço total do sistema, desde o início do atendimento da primeira até o final do atendimento da última requisição
double retornaTempoAtendimentoSistema(const std::vector<Requisicao>& requisicoes)
{
    double ultimoTempoSaida = requisicoes.at(requisicoes.size() - 1).retornaTempoSaidaSistema();
    double primeiroTempoChegada = requisicoes.at(0).retornaTempoChegadaAcumulativo();

    return ultimoTempoSaida - primeiroTempoChegada;
}

//Função que imprime os dados de cada evento ocorrido na simulação.
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

//Função que gera os valores determinísticos de tempo de serviço
static double (*geraTempoServicoDeterministico)(double taxaServico) = [](double taxaServico){return 1.0 / taxaServico; };

//Função que gera os valores probabilísticos de tempo de serviço com base em uma distribuição exponencial, cuja média é passada por parâmetro
static double (*geraTempoServicoProbabilistico)(double taxaServico) = [](double taxaServico){return retornaTempoExponencial(taxaServico); };

//Gera o tempo dos eventos baseado nos parâmetros lambda (média de chegadas) e beta (média de tempo de serviço), além da
//variável booleana que decide se o tempo de serviço usado será probabilístico ou determinístico. 
std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta, const bool& eDeterministico)
{
    double (*geraTempoServico)(double mediaServico);

    std::vector<Requisicao> ret;
    double tempoChegada = retornaTempoPoisson(lambda);
    double tempoServico;

    //gera o tempo de serviço dos eventos de forma determinística ou probabilística, conforme determinado no parâmetro de entrada
    if (eDeterministico)
        geraTempoServico = geraTempoServicoDeterministico;
    else
        geraTempoServico = geraTempoServicoProbabilistico;

    //gerando os dados da primeira requisição
    tempoServico = geraTempoServico(beta);
    double tempoChegadaAcumulativo = tempoChegada;
    double tempoInicioServico = tempoChegada;
    double tempoSaidaSistema = tempoChegada + tempoServico;
    double tempoSistema = tempoSaidaSistema - tempoChegadaAcumulativo;
    double tempoFila = tempoInicioServico - tempoChegadaAcumulativo;
    Requisicao novo = Requisicao(tempoChegada, tempoServico, tempoChegadaAcumulativo, tempoInicioServico,
                    tempoSaidaSistema, tempoSistema, tempoFila);

    //adiciona a requisição ao vetor
    adicionaEntradaVetor(ret, novo);

    //gera o tempo de chegada, início de serviço e saída de todas as requisições
    for (int i = 1 ; i < n ; i++)
    {
        tempoChegada = retornaTempoPoisson(lambda);
        tempoServico = geraTempoServico(beta);

        //gera o tempo de chegada da requisição atual com base no tempo atual de simulação e do valor gerado na distribuição
        tempoChegadaAcumulativo += tempoChegada;

        //o tempo de serviço é o tempo que a ultima requisição já foi atendida e a atual já chegou. Portanto , o maior valor entre 
        //o tempo dos dois eventos
        tempoInicioServico = std::max(tempoChegadaAcumulativo, ret[i - 1].retornaTempoSaidaSistema());

        //o tempo de saída do sistema é um cálculo simples: é o tempo de início mais o tempo de serviço do sistema
        tempoSaidaSistema = tempoInicioServico + tempoServico;

        //tempo total no sistema
        tempoSistema = tempoSaidaSistema - tempoChegadaAcumulativo;

        //tempo de espera na fila
        tempoFila = tempoInicioServico - tempoChegadaAcumulativo;
        
        novo = Requisicao(tempoChegada, tempoServico, tempoChegadaAcumulativo, tempoInicioServico,
                    tempoSaidaSistema, tempoSistema, tempoFila);

        adicionaEntradaVetor(ret, novo);
    }

    return ret;
}

//retorna os tempos de ocorrência de cada elemento de um vetor de requisições de acordo com a função recebida como entrada para pegar o tipo
// de tempo desejado
template <typename T, typename V> requires std::is_integral_v<V>
std::vector<double> retornaTemposEspecifico(const std::vector<T>& v, V (T::*retornaTempo)() const)
{
    std::vector<double> tempos;

    for (const auto& e: v)
        tempos.push_back((const_cast<T &>(e).*retornaTempo)());

    return tempos;
}

//Retorna os tempos de ocorrência de cada elemento de um vetor de eventos
template <typename T, typename U>
std::vector<U> retornaTemposEspecifico(const std::vector<T>& v, U (T::*retornaTempo)() const)
{
    std::vector<U> tempos;

    for (const auto& e: v)
        tempos.push_back((const_cast<T &>(e).*retornaTempo)());

    return tempos;
}

//Retorna o indice-ésimo valor de um vetor de números representando o tempo
double retornaTempoEspecifico(const std::vector<double>& tempos, const int& indice, const int& n)
{
    if (indice < n)
        return tempos[indice];
    else
        return std::numeric_limits<double>::max();
}

//gera cada um dos eventos do simulador, incluindo a chegada, início de serviço e saída das requisições
std::vector<Evento> geraEventosSimulador(const int& n, const std::vector<Requisicao>& requisicoes)
{
    std::vector<Evento> ret;

    //retorna os tempos de chegada, início e saída das requisições
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

    //loop que organiza as ocorrências dos eventos em ordem cronológica
    while (temposSaidaIndice < n)
    {
        double tempoChegadaAtual = retornaTempoEspecifico(temposChegada, temposChegadaIndice, n);
        double tempoInicioAtual = retornaTempoEspecifico(temposInicio, temposInicioIndice, n);
        double tempoSaidaAtual = retornaTempoEspecifico(temposSaida, temposSaidaIndice, n);
        int mudancaElementosFila, mudancaElementosSistema;

        //o próximo evento é a chegada de uma requisição
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

        //o próximo evento é a saída de uma requisição
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

        //o próximo evento é o início de um serviço
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

        //iterador de tempo. No caso deste simulador, o tempo não é discreto, então o iterador anda de evento em evento, até a saída
        // da última requisição
        ultimoEvento = tempoEvento;
    }

    return ret;
}

//função principal, responsável por iniciar as variáveis de acordo com os parâmetros especificados
void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros, const bool& eDeterministico)
{
    int n = static_cast<int>(parametros.find("n")->second);
    double taxaMediaChegada = parametros.find("taxaChegada")->second;
    double taxaServico = parametros.find("taxaServico")->second;
    double tempoAtendimentoTotal;

    std::vector<Requisicao> requisicoes = geraTemposEventos(n, taxaMediaChegada, taxaServico, eDeterministico);
    std::vector<Evento> eventos = geraEventosSimulador(n, requisicoes);
    
    #if NUM_THREADS == 1 && TAMANHO_AMOSTRA == 1

    imprimeEventos(eventos);

    #endif

    tempoAtendimentoTotal = retornaTempoAtendimentoSistema(requisicoes);
    
    #if NUM_THREADS == 1 && TAMANHO_AMOSTRA == 1

    std::cout << "\n\nTempo de atendimento do sistema: " << tempoAtendimentoTotal << std::endl;

    #endif

    calculaMetricas(requisicoes, eventos, tempoAtendimentoTotal);
}

//chama o simulador com os parametros de uma simulação de fila M/M/1
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