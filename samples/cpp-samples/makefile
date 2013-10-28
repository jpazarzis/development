CXX=g++
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
FLAGS=-g -D DEBUG -std=c++0x -pthread  
#FLAGS=-Ofast

INCLUDE=-I$(DEVELOPMENT_ROOT)/include -I/usr/include
VPATH = $(DEVELOPMENT_ROOT)/include
#LIBS = -L/usr/lib -lboost_date_time-mt

pool_sample:pool_sample.cpp Pool.h
	$(CXX) $(FLAGS) $(INCLUDE) -o pool_sample pool_sample.cpp


test_time:test_time.cpp 
	$(CXX) $(FLAGS) -o test_time test_time.cpp  

test_construction_boost_date:test_construction_boost_date.cpp toolkit.h
	$(CXX) $(FLAGS) $(INCLUDE) -o test_construction_boost_date test_construction_boost_date.cpp  


test1: test1.cpp
	$(CXX) $(FLAGS) -o test1 test1.cpp

xmlsample: xmlsample.cpp xmldocument.h toolkit.h
	$(CXX) $(FLAGS) $(INCLUDE) -o $@ $^

DateTime.o:DateTime.cpp DateTime.h
	$(CXX) $(FLAGS) $(INCLUDE) -c $<

clean:
	rm xmlsample
	rm test_time



    
