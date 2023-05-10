#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

class Evento
{
    private:
    
        double tempoEvento;
        double delayUltimoEvento;
        int numeroElementosFila;
        int numeroElementosSistema;
        double ultimoInicio;
        std::string tipoEvento;

    public:

        Evento(double tempoEvento, double delayUltimoEvento, int numeroElementosFila, int numeroElementosSistema, const std::string& tipoEvento, double ultimoInicio);

        double retornaTempoEvento() const;

        double retornaDelayUltimoEvento() const;

        int retornaNumeroElementosFila() const;

        int retornaNumeroElementosSistema() const;

        double retornaUltimoInicio() const;

        std::string retornaTipoEvento() const;
};

#endif