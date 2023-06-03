#include "estatistica/estatisticas.hpp"

#include <cmath>
#include <iostream>

/* Coloca mais uma amostra para análise. */

void Estatisticas::adicionaAmostra(const Metricas& amostra)
{
    this->amostras.push_back(amostra);
}

/* Aplica a fórmula da variância para cada métrica de cada amostra. */

void Estatisticas::calculaVarianciasDesviosPadroesAmostrais()
{
    /* Equivalem a E(X linha) */
    
    this->numeroMedioProcessosSistema = 0.0;
    this->numeroMedioFilaSistema = 0.0;
    this->tempoMedioSistema = 0.0;
    this->tempoMedioFila = 0.0;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    this->periodoOcupadoGeneralizadoMedio = 0.0;
    this->tempoMedioUmCliente = 0.0;

    #endif

    /* Equivalem a E(X linha ^ 2) */

    double esperancaQuadradoProcessosSistema = 0.0;
    double esperancaQuadradoFilaSistema = 0.0;
    double esperancaQuadradoTempoMedioSistema = 0.0;
    double esperancaQuadradoTempoMedioFila = 0.0;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    double esperancaQuadradoPeriodoOcupadoGeneralizadoMedio = 0.0;
    double esperancaQuadradoTempoMedioUmCliente = 0.0;

    #endif

    /* Realiza o somatório do numerador de E(X) e E(X^2) */

    for (const auto& amostra : this->amostras)
    {
        this->numeroMedioProcessosSistema += amostra.retornaNumeroMedioProcessosSistema();
        this->numeroMedioFilaSistema += amostra.retornaNumeroMedioFilaSistema();
        this->tempoMedioSistema += amostra.retornaTempoMedioSistema();
        this->tempoMedioFila += amostra.retornaTempoMedioFila();

        #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
        this->periodoOcupadoGeneralizadoMedio += amostra.retornaPeriodoOcupadoGeneralizadoMedio();
        this->tempoMedioUmCliente += amostra.retornaTempoMedioUmCliente();

        #endif
        
        esperancaQuadradoProcessosSistema += (amostra.retornaNumeroMedioProcessosSistema() * amostra.retornaNumeroMedioProcessosSistema());
        esperancaQuadradoFilaSistema += (amostra.retornaNumeroMedioFilaSistema() * amostra.retornaNumeroMedioFilaSistema());
        esperancaQuadradoTempoMedioSistema += (amostra.retornaTempoMedioSistema() * amostra.retornaTempoMedioSistema());
        esperancaQuadradoTempoMedioFila += (amostra.retornaTempoMedioFila() * amostra.retornaTempoMedioFila());

        #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
        esperancaQuadradoPeriodoOcupadoGeneralizadoMedio += (amostra.retornaPeriodoOcupadoGeneralizadoMedio() * amostra.retornaPeriodoOcupadoGeneralizadoMedio());
        esperancaQuadradoTempoMedioUmCliente += (amostra.retornaTempoMedioUmCliente() * amostra.retornaTempoMedioUmCliente());

        #endif
    }

    /* As 8 linhas a seguir dividem o numerador pelo tamanho da amostra para obter E(X) e E(X ^ 2) */

    this->numeroMedioProcessosSistema /= this->amostras.size();
    this->numeroMedioFilaSistema /= this->amostras.size();
    this->tempoMedioSistema /= this->amostras.size();
    this->tempoMedioFila /= this->amostras.size();
    
    esperancaQuadradoProcessosSistema /= this->amostras.size();
    esperancaQuadradoFilaSistema /= this->amostras.size();
    esperancaQuadradoTempoMedioSistema /= this->amostras.size();
    esperancaQuadradoTempoMedioFila /= this->amostras.size();

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    this->periodoOcupadoGeneralizadoMedio /= this->amostras.size();
    this->tempoMedioUmCliente /= this->amostras.size();

    esperancaQuadradoPeriodoOcupadoGeneralizadoMedio /= this->amostras.size();
    esperancaQuadradoTempoMedioUmCliente /= this->amostras.size();

    #endif

    /* Calcula a variância e desvio padrão de acordo com a definição. */

    this->varianciaProcessosSistema = esperancaQuadradoProcessosSistema - (this->numeroMedioProcessosSistema * this->numeroMedioProcessosSistema);
    this->varianciaFilaSistema = esperancaQuadradoFilaSistema - (this->numeroMedioFilaSistema * this->numeroMedioFilaSistema);
    this->varianciaTempoSistema = esperancaQuadradoTempoMedioSistema - (this->tempoMedioSistema * this->tempoMedioSistema);
    this->varianciaTempoFila = esperancaQuadradoTempoMedioFila - (this->tempoMedioFila * this->tempoMedioFila);

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    this->varianciaPeriodoOcupadoGeneralizadoMedio = esperancaQuadradoPeriodoOcupadoGeneralizadoMedio - (this->periodoOcupadoGeneralizadoMedio * this->periodoOcupadoGeneralizadoMedio);
    this->varianciaTempoMedioUmCliente = esperancaQuadradoTempoMedioUmCliente - (this->tempoMedioUmCliente * this->tempoMedioUmCliente);

    #endif

    this->desvioPadraoProcessosSistema = std::sqrt(this->varianciaProcessosSistema);
    this->desvioPadraoFilaSistema = std::sqrt(this->varianciaFilaSistema);
    this->desvioPadraoTempoSistema = std::sqrt(this->varianciaTempoSistema);
    this->desvioPadraoTempoFila = std::sqrt(this->varianciaTempoFila);

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    this->desvioPadraoPeriodoOcupadoGeneralizadoMedio = std::sqrt(this->varianciaPeriodoOcupadoGeneralizadoMedio);
    this->desvioPadraoTempoMedioUmCliente = std::sqrt(this->varianciaTempoMedioUmCliente);

    #endif
}

static void calculaLimites(std::unordered_map<std::string, double>& intervalosConfianca, const double& probabilidadeTaxaConfianca,
                            const double& valorMedio, const double& desvioPadrao, const int& tamanhoAmostral)
{
    double limiteInferior;
    double limiteSuperior;

    limiteInferior = valorMedio - probabilidadeTaxaConfianca * (desvioPadrao / std::sqrt(tamanhoAmostral));

    limiteSuperior = valorMedio + probabilidadeTaxaConfianca * (desvioPadrao / std::sqrt(tamanhoAmostral));

    intervalosConfianca.insert({"inferior", limiteInferior});
    intervalosConfianca.insert({"superior", limiteSuperior});
}

/* Aplica a fórmula do IC para cada estatística. */

void Estatisticas::calculaIntervalosConfianca(const double& probabilidadeTaxaConfianca)
{
    const int tamanhoAmostral = this->amostras.size();

    calculaLimites(this->intervaloConfiancaProcessosSistema, probabilidadeTaxaConfianca, this->numeroMedioProcessosSistema,
                    this->desvioPadraoProcessosSistema, tamanhoAmostral);

    calculaLimites(this->intervaloConfiancaFilaSistema, probabilidadeTaxaConfianca, this->numeroMedioFilaSistema,
                    this->desvioPadraoFilaSistema, tamanhoAmostral);

    calculaLimites(this->intervaloConfiancaTempoSistema, probabilidadeTaxaConfianca, this->tempoMedioSistema,
                    this->desvioPadraoTempoSistema, tamanhoAmostral);
    
    calculaLimites(this->intervaloConfiancaTempoFila, probabilidadeTaxaConfianca, this->tempoMedioFila,
                    this->desvioPadraoTempoFila, tamanhoAmostral);

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    calculaLimites(this->intervaloConfiancaPeriodoOcupadoGeneralizadoMedio, probabilidadeTaxaConfianca, this->periodoOcupadoGeneralizadoMedio,
                    this->desvioPadraoPeriodoOcupadoGeneralizadoMedio, tamanhoAmostral);

    calculaLimites(this->intervaloConfiancaTempoMedioUmCliente, probabilidadeTaxaConfianca, this->tempoMedioUmCliente,
                    this->desvioPadraoTempoMedioUmCliente, tamanhoAmostral);

    #endif
}

double Estatisticas::retornaNumeroMedioProcessosSistema() const
{
    return this->numeroMedioProcessosSistema;
}

double Estatisticas::retornaNumeroMedioFilaSistema() const
{
    return this->numeroMedioFilaSistema;
}

double Estatisticas::retornaTempoMedioSistema() const
{
    return this->tempoMedioSistema;
}

double Estatisticas::retornaTempoMedioFila() const
{
    return this->tempoMedioFila;
}

double Estatisticas::retornaPeriodoOcupadoGeneralizadoMedio() const
{
    return this->periodoOcupadoGeneralizadoMedio;
}

double Estatisticas::retornaTempoMedioUmCliente() const
{
    return this->tempoMedioUmCliente;
}

double Estatisticas::retornaVarianciaProcessosSistema() const
{
    return this->varianciaProcessosSistema;
}

double Estatisticas::retornaVarianciaFilaSistema() const
{
    return this->varianciaFilaSistema;
}

double Estatisticas::retornaVarianciaTempoSistema() const
{
    return this->varianciaTempoSistema;
}

double Estatisticas::retornaVarianciaTempoFila() const
{
    return this->varianciaTempoFila;
}

double Estatisticas::retornaVarianciaPeriodoOcupadoGeneralizadoMedio() const
{
    return this->varianciaPeriodoOcupadoGeneralizadoMedio;
}

double Estatisticas::retornaVarianciaTempoMedioUmCliente() const
{
    return this->varianciaTempoMedioUmCliente;
}

double Estatisticas::retornaDesvioPadraoProcessosSistema() const
{
    return this->desvioPadraoProcessosSistema;
}

double Estatisticas::retornaDesvioPadraoFilaSistema() const
{
    return this->desvioPadraoFilaSistema;
}

double Estatisticas::retornaDesvioPadraoTempoSistema() const
{
    return this->desvioPadraoTempoSistema;
}

double Estatisticas::retornaDesvioPadraoTempoFila() const
{
    return this->desvioPadraoTempoFila;
}

double Estatisticas::retornaDesvioPadraoPeriodoOcupadoGeneralizadoMedio() const
{
    return this->desvioPadraoPeriodoOcupadoGeneralizadoMedio;
}

double Estatisticas::retornaDesvioPadraoTempoMedioUmCliente() const
{
    return this->desvioPadraoTempoMedioUmCliente;
}

std::unordered_map<std::string, double> Estatisticas::retornaIntervaloConfiancaProcessosSistema() const
{
    return this->intervaloConfiancaProcessosSistema;
}

std::unordered_map<std::string, double> Estatisticas::retornaIntervaloConfiancaFilaSistema() const
{
    return this->intervaloConfiancaFilaSistema;
}

std::unordered_map<std::string, double> Estatisticas::retornaIntervaloConfiancaTempoSistema() const
{
    return this->intervaloConfiancaTempoSistema;
}

std::unordered_map<std::string, double> Estatisticas::retornaIntervaloConfiancaTempoFila() const
{
    return this->intervaloConfiancaTempoFila;
}

std::unordered_map<std::string, double> Estatisticas::retornaIntervaloConfiancaPeriodoOcupadoGeneralizadoMedio() const
{
    return this->intervaloConfiancaPeriodoOcupadoGeneralizadoMedio;
}

std::unordered_map<std::string, double> Estatisticas::retornaIntervaloConfiancaTempoMedioUmCliente() const
{
    return this->intervaloConfiancaTempoMedioUmCliente;
}

void Estatisticas::imprimeAnaliseAmostral(const double& taxaChegada, const double& taxaServico)
{
    extern int numClientes;
    double tempoMedioPeriodoOcupadoAnalitico = (1 / taxaServico) / (1 - (taxaChegada / taxaServico));

    std::cout << "Média amostral de processos no sistema (E(N)): " << this->retornaNumeroMedioProcessosSistema() << std::endl;
    std::cout << "Média amostral de processos na fila (E(N_q)): " << this->retornaNumeroMedioFilaSistema() << std::endl;     
    std::cout << "Média amostral de tempo no sistema (E(T)): " << this->retornaTempoMedioSistema() << std::endl; 
    std::cout << "Média amostral de tempo na fila (E(W)): " << this->retornaTempoMedioFila() << std::endl;

    std::cout << "\nComparativo E(T) amostral simulado x E(T) analítico: " << this->retornaTempoMedioSistema() << " " << tempoMedioPeriodoOcupadoAnalitico << std::endl;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    double tempoMedioPeriodoOcupadoGeneralizadoAnalitico = numClientes * tempoMedioPeriodoOcupadoAnalitico;

    std::cout << "\nComparativo E(B_C) amostral simulado (Período ocupado generalizado) x C * E(T) analítico " << this->retornaPeriodoOcupadoGeneralizadoMedio() << " " << tempoMedioPeriodoOcupadoGeneralizadoAnalitico << std::endl;
    std::cout << "Comparativo E(U_C) simulado vs E(U_C) analítico: " << tempoMedioUmCliente <<
                 " " << numClientes * tempoMedioPeriodoOcupadoAnalitico - tempoMedioPeriodoOcupadoAnalitico << std::endl;

    #endif

    std::cout << "\nVariância amostral de processos no sistema (E(N)): " << this->retornaVarianciaProcessosSistema() << std::endl;
    std::cout << "Variância amostral de processos na fila (E(N_q)): " << this->retornaVarianciaFilaSistema() << std::endl;     
    std::cout << "Variância amostral de tempo no sistema (E(T)): " << this->retornaVarianciaTempoSistema() << std::endl; 
    std::cout << "Variância amostral de tempo na fila (E(W)): " << this->retornaVarianciaTempoFila() << std::endl;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    std::cout << "Variância amostral do período ocupado generalizado (E(B_C)): " << this->retornaVarianciaPeriodoOcupadoGeneralizadoMedio() << std::endl;
    std::cout << "Variância amostral do tempo médio até atingir um cliente (E(U_C)): " <<
                    this->retornaVarianciaTempoMedioUmCliente() << std::endl;

    #endif

    std::cout << "\nDesvio padrão amostral de processos no sistema (E(N)): " << this->retornaDesvioPadraoProcessosSistema() << std::endl;
    std::cout << "Desvio padrão amostral de processos na fila (E(N_q)): " << this->retornaDesvioPadraoFilaSistema() << std::endl;     
    std::cout << "Desvio padrão amostral de tempo no sistema (E(T)): " << this->retornaDesvioPadraoTempoSistema() << std::endl; 
    std::cout << "Desvio padrão amostral de tempo na fila (E(W)): " << this->retornaDesvioPadraoTempoFila() << std::endl;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    std::cout << "Desvio padrão amostral do período ocupado generalizado (E(B_C)): " << this->retornaDesvioPadraoPeriodoOcupadoGeneralizadoMedio() << std::endl;
    std::cout << "Desvio padrão amostral do tempo médio até atingir um cliente (E(U_C)): " <<
                    this->retornaDesvioPadraoTempoMedioUmCliente() << std::endl;

    #endif

    auto intervalo = this->retornaIntervaloConfiancaProcessosSistema();

    std::cout << "\nIntervalo de confiança de processos no sistema (E(N)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;

    intervalo = this->retornaIntervaloConfiancaFilaSistema();

    std::cout << "Intervalo de confiança de processos na fila (E(N_q)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;     

    intervalo = this->retornaIntervaloConfiancaTempoSistema();

    std::cout << "Intervalo de confiança de tempo no sistema (E(T)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl; 

    intervalo = this->retornaIntervaloConfiancaTempoFila();

    std::cout << "Intervalo de confiança de tempo na fila (E(W)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    intervalo = this->retornaIntervaloConfiancaPeriodoOcupadoGeneralizadoMedio();

    std::cout << "Intervalo de confiança de período ocupado generalizado (E(B_C)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;

    intervalo = this->retornaIntervaloConfiancaTempoMedioUmCliente();

    std::cout << "Intervalo de confiança de tempo médio até atingir um cliente (E(U_C)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;

    #endif
}

std::vector<Metricas> Estatisticas::retornaMetricas() const
{
    return this->amostras;
}