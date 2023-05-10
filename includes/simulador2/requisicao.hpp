#ifndef REQUISICAO_HPP
#define REQUISICAO_HPP

#include <string>

enum TIPO_CHEGADA
{
    CHEGADA,
    SAIDA
};

class Requisicao
{
    private:

        double tempoRequisicao;
        TIPO_CHEGADA tipo;

    public:

        Requisicao(double tempoRequisicao, const TIPO_CHEGADA tipo);

        double retornaTempoRequisicao() const;

        TIPO_CHEGADA retornaTipoRequisicao() const;
};

#endif