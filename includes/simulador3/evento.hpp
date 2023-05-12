#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

//Classe evento, representante de um evento no sistema
class Evento
{
    private:
    
        //tempo de ocorrência do evento
        double tempoEvento;
        
        //diferença de tempo para o evento anterior
        double delayUltimoEvento;

        //número de elementos na fila durante a ocorrência do evento
        int numeroElementosFila;

        
        //número de elementos no sistema durante a ocorrência do evento
        int numeroElementosSistema;

        //ultimo evento de início registrado
        double ultimoInicio;
        
        //tipo do evento: Chegada, Início de serviço ou Saída
        std::string tipoEvento;

    public:

        //Construtor
        Evento(double tempoEvento, double delayUltimoEvento, int numeroElementosFila, int numeroElementosSistema, const std::string& tipoEvento, double ultimoInicio);

        //getters da classe. Por serem atributos imutáveis, não há métodos para setar novos valores
        double retornaTempoEvento() const;

        double retornaDelayUltimoEvento() const;

        int retornaNumeroElementosFila() const;

        int retornaNumeroElementosSistema() const;

        double retornaUltimoInicio() const;

        std::string retornaTipoEvento() const;
};

#endif