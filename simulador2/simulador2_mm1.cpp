#include <iostream>
#include <vector>
#include <queue>

#include "simulador2/requisicao.hpp"
#include "simulador2/simulador2_mm2.hpp"

bool(*teste)(Requisicao r1, Requisicao r2)=[](Requisicao r1, Requisicao r2){ return r1.retornaTempoRequisicao() < r2.retornaTempoRequisicao();};


int main()
{
    std::vector<Requisicao> teste;
    std::priority_queue<Requisicao, std::vector<Requisicao>,  decltype(teste)> filaRequisicoes();
}
            