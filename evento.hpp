#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

class Evento
{
    private:
    
        double tempoEvento;
        int numeroElementosFila;
        int numeroElementosSistema;
        std::string tipoEvento;

    public:

        Evento(double tempoEvento, int numeroElementosFila, int numeroElementosSistema, const std::string& tipoEvento);

        double retornaTempoEvento();

        int retornaNumeroElementosFila();

        int retornaNumeroElementosSistema();

        std::string retornaTipoEvento();
};

#endif