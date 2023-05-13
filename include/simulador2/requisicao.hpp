#ifndef REQUISICAO_HPP
#define REQUISICAO_HPP

enum TIPO_REQUISICAO
{
    CHEGADA,
    SAIDA
};

/* Guarda os dados para uma requisição que fica na fila do simulador 2. */

class Requisicao
{
    private:

        double tempoRequisicao;
        TIPO_REQUISICAO tipo;

    public:

        Requisicao(double tempoRequisicao, const TIPO_REQUISICAO tipo);

        double retornaTempoRequisicao() const;

        TIPO_REQUISICAO retornaTipoRequisicao() const;
};

#endif