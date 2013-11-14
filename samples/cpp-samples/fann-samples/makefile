CXX=g++
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
#FLAGS=-g -D DEBUG -std=c++0x -pthread  
FLAGS=-Ofast

INCLUDE=-I$(DEVELOPMENT_ROOT)/include -I/usr/include
VPATH = $(DEVELOPMENT_ROOT)/include
#LIBS = -L/usr/lib -lboost_date_time-mt

FLAGS=-Ofast -ggdb3 -std=c++0x -pthread  
SRC_PATH=$(DEVELOPMENT_ROOT)/src/

fann_test: fann_test.cpp
	$(CXX) $(FLAGS) -o fann_test fann_test.cpp   -lfann -lm

back_test: back_test.cpp
	$(CXX) $(FLAGS) -o back_test back_test.cpp   -lfann -lm


simple_execution: simple_execution.cpp
	$(CXX) $(FLAGS) -o simple_execution simple_execution.cpp   -lfann -lm

