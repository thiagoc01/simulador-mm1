#ifndef SIMULADOR3_MM1_PROB_HPP
#define SIMULADOR3_MM1_PROB_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <concepts>

#include "requisicao.hpp"
#include "evento.hpp"

template <typename T>
void adicionaEntradaVetor(std::vector<T>& v, T entrada);

double retornaTempoAtendimentoSistema(const std::vector<Requisicao>& requisicoes);

std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta);

std::vector<Evento> geraEventosSimulador(const int& n, const std::vector<Requisicao>& requisicoes);

template <typename T, typename V> requires std::is_integral_v<V>
std::vector<double> retornaTemposEspecifico(const std::vector<T>& v, V (T::*retornaTempo)() const);

template <typename T, typename U>
std::vector<U> retornaTemposEspecifico(const std::vector<T>& v, U (T::*retornaTempo)() const);

void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros);

void simulaFilaMM1(int numIteracoes, double taxaChegada, double taxaServico, bool eDeterministico);

#endif