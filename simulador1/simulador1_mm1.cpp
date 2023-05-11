#include <iostream>
#include <random>
#include <vector>

#include "geracao_tempos.hpp"

static int numProcessosSistema = 0;
static int numProcessosFila = 0;

static std::vector<double> temposChegadaAcumulativo; // Guarda os tempos de chegada de cada requisição
static std::vector<double> temposSaida; // Guarda os tempos de saída de cada requisição
static std::vector<double> temposEspera; // Guarda os tempos de espera de cada requisição
static std::vector<double> temposSistema; // Guarda os tempos no sistema de cada requisição
static std::vector<std::pair<double, int>> numerosProcessosFilaPeriodo; // Guarda o número de processos no sistema em um intervalo de tempo
static std::vector<std::pair<double, int>> numerosProcessosSistemaPeriodo; // Guarda o número de processos na fila em um intervalo de tempo

/* Ponteiros de função para a geração de tempos de serviço de forma determínistica ou probabilística */

static double (*geraTempoServicoDeterministico)(double mediaServico) = [](double mediaServico){return mediaServico; };
static double (*geraTempoServicoProbabilistico)(double mediaServico) = [](double mediaServico){return retornaTempoExponencial(mediaServico); };

static double tempoSimulacao = 0.0; // Tempo do simulador (snapshots de eventos)
static double ultimoEvento = 0.0; // Ocorrência da última chegada ou saída
static double tempoSistemaRespostaMedia; // Armazena E(T)
static double tempoSistemaEsperaMedia; // Armazena E(W)
static double numProcessosSistemaTotal = 0.0; // Somatório do número de processos no sistema
static double numProcessosFilaTotal = 0.0; // Somatório do número de processos na fila

/* Função que faz a inicialização das variáveis utilizadas*/
void reiniciaVariaveis()
{
    numProcessosSistema = 0;
    numProcessosFila = 0;

    temposChegadaAcumulativo.clear();
    temposSaida.clear(); 
    temposEspera.clear();
    temposSistema.clear();
    numerosProcessosFilaPeriodo.clear();
    numerosProcessosSistemaPeriodo.clear();

    tempoSimulacao = 0.0;
    ultimoEvento = 0.0;
    tempoSistemaRespostaMedia;
    tempoSistemaEsperaMedia;
    numProcessosSistemaTotal = 0.0;
    numProcessosFilaTotal = 0.0;
}

void calculaMetricas()
{
    /*
        Realiza o cálculo análogo a esperança. Multiplica cada número de processos 
        pela taxa relativa de tempo que esse número ocorreu no sistema.
        A divisão pelo tempo total gera o que seria a probabilidade da definição da esperança.
    */

    for (const auto& numeroProcessosPeriodo : numerosProcessosSistemaPeriodo)
    {
        auto periodo = numeroProcessosPeriodo.first;
        auto numeroProcessos = numeroProcessosPeriodo.second;
        numProcessosSistemaTotal += periodo * numeroProcessos;
    }

    /* Análogo ao anterior, mas para processos na fila */

    for (const auto& numeroProcessosPeriodo : numerosProcessosFilaPeriodo)
    {
        auto periodo = numeroProcessosPeriodo.first;
        auto numeroProcessos = numeroProcessosPeriodo.second;
        numProcessosFilaTotal += periodo * numeroProcessos;
    }

    if (temposSistema.empty()) // Se nenhuma requisição saiu do sistema, o tempo médio é 0.
        tempoSistemaRespostaMedia = 0.0;
    else
        tempoSistemaRespostaMedia = std::accumulate(temposSistema.begin(), temposSistema.end(), 0.0) / temposSistema.size();

    if (temposEspera.empty()) // Análogo ao anterior.
        tempoSistemaEsperaMedia = 0.0;
    else
        tempoSistemaEsperaMedia = std::accumulate(temposEspera.begin(), temposEspera.end(), 0.0) / temposEspera.size();

    std::cout << "\n\nNúmero médio de processos no sistema (E(N)): " << numProcessosSistemaTotal / tempoSimulacao << std::endl;
    std::cout << "Número médio de processos na fila (E(N_q)): " << numProcessosFilaTotal / tempoSimulacao << std::endl;
    std::cout << "Tempo médio no sistema (E(T)): " <<  tempoSistemaRespostaMedia << std::endl;
    std::cout << "Tempo médio na fila (E(W)): " <<  tempoSistemaEsperaMedia << std::endl;
}

/* Função que recebe os tempos de chegada e de serviço e atualiza esses tempos */
void atualizaSistema(const double& tempoChegada, const double& tempoSaida)
{
    /* Quando o sistema está vazio ou o tempo que o processo leva para chegar é menor do que o de serviço, o processo entra na fila de espera e é atendido */
    if (numProcessosSistema == 0 || tempoChegada < tempoSaida)
    {
        /* O tempo de chegada do processo é somado ao tempo total passado */
        tempoSimulacao += tempoChegada;
        
        /* Guarda o tempo da chegada */
        temposChegadaAcumulativo.push_back(tempoSimulacao);

        /* Guarda os números de processos respectivos e o período que ocorreram */
        numerosProcessosSistemaPeriodo.push_back({tempoSimulacao - ultimoEvento, numProcessosSistema});
        numerosProcessosFilaPeriodo.push_back({tempoSimulacao - ultimoEvento, numProcessosSistema});
        
        /* Instante que o evento chega */
        ultimoEvento = tempoSimulacao;
        
        /* O processo entra na fila */
        numProcessosFila++;
        
        /* Imprimindo o tempo de simulação, chegada e o número de processos atuais no sistema */
        std::cout << "Tempo: " << tempoSimulacao << " - CHEGADA " << numProcessosSistema << "->" << numProcessosSistema + 1 << std::endl;
        numProcessosSistema++;
    }
    
    else
    {
        /* A requisição antes de ser atendida estava na fila. Guarda o período de tempo e a quantidade antes do atendimento */
        numerosProcessosFilaPeriodo.push_back({tempoSimulacao - ultimoEvento, numProcessosFila});
        numProcessosFila--;

        /* Nos dois casos a seguir, acessamos o vetor de tempos de chegada utilizando o vetor de tempos de saída.
           A i-ésima requisição a sair foi a i-ésima a chegar por ser um sistema com um servidor.
           Logo, esse acesso obtém o tempo respectivo. */

        /* Guarda o tempo de espera antes do atendimento */
        temposEspera.push_back(tempoSimulacao - temposChegadaAcumulativo[temposSaida.size()]);
        
         /* O tempo de saída do processo é somado ao tempo total passado */
        tempoSimulacao += tempoSaida;

        /* Guarda o tempo de saída e o tempo de resposta no sistema. */
        temposSaida.push_back(tempoSaida);
        temposSistema.push_back(tempoSimulacao - temposChegadaAcumulativo[temposSaida.size() - 1]);

         /* Durante o serviço ocorreu a quantidade anterior menos um */
        numerosProcessosFilaPeriodo.push_back({tempoSaida, numProcessosFila});
        numerosProcessosSistemaPeriodo.push_back({tempoSimulacao - ultimoEvento, numProcessosSistema});
        
        ultimoEvento = tempoSimulacao;
        
        /* Imprimindo o tempo de simulação, saída e o número de processos atuais no sistema */
        std::cout << "Tempo: " << tempoSimulacao << " - SAÍDA " << numProcessosSistema << "->" << numProcessosSistema - 1 << std::endl;

        numProcessosSistema--;
    }
}

/* Função que recebe o λ, o μ e o número de iterações do simulador e roda o simulador do tipo M/M/1 */
void simulaFilaMM1(int numIteracoes, double mediaChegada, double mediaServico, bool eDeterminismo)
{
    double (*geraTempoServico)(double mediaServico);

    /* Inicializa as variáveis */
    reiniciaVariaveis();

    if (eDeterminismo)
        geraTempoServico = geraTempoServicoDeterministico;
    
    else
        geraTempoServico = geraTempoServicoProbabilistico;
    
    /* Loop que roda o simulador até o número de iterações desejado */
    for (int i = 0 ; i < numIteracoes ; i++)
    {
        /* Gera um tempo de chegada com distribuição de Poisson e um tempo de serviço pela distribuição Exponencial */
        double tempoChegada = retornaTempoPoisson(1.0 / mediaChegada);
        double tempoSaida = geraTempoServico(1.0 / mediaServico);
        
        /* Faz a atualização dos tempos para a próxima iteração */
        atualizaSistema(tempoChegada, tempoSaida);     
    }
    
    /* Função que calcula o número de processos na fila, no sistema e a o tempo médio na fila e no sistema */
    calculaMetricas();  
}
