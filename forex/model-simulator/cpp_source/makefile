CXX=g++ 
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
#FLAGS=-g -D DEBUG -ggdb3 -std=c++0x -pthread  
FLAGS=-Ofast -ggdb3 -std=c++0x -pthread  

INCLUDE=-I$(DEVELOPMENT_ROOT)/include
VPATH = $(DEVELOPMENT_ROOT)/include
SRC_PATH=$(DEVELOPMENT_ROOT)/src/

model_optimizer:model_optimizer.cpp SellBasedInDelta.h Optimizable.h GeneticAlgorithm.h  std_include.h TickEngine.h TickProcessor.h Object.h Order.o Identifiable.o CloneableDouble.o  Logger.o toolkit.o Model.h FitnessCalculators.h xmldocument.h toolkit.o FitnessStatistics.h ParallelProcessor.h TickPool.o 
	$(CXX) $(FLAGS)  $(INCLUDE) -o model_optimizer model_optimizer.cpp CloneableDouble.o Logger.o Order.o Identifiable.o toolkit.o TickPool.o

CloneableDouble.o:$(SRC_PATH)CloneableDouble.cpp CloneableDouble.h std_include.h Logger.h Object.h toolkit.o
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)CloneableDouble.cpp toolkit.o

Logger.o: Logger.h $(SRC_PATH)Logger.cpp
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)Logger.cpp

Order.o: Order.cpp Order.h TickEngine.h TickProcessor.h Identifiable.h Object.h TickPool.o Pool.h toolkit.h
	$(CXX) $(FLAGS) $(INCLUDE) -c Order.cpp  

Identifiable.o: Identifiable.h $(SRC_PATH)Identifiable.cpp
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)Identifiable.cpp

toolkit.o: toolkit.h $(SRC_PATH)toolkit.cpp
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)toolkit.cpp

TickPool.o: TickPool.cpp TickPool.h std_include.h Tick.h toolkit.h
	$(CXX) $(FLAGS) $(INCLUDE) -c TickPool.cpp


clean:
	rm *.o
	rm model_optimizer
	rm ga_sample1
    
