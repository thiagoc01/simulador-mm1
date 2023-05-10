#ifndef SIMULADOR_MM1_PROB_HPP
#define SIMULADOR_MM1_PROB_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include "requisicao.hpp"
#include "evento.hpp"

template <typename T>
void adicionaEntradaVetor(std::vector<T>& v, T entrada);

double retornaTempoAtendimentoSistema(const std::vector<Requisicao>& requisicoes);

std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta, const bool& eDeterministic);

std::vector<Evento> geraEventosSimulador(const int& n, const std::vector<Requisicao>& requisicoeso);

template <typename T, typename U>
std::vector<U> retornaTemposEspecifico(const std::vector<T>& v, U (T::*retornaTempo)() const);

std::vector<double> retornaTemposEspecifico(const std::vector<Evento>& v, int (Evento::*retornaTempo)() const);

void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros);

void simulaFilaMM1(int numIteracoes, double taxaChegada, double taxaServico, bool eDeterministico);

#endif