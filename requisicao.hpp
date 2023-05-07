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

        double retornaTempoChegada();

        double retornaTempoServico();

        double retornaTempoChegadaAcumulativo();

        double retornaTempoInicioServico();

        double retornaTempoSaidaSistema();

        double retornaTempoSistema();

        double retornaTempoFila();
};

#endif