#include "evento.hpp"

Evento::Evento(double tempoEvento, int numeroElementosFila, int numeroElementosSistema, const std::string& tipoEvento)
{
    this->tempoEvento = tempoEvento;
    this->numeroElementosFila = numeroElementosFila;
    this->numeroElementosSistema = numeroElementosSistema;
    this->tipoEvento = tipoEvento;
}

double Evento::retornaTempoEvento()
{
    return this->tempoEvento;
}

int Evento::retornaNumeroElementosFila()
{
    return this->numeroElementosFila;
}

int Evento::retornaNumeroElementosSistema()
{
    return this->numeroElementosSistema;
}

std::string Evento::retornaTipoEvento()
{
    return this->tipoEvento;
}