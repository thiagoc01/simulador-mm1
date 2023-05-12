#ifndef SIMULADOR3_MM1_PROB_HPP
#define SIMULADOR3_MM1_PROB_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <concepts>

#include "requisicao.hpp"
#include "evento.hpp"
#include "simulador_mm1.hpp"

template <typename T>
//Função responsável por inserir um elemento em um vetor. Utilizada tanto para requisições como para eventos
void adicionaEntradaVetor(std::vector<T>& v, T entrada);

//retorna o tempo de serviço total do sistema, desde o início do atendimento da primeira até o final do atendimento da última requisição
double retornaTempoAtendimentoSistema(const std::vector<Requisicao>& requisicoes);

//Gera o tempo dos eventos baseado nos parâmetros lambda (média de chegadas) e beta (média de tempo de serviço), além da
//variável booleana que decide se o tempo de serviço usado será probabilístico ou determinístico. 
std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta);

//gera cada um dos eventos do simulador, incluindo a chegada, início de serviço e saída das requisições
std::vector<Evento> geraEventosSimulador(const int& n, const std::vector<Requisicao>& requisicoes);


//retorna os tempos de ocorrência de cada elemento de um vetor de requisições de acordo com a função recebida como entrada para pegar o tipo
// de tempo desejado
template <typename T, typename U>
std::vector<U> retornaTemposEspecifico(const std::vector<T>& v, U (T::*retornaTempo)() const);

//função principal, responsável por iniciar as variáveis de acordo com os parâmetros especificados
void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros);

#endif