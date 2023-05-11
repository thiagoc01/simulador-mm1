CXX = g++
INCLUDEDIR = includes/
FLAGS = -I $(INCLUDEDIR) -std=c++2a
OBJ1 = simulador1_mm1.o thread.o geracao_tempos.o estatisticas.o metricas.o
OBJ2 = simulador2_mm1.o thread.o requisicao.o geracao_tempos.o estatisticas.o metricas.o
OBJ3 = simulador3_mm1.o evento.o requisicao.o geracao_tempos.o estatisticas.o metricas.o
TARGET = simulador

simulador1 : thread estatisticas geracao_aleatorios simulador1_compilacao_objs simulador1_compilacao clean
	
simulador1_compilacao: main.cpp
	$(CXX) $(OBJ1) main.cpp -o $(TARGET) $(FLAGS)

simulador1_compilacao_objs: simulador1/*.cpp
	$(CXX) -c simulador1/simulador1_mm1.cpp $(FLAGS)

simulador2 : thread estatisticas geracao_aleatorios simulador2_compilacao_objs simulador2_compilacao clean
	
simulador2_compilacao: main.cpp
	$(CXX) $(OBJ2) main.cpp -o $(TARGET) $(FLAGS)

simulador2_compilacao_objs: simulador2/*.cpp
	$(CXX) -c simulador2/simulador2_mm1.cpp $(FLAGS)
	$(CXX) -c simulador2/requisicao.cpp $(FLAGS)

simulador3 : estatisticas geracao_aleatorios simulador3_compilacao_objs simulador3_compilacao clean
	
simulador3_compilacao: simulador3/main.cpp
	$(CXX) $(OBJ3) simulador3/main.cpp -o $(TARGET) $(FLAGS)

simulador3_compilacao_objs: simulador3/*.cpp
	$(CXX) -c simulador3/evento.cpp $(FLAGS)
	$(CXX) -c simulador3/requisicao.cpp $(FLAGS)
	$(CXX) -c simulador3/simulador3_mm1.cpp $(FLAGS)
	
geracao_aleatorios: geradores_aleatorios/geracao_tempos.cpp
	$(CXX) -c geradores_aleatorios/geracao_tempos.cpp $(FLAGS)

estatisticas: estatistica/*.cpp
	$(CXX) -c estatistica/*.cpp $(FLAGS)

thread: thread/thread.cpp
	$(CXX) -c thread/thread.cpp $(FLAGS)

clean:
	rm -f *.o
