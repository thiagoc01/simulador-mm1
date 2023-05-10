#ifndef REQUISICAO2_HPP
#define REQUISICAO2_HPP

#include <string>

class Requisicao
{
    private:

        double tempoRequisicao;
        std::string tipo;

    public:

        Requisicao(double tempoRequisicao, const std::string& tipo);

        double retornaTempoRequisicao() const;

        std::string retornaTipoRequisicao() const;
};

#endif