#include "estatistica/estatisticas.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

/*
    Todos os cálculos pra número de clientes e tempos de sistema são análogos aos demais, mas com n-dimensões.
    Por serem muitos dados, não salvamos na amostra, e sim calculamos a cada iteração.
    Utilizamos o fato de que E(sum_i X_i) = sum_i E(X_i), onde X_i é a variável de uma amostra i.
*/

/* Coloca mais uma amostra para análise. */

void Estatisticas::adicionaAmostra(const Metricas& amostra)
{
    this->amostras.push_back(amostra);
}

/* Aplica a fórmula da variância para cada métrica de cada amostra. */

void Estatisticas::calculaMediaAmostralTempoSistemaQtdProcessos(const struct ContagensTemposSistema& quantidades, const bool& eUmaIteracao)
{
    if (eUmaIteracao) // Calculando uma iteração, nós devemos acumular os valores apenas. Aplica E(X) e E(X^2) pra cada ocorrência
    {
        for (const auto& temposProcessos : quantidades.contagensTemposSistema)
        {
            this->mediaTemposNumerosProcessos.estatisticaTemposSistema[temposProcessos.first] +=
                    (temposProcessos.second / static_cast<double>(quantidades.totalOcorrenciasTemposSistema));

            this->esperancaQuadradoTemposNumerosProcessos.estatisticaTemposSistema[temposProcessos.first] += ( (temposProcessos.second / static_cast<double>(quantidades.totalOcorrenciasTemposSistema))
                                * (temposProcessos.second / static_cast<double>(quantidades.totalOcorrenciasTemposSistema)) );
        }

        for (const auto& numerosProcessos : quantidades.contagemTemposQtdProcessos)
        {
            this->mediaTemposNumerosProcessos.estatisticaNumeroProcessos[numerosProcessos.first] +=
                    (numerosProcessos.second / quantidades.tempoSimulacao);

            this->esperancaQuadradoTemposNumerosProcessos.estatisticaNumeroProcessos[numerosProcessos.first] += ( (numerosProcessos.second / static_cast<double>(quantidades.tempoSimulacao))
                    * (numerosProcessos.second / quantidades.tempoSimulacao) );
        }
    }

    else // Calcula a CDF e também a média
    {
        /* Prepara o vetor do eixo X */

        for (const auto& temposProcessos : this->mediaTemposNumerosProcessos.estatisticaTemposSistema)
            this->temposSistema.push_back(temposProcessos.first);

        for (const auto& numerosProcessos : this->mediaTemposNumerosProcessos.estatisticaNumeroProcessos)
            this->numerosProcessos.push_back(numerosProcessos.first);

        std::sort(this->temposSistema.begin(), this->temposSistema.end()); // Faz os sorts pra organizar de forma crescente
        std::sort(this->numerosProcessos.begin(), this->numerosProcessos.end());

        /* Prepara o vetor do eixo Y com os valores normalizados e também as médias */

        for (const auto& tempos : this->temposSistema)
        {
            this->mediaTemposNumerosProcessos.estatisticaTemposSistema[tempos] /= this->amostras.size();

            this->densidadeTempos.push_back(this->mediaTemposNumerosProcessos.estatisticaTemposSistema.at(tempos));

            this->esperancaQuadradoTemposNumerosProcessos.estatisticaTemposSistema[tempos] /= this->amostras.size();
        }

        for (const auto& numeros : this->numerosProcessos)
        {
            this->mediaTemposNumerosProcessos.estatisticaNumeroProcessos[numeros] /= this->amostras.size();

            this->densidadeNumeros.push_back(this->mediaTemposNumerosProcessos.estatisticaNumeroProcessos.at(numeros));

            this->esperancaQuadradoTemposNumerosProcessos.estatisticaNumeroProcessos[numeros] /= this->amostras.size();
        }
    }    
}

void Estatisticas::calculaVarianciaDesvioPadraoTemposNumerosProcessos(const struct EstatisticaTemposSistema& esperancaQuadradoTemposNumerosProcessos)
{
    for (const auto& tempos : this->temposSistema)
    {
        this->varianciasTemposNumerosProcessos.estatisticaTemposSistema[tempos] = 
                    esperancaQuadradoTemposNumerosProcessos.estatisticaTemposSistema.at(tempos) - 
                    (this->mediaTemposNumerosProcessos.estatisticaTemposSistema.at(tempos) 
                    * this->mediaTemposNumerosProcessos.estatisticaTemposSistema.at(tempos));

        this->desviosPadroesTemposNumerosProcessos.estatisticaTemposSistema[tempos] = 
                    std::sqrt(this->varianciasTemposNumerosProcessos.estatisticaTemposSistema.at(tempos));
    }

    for (const auto& numeros : this->numerosProcessos)
    {
        this->varianciasTemposNumerosProcessos.estatisticaNumeroProcessos[numeros] = 
                    esperancaQuadradoTemposNumerosProcessos.estatisticaNumeroProcessos.at(numeros) - 
                    (this->mediaTemposNumerosProcessos.estatisticaNumeroProcessos.at(numeros) 
                    * this->mediaTemposNumerosProcessos.estatisticaNumeroProcessos.at(numeros));

        this->desviosPadroesTemposNumerosProcessos.estatisticaNumeroProcessos[numeros] = 
                    std::sqrt(this->varianciasTemposNumerosProcessos.estatisticaNumeroProcessos.at(numeros));
    }

}

void Estatisticas::calculaVarianciasDesviosPadroesAmostrais()
{
    /* Equivalem a E(X linha) */
    
    this->numeroMedioProcessosSistema = 0.0;
    this->numeroMedioFilaSistema = 0.0;
    this->tempoMedioSistema = 0.0;
    this->tempoMedioFila = 0.0;
    this->probabilidadeMediaExtincao = 0.0;


    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO

    this->periodoOcupadoGeneralizadoMedio = 0.0;
    this->tempoMedioUmCliente = 0.0;

    #endif

    /* Equivalem a E(X linha ^ 2) */

    double esperancaQuadradoProcessosSistema = 0.0;
    double esperancaQuadradoFilaSistema = 0.0;
    double esperancaQuadradoTempoMedioSistema = 0.0;
    double esperancaQuadradoTempoMedioFila = 0.0;
    double esperancaQuadradoProbabilidadeExtincao = 0.0;
    

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
        this->probabilidadeMediaExtincao += amostra.retornaProbabilidadeExtincao();

        #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
        this->periodoOcupadoGeneralizadoMedio += amostra.retornaPeriodoOcupadoGeneralizadoMedio();
        this->tempoMedioUmCliente += amostra.retornaTempoMedioUmCliente();

        #endif
        
        esperancaQuadradoProcessosSistema += (amostra.retornaNumeroMedioProcessosSistema() * amostra.retornaNumeroMedioProcessosSistema());
        esperancaQuadradoFilaSistema += (amostra.retornaNumeroMedioFilaSistema() * amostra.retornaNumeroMedioFilaSistema());
        esperancaQuadradoTempoMedioSistema += (amostra.retornaTempoMedioSistema() * amostra.retornaTempoMedioSistema());
        esperancaQuadradoTempoMedioFila += (amostra.retornaTempoMedioFila() * amostra.retornaTempoMedioFila());
        esperancaQuadradoProbabilidadeExtincao += (amostra.retornaProbabilidadeExtincao() * amostra.retornaProbabilidadeExtincao());


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
    this->probabilidadeMediaExtincao /= this->amostras.size();
    
    esperancaQuadradoProcessosSistema /= this->amostras.size();
    esperancaQuadradoFilaSistema /= this->amostras.size();
    esperancaQuadradoTempoMedioSistema /= this->amostras.size();
    esperancaQuadradoTempoMedioFila /= this->amostras.size();
    esperancaQuadradoProbabilidadeExtincao /= this->amostras.size();

    struct ContagensTemposSistema anon;

    calculaMediaAmostralTempoSistemaQtdProcessos(anon, false);

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
    this->varianciaProbabilidadeExtincao = esperancaQuadradoProbabilidadeExtincao - (this->probabilidadeMediaExtincao * this->probabilidadeMediaExtincao);


    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    this->varianciaPeriodoOcupadoGeneralizadoMedio = esperancaQuadradoPeriodoOcupadoGeneralizadoMedio - (this->periodoOcupadoGeneralizadoMedio * this->periodoOcupadoGeneralizadoMedio);
    this->varianciaTempoMedioUmCliente = esperancaQuadradoTempoMedioUmCliente - (this->tempoMedioUmCliente * this->tempoMedioUmCliente);
    
    #endif

    this->desvioPadraoProcessosSistema = std::sqrt(this->varianciaProcessosSistema);
    this->desvioPadraoFilaSistema = std::sqrt(this->varianciaFilaSistema);
    this->desvioPadraoTempoSistema = std::sqrt(this->varianciaTempoSistema);
    this->desvioPadraoTempoFila = std::sqrt(this->varianciaTempoFila);
    this->desvioPadraoProbabilidadeExtincao = std::sqrt(this->varianciaProbabilidadeExtincao);

    calculaVarianciaDesvioPadraoTemposNumerosProcessos(esperancaQuadradoTemposNumerosProcessos);

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

static void calculaLimitesTemposNumerosProcessos(std::unordered_map<std::string, struct EstatisticaTemposSistema>& intervalosConfianca,
                            const double& probabilidadeTaxaConfianca,
                            const struct EstatisticaTemposSistema& valoresMedios,
                            const struct EstatisticaTemposSistema& desviosPadroes,
                            const int& tamanhoAmostral)
{

    intervalosConfianca.insert({"inferior", EstatisticaTemposSistema()});
    intervalosConfianca.insert({"superior", EstatisticaTemposSistema()});

    for (const auto& tempos : valoresMedios.estatisticaTemposSistema)
    {
        double limiteInferior, limiteSuperior;

        limiteInferior = valoresMedios.estatisticaTemposSistema.at(tempos.first) - probabilidadeTaxaConfianca
                        * (desviosPadroes.estatisticaTemposSistema.at(tempos.first) / std::sqrt(tamanhoAmostral));

        limiteSuperior = valoresMedios.estatisticaTemposSistema.at(tempos.first) + probabilidadeTaxaConfianca
                        * (desviosPadroes.estatisticaTemposSistema.at(tempos.first) / std::sqrt(tamanhoAmostral));

        intervalosConfianca["inferior"].estatisticaTemposSistema[tempos.first] = limiteInferior;
        intervalosConfianca["superior"].estatisticaTemposSistema[tempos.first] = limiteSuperior;
    }

    for (const auto& numeros : valoresMedios.estatisticaNumeroProcessos)
    {
        double limiteInferior, limiteSuperior;

        limiteInferior = valoresMedios.estatisticaNumeroProcessos.at(numeros.first) - probabilidadeTaxaConfianca
                        * (desviosPadroes.estatisticaNumeroProcessos.at(numeros.first) / std::sqrt(tamanhoAmostral));

        limiteSuperior = valoresMedios.estatisticaNumeroProcessos.at(numeros.first) + probabilidadeTaxaConfianca
                        * (desviosPadroes.estatisticaNumeroProcessos.at(numeros.first) / std::sqrt(tamanhoAmostral));

        intervalosConfianca["inferior"].estatisticaNumeroProcessos[numeros.first] = limiteInferior;
        intervalosConfianca["superior"].estatisticaNumeroProcessos[numeros.first] = limiteSuperior;
    }
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

    calculaLimitesTemposNumerosProcessos(this->intervalosConfiancasTemposNumerosProcessos, probabilidadeTaxaConfianca,
                    this->mediaTemposNumerosProcessos, this->desviosPadroesTemposNumerosProcessos, tamanhoAmostral);

    calculaLimites(this->intervaloConfiancaProbabilidadeExtincao, probabilidadeTaxaConfianca, this->probabilidadeMediaExtincao,
                    this->desvioPadraoProbabilidadeExtincao, tamanhoAmostral);

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

struct EstatisticaTemposSistema Estatisticas::retornaMediasTemposNumerosProcessos() const
{
    return this->mediaTemposNumerosProcessos;
}

double Estatisticas::retornaProbabilidadeMediaExtincao() const
{
    return this->probabilidadeMediaExtincao;
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

struct EstatisticaTemposSistema Estatisticas::retornaVarianciasTemposNumerosProcessos() const
{
    return this->varianciasTemposNumerosProcessos;
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

struct EstatisticaTemposSistema Estatisticas::retornaDesviosPadroesTemposNumerosProcessos() const
{
    return this->desviosPadroesTemposNumerosProcessos;
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

std::unordered_map<std::string, struct EstatisticaTemposSistema> Estatisticas::retornaIntervalosConfiancasTemposNumerosProcessos() const
{
    return this->intervalosConfiancasTemposNumerosProcessos;
}

const std::vector<double>& Estatisticas::retornaTemposSistema() const
{
    return this->temposSistema;
}

const std::vector<double>& Estatisticas::retornaNumerosProcessos() const
{
    return this->numerosProcessos;
}

const std::vector<double>& Estatisticas::retornaDensidadeTempos() const
{
    return this->densidadeTempos;
}

const std::vector<double>& Estatisticas::retornaDensidadeNumeros() const
{
    return this->densidadeNumeros;
}

const std::vector<double>& Estatisticas::retornaDensidadeTemposInferior() const
{
    return this->densidadeTemposInferior;
}

const std::vector<double>& Estatisticas::retornaDensidadeNumerosInferior() const
{
    return this->densidadeNumerosInferior;
}

const std::vector<double>& Estatisticas::retornaDensidadeTemposSuperior() const
{
    return this->densidadeTemposSuperior;
}

const std::vector<double>& Estatisticas::retornaDensidadeNumerosSuperior() const
{
    return this->densidadeNumerosSuperior;
}

void Estatisticas::imprimeDadosTemposNumsProcessos(const struct EstatisticaTemposSistema& dados, const std::string& tipoDado)
{
    for (const auto& num : this->numerosProcessos)
        std::cout << tipoDado << " amostral da probabilidade de " << num << " processos: " << dados.estatisticaNumeroProcessos.at(num) << std::endl;
        

    std::cout << std::endl;
}

void Estatisticas::imprimeIntervalosConfiancaTemposNumsProcessos(const std::unordered_map<std::string, struct EstatisticaTemposSistema>& dados)
{
    for (const auto& num : this->temposSistema)
    {
        this->densidadeTemposInferior.push_back(dados.at("inferior").estatisticaTemposSistema.at(num));
        this->densidadeTemposSuperior.push_back(dados.at("superior").estatisticaTemposSistema.at(num));
    }

    for (const auto& num : this->numerosProcessos)
    {
        std::cout << "Intervalo de confiança da probabilidade de " << num << " processos na fila: [" <<
        dados.at("inferior").estatisticaNumeroProcessos.at(num) << ", " << dados.at("superior").estatisticaNumeroProcessos.at(num) << "]" << std::endl;
        this->densidadeNumerosInferior.push_back(dados.at("inferior").estatisticaNumeroProcessos.at(num));
        this->densidadeNumerosSuperior.push_back(dados.at("superior").estatisticaNumeroProcessos.at(num));
    }

    std::cout << std::endl;
}

void Estatisticas::imprimeAnaliseAmostral(const double& taxaChegada, const double& taxaServico)
{
    extern int numClientes;
    double tempoMedioPeriodoOcupadoAnalitico = (1 / taxaServico) / (1 - (taxaChegada / taxaServico));

    std::cout << "Média amostral de processos no sistema (E(N)): " << this->retornaNumeroMedioProcessosSistema() << std::endl;
    std::cout << "Média amostral de processos na fila (E(N_q)): " << this->retornaNumeroMedioFilaSistema() << std::endl;     
    std::cout << "Média amostral de tempo no sistema (E(T)): " << this->retornaTempoMedioSistema() << std::endl; 
    std::cout << "Média amostral de tempo na fila (E(W)): " << this->retornaTempoMedioFila() << std::endl;
    std::cout << "Probabilidade média extinção: " << probabilidadeMediaExtincao << std::endl;

    imprimeDadosTemposNumsProcessos(this->mediaTemposNumerosProcessos, "Média");

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
    std::cout << "Variância amostral da probabilidade de extinção: " << this->varianciaProbabilidadeExtincao << std::endl;

    imprimeDadosTemposNumsProcessos(this->varianciasTemposNumerosProcessos, "Variância");

    #ifdef CALCULAR_PERIODO_OCUPADO_GENERALIZADO
    
    std::cout << "Variância amostral do período ocupado generalizado (E(B_C)): " << this->retornaVarianciaPeriodoOcupadoGeneralizadoMedio() << std::endl;
    std::cout << "Variância amostral do tempo médio até atingir um cliente (E(U_C)): " <<
                    this->retornaVarianciaTempoMedioUmCliente() << std::endl;

    #endif

    std::cout << "\nDesvio padrão amostral de processos no sistema (E(N)): " << this->retornaDesvioPadraoProcessosSistema() << std::endl;
    std::cout << "Desvio padrão amostral de processos na fila (E(N_q)): " << this->retornaDesvioPadraoFilaSistema() << std::endl;     
    std::cout << "Desvio padrão amostral de tempo no sistema (E(T)): " << this->retornaDesvioPadraoTempoSistema() << std::endl; 
    std::cout << "Desvio padrão amostral de tempo na fila (E(W)): " << this->retornaDesvioPadraoTempoFila() << std::endl;
    std::cout << "Desvio padrão amostral da probabilidade de extinção: " << this->desvioPadraoProbabilidadeExtincao << std::endl;

    imprimeDadosTemposNumsProcessos( this->desviosPadroesTemposNumerosProcessos, "Desvio padrão");

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

    intervalo = this->intervaloConfiancaProbabilidadeExtincao;

    std::cout << "Intervalo de confiança da probabilidade de extinção: [" << intervalo["inferior"] << ", " << intervalo["superior"]<< "]" << std::endl;

    imprimeIntervalosConfiancaTemposNumsProcessos(this->intervalosConfiancasTemposNumerosProcessos);

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