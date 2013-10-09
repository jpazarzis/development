CXX=g++
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
FLAGS=-g -D DEBUG 
#FLAGS=-Ofast

main:main.cpp CloneableDouble.o Optimizable.h GeneticAlgorithm.h Statistics.h std_include.h
	$(CXX) $(FLAGS) -o main main.cpp CloneableDouble.o

CloneableDouble.o:CloneableDouble.cpp CloneableDouble.h std_include.h
	$(CXX) $(FLAGS) -c CloneableDouble.cpp






    
