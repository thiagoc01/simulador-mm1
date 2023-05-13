# Simuladores de fila MM1 | Trabalho 1 | Modelagem e Avaliação de Desempenho 2023.1

<br>

Esse repositório contém o código de três simuladores que simulam uma fila MM1 de três formas diferentes. O objetivo é mostrar que eles são equivalentes.

<br>

# Dependências

- Cabeçalho de threads POSIX
- Python 3 e Python3-dev
- Matplotlib
- G++ versão 10 ou superior (compatibilidade com C++20)

<br>

# Como funciona?

Cada simulador recebe três parâmetros: a taxa de chegada $\lambda$, a taxa de serviço $\mu$ e o número de iterações/requisições (simulador 3) desejada. E um parâmetro opcional D, que indica se o tempo de serviço é determínistico ou não. O simulador é rodado n vezes, onde n é o tamanho da amostra. Em cada iteração, são gerados os tempos de chegada e serviço de um processo, ambos via distribuição exponencial com médias $\frac{1}{\lambda}$ e $\frac{1}{\mu}$, respectivamente. Ao fim, são exibidas estatísticas da amostra e gráficos para quatro métricas:

- E($N$): Número médio de processos no sistema;
- E($N_q$): Número médio de processos na fila;
- E($T$): Tempo médio de processos no sistema;
- E($W$): Tempo médio de processos na fila;

Se o tamanho da amostra for 1, as estatísticas não são exibidas, porém os eventos de uma execução que serão, assim como as métricas daquela execução.

<br>

# Como utilizar?

Clone o repositório:

```shell
$ git clone https://github.com/thiagoc01/simulador-mm1.git
```

Compile o simulador desejado:

```shell
$ make simulador<simulador desejado> [TAMANHO_AMOSTRA=<Número natural>] [NUM_THREADS=<Número natural>]
```

Em \<simulador desejado\>, digite 1, 2 ou 3.
Você pode escolher o tamanho da amostra com a variável opcional TAMANHO_AMOSTRA, que tem tamanho máximo permitido de 50000. Assim como o número de threads com NUM_THREADS. Por padrão, o tamanho da amostra é 10000 e o número de threads é 5. 

Exemplo de compilação:

```shell
$ make simulador1 TAMANHO_AMOSTRA=50000 NUM_THREADS=8
```

Irá compilar o simulador 1 com tamanho de amostra 50000 e número de threads igual a 8.

Para executar qualquer um dos simuladores, digite:

```bash
$ ./simulador <número de iterações/requisições> <lambda> <mi> [D]
```

Onde D é um parâmetro opcional para indicar que os serviços são determinísticos. Lambda e mi são as taxas de chegada e serviço.

<br>

# Instalação de dependências

Se você estiver em um ambiente Linux, certifique-se de que tenha instalado as dependências:

```bash
$ sudo apt install python3
$ sudo apt install python3-dev
$ sudo apt install pip
$ pip install matplotlib
$ sudo apt install g++
```

Nesse exemplo, o gerenciador de pacotes é o apt.

Em um ambiente Windows, tenha o make instalado via chocolatey, o g++ e o python na pasta padrão. Lembre-se de adicioná-los ao PATH.




