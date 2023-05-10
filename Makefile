CXX = g++
INCLUDEDIR = includes/
FLAGS = -I $(INCLUDEDIR)
OBJ1 = simulador1_mm1.o geracao_tempos.o
OBJ2 = simulador2.mm1.o
OBJ3 = simulador3_mm1.o evento.o requisicao.o geracao_tempos.o
TARGET = simulador

simulador1 : simulador1_compilacao_objs simulador1_compilacao clean
	
simulador1_compilacao: simulador1/main.cpp
	$(CXX) $(OBJ1) simulador1/main.cpp -o $(TARGET) $(FLAGS)

simulador1_compilacao_objs: simulador1/*.cpp geradores_aleatorios/geracao_tempos.cpp
	$(CXX) -c simulador1/simulador1_mm1.cpp $(FLAGS)
	$(CXX) -c geradores_aleatorios/geracao_tempos.cpp $(FLAGS)

simulador3 : simulador3_compilacao_objs simulador3_compilacao clean
	
simulador3_compilacao: simulador3/main.cpp
	$(CXX) $(OBJ3) simulador3/main.cpp -o $(TARGET) $(FLAGS)

simulador3_compilacao_objs: simulador3/*.cpp geradores_aleatorios/geracao_tempos.cpp
	$(CXX) -c simulador3/evento.cpp $(FLAGS)
	$(CXX) -c simulador3/requisicao.cpp $(FLAGS)
	$(CXX) -c simulador3/simulador3_mm1.cpp $(FLAGS)
	$(CXX) -c geradores_aleatorios/geracao_tempos.cpp $(FLAGS)

clean:
	rm -f *.o
