#ifndef REQUISICAO_HPP
#define REQUISICAO_HPP

class Requisicao
{
    private:

        double tempoChegada;
        double tempoServico;
        double tempoChegadaAcumulativo;
        double tempoInicioServico;
        double tempoSaidaSistema;
        double tempoSistema = tempoSaidaSistema - tempoChegadaAcumulativo;
        double tempoFila = tempoInicioServico - tempoChegadaAcumulativo;

    public:

        Requisicao(double tempoChegada, double tempoServico, double tempoChegadaAcumulativo,
            double tempoInicioServico,
            double tempoSaidaSistema,
            double tempoSistema,
            double tempoFila);

        double retornaTempoChegada() const;

        double retornaTempoServico() const;

        double retornaTempoChegadaAcumulativo() const;

        double retornaTempoInicioServico() const;

        double retornaTempoSaidaSistema() const;

        double retornaTempoSistema() const;

        double retornaTempoFila() const;
};

#endif