CXX=g++ 
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
FLAGS=-g -D DEBUG -std=c++0x -pthread  
#FLAGS=-g -w -D DEBUG  -ggdb3 -std=c++0x -pthread  
#FLAGS=-Ofast -ggdb3 -std=c++0x -pthread  -w

INCLUDE=-I$(DEVELOPMENT_ROOT)/include -I/usr/include/python2.7

VPATH = $(DEVELOPMENT_ROOT)/include

SRC_PATH=$(DEVELOPMENT_ROOT)/src/

OBJ=CloneableDouble.o Logger.o Order.o Identifiable.o toolkit.o TickPool.o

PYTHON_LIB=-L/usr/lib/python2.7 -lpython2.7

HEADERS=SellBasedInDelta.h Optimizable.h GeneticAlgorithm.h  std_include.h \
        TickEngine.h TickProcessor.h Object.h Model.h FitnessCalculators.h \
        xmldocument.h FitnessStatistics.h ParallelProcessor.h PythonSignalCaller.h 


TEST_FREE_MARGIN_DEPENDENCIES=test_free_margin.cpp Optimizable.h GeneticAlgorithm.h  std_include.h \
                         TickProcessor.h CloneableDouble.o Identifiable.o  \
                         Logger.o toolkit.o Model.h FitnessCalculators.h xmldocument.h toolkit.o FitnessStatistics.h \
                         FreeMarginModel.h FreeMarginExecutor.h Position.o ParallelProcessor.h TickPool.o 

test_candle_sticks:test_candle_sticks.cpp CandleStick.h CandleStickCollection.h
	$(CXX) $(FLAGS)  $(INCLUDE) -o test_candle_sticks test_candle_sticks.cpp $(OBJ)

test_free_margin:$(TEST_FREE_MARGIN_DEPENDENCIES) $(OBJ)
	$(CXX) $(FLAGS)  $(INCLUDE) -o test_free_margin test_free_margin.cpp  Position.o $(OBJ)

bftd:bftd.cpp xmldocument.h
	$(CXX) $(FLAGS)  $(INCLUDE) -o bftd bftd.cpp 

python_optimizer:python_optimizer.cpp $(HEADERS) PythonDrivenModel.h $(OBJ)
	$(CXX) $(FLAGS)  $(INCLUDE) -o python_optimizer python_optimizer.cpp $(OBJ) $(PYTHON_LIB)


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

Position.o: Position.cpp Position.h TickPool.h
	$(CXX) $(FLAGS) $(INCLUDE) -c Position.cpp
