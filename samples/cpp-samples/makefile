CXX=g++
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
FLAGS=-g -D DEBUG -std=c++0x -pthread  
#FLAGS=-Ofast

INCLUDE=-I$(DEVELOPMENT_ROOT)/include -I/usr/include
VPATH = $(DEVELOPMENT_ROOT)/include
#LIBS = -L/usr/lib -lboost_date_time-mt

FLAGS=-Ofast -ggdb3 -std=c++0x -pthread  
SRC_PATH=$(DEVELOPMENT_ROOT)/src/


ga_sample1:ga_sample1.cpp Optimizable.h GeneticAlgorithm.h std_include.h Object.h Logger.o toolkit.o CloneableDouble.o 
	$(CXX) $(FLAGS) $(INCLUDE) -o ga_sample1 ga_sample1.cpp CloneableDouble.o Logger.o toolkit.o

CloneableDouble.o:$(SRC_PATH)CloneableDouble.cpp CloneableDouble.h std_include.h Logger.h Object.h toolkit.o
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)CloneableDouble.cpp toolkit.o

Logger.o: Logger.h $(SRC_PATH)Logger.cpp
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)Logger.cpp

toolkit.o: toolkit.h $(SRC_PATH)toolkit.cpp
	$(CXX) $(FLAGS) $(INCLUDE) -c $(SRC_PATH)toolkit.cpp


pool_sample:pool_sample.cpp Pool.h
	$(CXX) $(FLAGS) $(INCLUDE) -o pool_sample pool_sample.cpp

multi_threaded_pool_consumer:multi_threaded_pool_consumer.cpp Pool.h
	$(CXX) $(FLAGS) $(INCLUDE) -o multi_threaded_pool_consumer multi_threaded_pool_consumer.cpp

parallel_processor:parallel_processor.cpp Pool.h ParallelProcessor.h
	$(CXX) $(FLAGS) $(INCLUDE) -o parallel_processor parallel_processor.cpp

funtional_use:funtional_use.cpp 
	$(CXX) $(FLAGS) $(INCLUDE) -o funtional_use funtional_use.cpp


test_time:test_time.cpp 
	$(CXX) $(FLAGS) -o test_time test_time.cpp  

test_construction_boost_date:test_construction_boost_date.cpp toolkit.h
	$(CXX) $(FLAGS) $(INCLUDE) -o test_construction_boost_date test_construction_boost_date.cpp  


test1: test1.cpp
	$(CXX) $(FLAGS) -o test1 test1.cpp

test2: test2.cpp
	$(CXX) $(FLAGS) -o test2 test2.cpp

convert_int_to_bin_array_sample: convert_int_to_bin_array_sample.cpp
	$(CXX) $(FLAGS) -o convert_int_to_bin_array_sample convert_int_to_bin_array_sample.cpp



xmlsample: xmlsample.cpp xmldocument.h toolkit.h
	$(CXX) $(FLAGS) $(INCLUDE) -o $@ $^

DateTime.o:DateTime.cpp DateTime.h
	$(CXX) $(FLAGS) $(INCLUDE) -c $<
