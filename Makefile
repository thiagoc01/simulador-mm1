ifdef NUM_THREADS
	AUX = $(NUM_THREADS)
	override NUM_THREADS2 = -D NUM_THREADS=$(AUX)
endif

ifdef TAMANHO_AMOSTRA
	AUX2 = $(TAMANHO_AMOSTRA)
	override TAMANHO_AMOSTRA2 = -D TAMANHO_AMOSTRA=$(AUX2)
endif

ifdef PERIODO_OCUPADO_GENERALIZADO
	DEFINE_PERIODO = -D CALCULAR_PERIODO_OCUPADO_GENERALIZADO
endif

ifeq ($(OS),Windows_NT)
	versaoPythonLista = $(subst ., ,$(filter-out Python , $(shell python --version)))

	ifneq ($(word 2, $(versaoPythonLista)),)

	versaoPython = $(word 1, $(versaoPythonLista)).$(word 2, $(versaoPythonLista))

	else

	versaoPython = $(word 1, $(versaoPythonLista))

	endif

	versaoPython2 = $(subst .,,$(versaoPython))
	appDataRoaming = $(shell echo ${APPDATA})
	appData = $(subst Roaming,\,$(appDataRoaming))
	INCLUDEDIR1 = include/
	INCLUDEDIR2 = $(appData)\Local\Programs\Python\Python$(versaoPython2)\include
	INCLUDEDIR3 = "$(appData)\Local\Programs\Python\Python$(versaoPython2)\Lib\site-packages\numpy\core\include"
	LIB = "$(appData)\Local\Programs\Python\Python$(versaoPython2)\libs"
	FLAGS = -I $(INCLUDEDIR1) -I "$(INCLUDEDIR2)" -I $(INCLUDEDIR3) -L $(LIB) $(NUM_THREADS2) $(TAMANHO_AMOSTRA2) -std=c++2a -lpython$(versaoPython2) -lpthread -w $(DEFINE_PERIODO)

	ifeq ("$(wildcard $(INCLUDEDIR2))", "")
		INCLUDEDIR2 = "$(appData)\Local\Programs\Python\Python$(versaoPython)\include"
		INCLUDEDIR3 = "$(appData)\Local\Programs\Python\Python$(versaoPython)\Lib\site-packages\numpy\core\include"
		LIB = "$(appData)\Local\Programs\Python\Python$(versaoPython)\libs"
		FLAGS = -I $(INCLUDEDIR1) -I $(INCLUDEDIR2) -I $(INCLUDEDIR3) -L $(LIB) $(NUM_THREADS2) $(TAMANHO_AMOSTRA2) -std=c++2a -lpython$(versaoPython) -lpthread -w $(DEFINE_PERIODO)
	endif
		
else
	ifeq ($(shell uname -s), Linux)

		versaoPythonLista = $(subst ., ,$(filter-out Python , $(shell python3 --version)))

		ifneq ($(word 2, $(versaoPythonLista)),)

		versaoPython = $(word 1, $(versaoPythonLista)).$(word 2, $(versaoPythonLista))

		else

		versaoPython = $(word 1, $(versaoPythonLista))

		endif

		INCLUDEDIR1 = include/
		INCLUDEDIR2 = /usr/include/python$(versaoPython)
		FLAGS = -I $(INCLUDEDIR1) -I $(INCLUDEDIR2) $(NUM_THREADS2) $(TAMANHO_AMOSTRA2) -std=c++2a -lpython$(versaoPython) -lpthread -w $(DEFINE_PERIODO) -g

	else

		versaoPythonLista = $(subst ., ,$(filter-out Python , $(shell python3 --version)))

		ifneq ($(word 2, $(versaoPythonLista)),)

		versaoPython = $(word 1, $(versaoPythonLista)).$(word 2, $(versaoPythonLista))

		else

		versaoPython = $(word 1, $(versaoPythonLista))

		endif

		INCLUDEDIR1 = include/
		INCLUDEDIR2 = /usr/local/Cellar/python/$(versaoPython)/Frameworks/Python.framework/Versions/$(versaoPython)/include/python$(versaoPython)
		INCLUDEDIR3 = /usr/local/lib/python$(versaoPython)/site-packages/numpy/core/include
		LIB = /usr/local/Cellar/python/$(versaoPython)/Frameworks/Python.framework/Versions/$(versaoPython)/lib
		FLAGS = -I $(INCLUDEDIR1) -I $(INCLUDEDIR2) -I $(INCLUDEDIR3) -L $(LIB) $(NUM_THREADS2) $(TAMANHO_AMOSTRA2) -std=c++2a -lpython$(versaoPython) -lpthread -w $(DEFINE_PERIODO)
		
	endif


endif

OBJ1 = simulador1_mm1.o thread.o grafico.o geracao_tempos.o estatisticas.o metricas.o
OBJ2 = simulador2_mm1.o grafico.o thread.o requisicao.o geracao_tempos.o estatisticas.o metricas.o
OBJ3 = simulador3_mm1.o thread.o grafico.o evento.o requisicao.o geracao_tempos.o estatisticas.o metricas.o
TARGET = simulador

VERSAO_G++ = $(shell g++ -dumpversion)
VERSAO_G++_LISTA = $(subst ., ,$(VERSAO_G++))
TESTE_G++ =$(shell test $(word 1, $(VERSAO_G++_LISTA)) -ge 10; echo $$?;)

.PHONY : teste_g++
	
teste_g++:

ifeq ($(TESTE_G++),0)
	$(eval CXX = g++)

else
	$(info "Necessário g++ na versão 10 ou superior.")
	exit 1
endif

simulador1 : teste_g++ thread.o grafico.o estatisticas geracao_aleatorios simulador1_compilacao_objs simulador1_compilacao clean
	
simulador1_compilacao: main.cpp
	$(CXX) $(OBJ1) main.cpp -o $(TARGET) $(FLAGS)

simulador1_compilacao_objs: simulador1/*.cpp
	$(CXX) -c simulador1/simulador1_mm1.cpp $(FLAGS)

simulador2 : teste_g++ thread.o grafico2 estatisticas2 geracao_aleatorios simulador2_compilacao_objs simulador2_compilacao clean
	
simulador2_compilacao: main.cpp
	$(CXX) $(OBJ2) main.cpp -o $(TARGET) $(FLAGS) -D SIMULADOR_2

simulador2_compilacao_objs: simulador2/*.cpp
	$(CXX) -c simulador2/simulador2_mm1.cpp $(FLAGS) -D SIMULADOR_2
	$(CXX) -c simulador2/requisicao.cpp $(FLAGS) -D SIMULADOR_2

simulador3 : teste_g++ thread.o grafico.o estatisticas geracao_aleatorios simulador3_compilacao_objs simulador3_compilacao clean
	
simulador3_compilacao: main.cpp
	$(CXX) $(OBJ3) main.cpp -o $(TARGET) $(FLAGS)

simulador3_compilacao_objs: simulador3/*.cpp
	$(CXX) -c simulador3/evento.cpp $(FLAGS)
	$(CXX) -c simulador3/requisicao.cpp $(FLAGS)
	$(CXX) -c simulador3/simulador3_mm1.cpp $(FLAGS)
	
geracao_aleatorios: geradores_aleatorios/geracao_tempos.cpp
	$(CXX) -c geradores_aleatorios/geracao_tempos.cpp $(FLAGS)

estatisticas: estatistica/*.cpp
	$(CXX) -c estatistica/*.cpp $(FLAGS)

estatisticas2: estatistica/*.cpp
	$(CXX) -c estatistica/*.cpp $(FLAGS) -D SIMULADOR_2

thread.o: thread/thread.cpp
	$(CXX) -c thread/thread.cpp $(FLAGS)

grafico.o: grafico/grafico.cpp
	$(CXX) -c grafico/grafico.cpp $(FLAGS)

grafico2: grafico/grafico.cpp
	$(CXX) -c grafico/grafico.cpp $(FLAGS) -D SIMULADOR_2

clean:
	rm -f *.o
