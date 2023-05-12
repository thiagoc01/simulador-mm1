#ifndef REQUISICAO_HPP
#define REQUISICAO_HPP

//Classe representante da requisição no simulador
class Requisicao
{
    private:

        //tempo de chegada da requisição
        double tempoChegada;

        //tempo de duração de serviço da requisição
        double tempoServico;

        //tempo de chegada acumulada da requisição. Tempo de chegada gerado na distribuição mais o tempo atual do simulador, sendo este cumulativo
        double tempoChegadaAcumulativo;

        //tempo de início de serviço da requisição
        double tempoInicioServico;

        //tempo de saída da requisição
        double tempoSaidaSistema;

        //Tempo total no sistema
        double tempoSistema = tempoSaidaSistema - tempoChegadaAcumulativo;

        //Tempo total de espera na fila
        double tempoFila = tempoInicioServico - tempoChegadaAcumulativo;

    public:

        //Construtor
        Requisicao(double tempoChegada, double tempoServico, double tempoChegadaAcumulativo,
            double tempoInicioServico,
            double tempoSaidaSistema,
            double tempoSistema,
            double tempoFila);

        //getters da classe. Por serem atributos imutáveis, não há métodos para setar novos valores
        double retornaTempoChegada() const;

        double retornaTempoServico() const;

        double retornaTempoChegadaAcumulativo() const;

        double retornaTempoInicioServico() const;

        double retornaTempoSaidaSistema() const;

        double retornaTempoSistema() const;

        double retornaTempoFila() const;
};

#endif