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

std::vector<Requisicao> geraTemposEventos(const int& n, const double& lambda, const double& beta);

std::vector<Evento> geraEventosSimulador(const int& n, const std::vector<Requisicao>& requisicoes);

void iniciaSimulacao(const std::unordered_map<std::string, double>& parametros);

void simulaFilaProbabilisticaMM1(int numIteracoes, double mediaChegada, double mediaServico);

#endif