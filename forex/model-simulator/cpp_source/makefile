CXX=g++ 
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
#FLAGS=-g -D DEBUG -ggdb3 -std=c++0x -pthread  
FLAGS=-Ofast -ggdb3 -std=c++0x -pthread  

INCLUDE=-I$(DEVELOPMENT_ROOT)/include
VPATH = $(DEVELOPMENT_ROOT)/include


model_optimizer:model_optimizer.cpp SellBasedInDelta.h Optimizable.h GeneticAlgorithm.h Statistics.h std_include.h TickEngine.h TickProcessor.h Object.h Order.o Identifiable.o CloneableDouble.o  Logger.o Utilities.o Model.h FitnessCalculators.h xmldocument.h toolkit.h TickPool.o
	$(CXX) $(FLAGS)  $(INCLUDE) -o model_optimizer model_optimizer.cpp CloneableDouble.o Logger.o Order.o Identifiable.o Utilities.o TickPool.o

ga_sample1:ga_sample1.cpp CloneableDouble.o Optimizable.h GeneticAlgorithm.h Statistics.h std_include.h Object.h Logger.o Utilities.o
	$(CXX) $(FLAGS) -o ga_sample1 ga_sample1.cpp CloneableDouble.o Logger.o Utilities.o

CloneableDouble.o:CloneableDouble.cpp CloneableDouble.h std_include.h Logger.h Object.h
	$(CXX) $(FLAGS) -c CloneableDouble.cpp

Logger.o: Logger.h Logger.cpp
	$(CXX) $(FLAGS) -c Logger.cpp

Order.o: Order.cpp Order.h TickEngine.h TickProcessor.h Identifiable.h Object.h
	$(CXX) $(FLAGS) -c Order.cpp

Identifiable.o: Identifiable.h Identifiable.cpp
	$(CXX) $(FLAGS) -c Identifiable.cpp

Utilities.o: Utilities.h Utilities.cpp
	$(CXX) $(FLAGS) -c Utilities.cpp

TickPool.o: TickPool.cpp TickPool.h std_include.h Tick.h
	$(CXX) $(FLAGS) -c TickPool.cpp

test1: test.cpp Statistics.h
	$(CXX) $(FLAGS) -o test1 test.cpp


    
