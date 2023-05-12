#include "../includes/estatistica/estatisticas.hpp"

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

    /* Equivalem a E(X linha ^ 2) */

    double esperancaQuadradoProcessosSistema = 0.0;
    double esperancaQuadradoFilaSistema = 0.0;
    double esperancaQuadradoTempoMedioSistema = 0.0;
    double esperancaQuadradoTempoMedioFila = 0.0;

    /* Realiza o somatório do numerador de E(X) e E(X^2) */

    for (const auto& amostra : this->amostras)
    {
        this->numeroMedioProcessosSistema += amostra.retornaNumeroMedioProcessosSistema();
        this->numeroMedioFilaSistema += amostra.retornaNumeroMedioFilaSistema();
        this->tempoMedioSistema += amostra.retornaTempoMedioSistema();
        this->tempoMedioFila += amostra.retornaTempoMedioFila();
        
        esperancaQuadradoProcessosSistema += (amostra.retornaNumeroMedioProcessosSistema() * amostra.retornaNumeroMedioProcessosSistema());
        esperancaQuadradoFilaSistema += (amostra.retornaNumeroMedioFilaSistema() * amostra.retornaNumeroMedioFilaSistema());
        esperancaQuadradoTempoMedioSistema += (amostra.retornaTempoMedioSistema() * amostra.retornaTempoMedioSistema());
        esperancaQuadradoTempoMedioFila += (amostra.retornaTempoMedioFila() * amostra.retornaTempoMedioFila());
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

    /* Calcula a variância e desvio padrão de acordo com a definição. */

    this->varianciaProcessosSistema = esperancaQuadradoProcessosSistema - (this->numeroMedioProcessosSistema * this->numeroMedioProcessosSistema);
    this->varianciaFilaSistema = esperancaQuadradoFilaSistema - (this->numeroMedioFilaSistema * this->numeroMedioFilaSistema);
    this->varianciaTempoSistema = esperancaQuadradoTempoMedioSistema - (this->tempoMedioSistema * this->tempoMedioSistema);
    this->varianciaTempoFila = esperancaQuadradoTempoMedioFila - (this->tempoMedioFila * this->tempoMedioFila);

    this->desvioPadraoProcessosSistema = std::sqrt(this->varianciaProcessosSistema);
    this->desvioPadraoFilaSistema = std::sqrt(this->varianciaFilaSistema);
    this->desvioPadraoTempoSistema = std::sqrt(this->varianciaTempoSistema);
    this->desvioPadraoTempoFila = std::sqrt(this->varianciaTempoFila);
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

void Estatisticas::imprimeAnaliseAmostral()
{
    std::cout << "Média amostral de processos no sistema (E(N)): " << this->retornaNumeroMedioProcessosSistema() << std::endl;
    std::cout << "Média amostral de processos na fila (E(N_q)): " << this->retornaNumeroMedioFilaSistema() << std::endl;     
    std::cout << "Média amostral de tempo no sistema (E(T)): " << this->retornaTempoMedioSistema() << std::endl; 
    std::cout << "Média amostral de tempo na fila (E(W)): " << this->retornaTempoMedioFila() << std::endl;

    std::cout << "\nVariância amostral de processos no sistema (E(N)): " << this->retornaVarianciaProcessosSistema() << std::endl;
    std::cout << "Variância amostral de processos na fila (E(N_q)): " << this->retornaVarianciaFilaSistema() << std::endl;     
    std::cout << "Variância amostral de tempo no sistema (E(T)): " << this->retornaVarianciaTempoSistema() << std::endl; 
    std::cout << "Variância amostral de tempo na fila (E(W)): " << this->retornaVarianciaTempoFila() << std::endl;

    std::cout << "\nDesvio padrão amostral de processos no sistema (E(N)): " << this->retornaDesvioPadraoProcessosSistema() << std::endl;
    std::cout << "Desvio padrão amostral de processos na fila (E(N_q)): " << this->retornaDesvioPadraoFilaSistema() << std::endl;     
    std::cout << "Desvio padrão amostral de tempo no sistema (E(T)): " << this->retornaDesvioPadraoTempoSistema() << std::endl; 
    std::cout << "Desvio padrão amostral de tempo na fila (E(W)): " << this->retornaDesvioPadraoTempoFila() << std::endl;

    auto intervalo = this->retornaIntervaloConfiancaProcessosSistema();

    std::cout << "\nIntervalo de confiança de processos no sistema (E(N)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;

    intervalo = this->retornaIntervaloConfiancaFilaSistema();

    std::cout << "Intervalo de confiança de processos na fila (E(N_q)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;     

    intervalo = this->retornaIntervaloConfiancaTempoSistema();

    std::cout << "Intervalo de confiança de tempo no sistema (E(T)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl; 

    intervalo = this->retornaIntervaloConfiancaTempoFila();

    std::cout << "Intervalo de confiança de tempo na fila (E(W)): [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;
}

std::vector<Metricas> Estatisticas::retornaMetricas() const
{
    return this->amostras;
}