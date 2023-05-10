#include "simulador3/evento.hpp"

Evento::Evento(double tempoEvento, double delayUltimoEvento, int numeroElementosFila, int numeroElementosSistema,
                const std::string& tipoEvento, double ultimoInicio)
{
    this->tempoEvento = tempoEvento;
    this->delayUltimoEvento = delayUltimoEvento;
    this->numeroElementosFila = numeroElementosFila;
    this->numeroElementosSistema = numeroElementosSistema;
    this->tipoEvento = tipoEvento;
    this->ultimoInicio = ultimoInicio;
}

double Evento::retornaTempoEvento() const
{
    return this->tempoEvento;
}

double Evento::retornaDelayUltimoEvento() const
{
    return this->delayUltimoEvento;
}

int Evento::retornaNumeroElementosFila() const
{
    return this->numeroElementosFila;
}

int Evento::retornaNumeroElementosSistema() const
{
    return this->numeroElementosSistema;
}

std::string Evento::retornaTipoEvento() const
{
    return this->tipoEvento;
}

double Evento::retornaUltimoInicio() const
{
    return this->ultimoInicio;
}