CXX=g++
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
FLAGS=-g -D DEBUG -w -ggdb3 -std=c++0x -pthread
#FLAGS=-Ofast -ggdb3 -std=c++0x -pthread -w

INCLUDE=-I$(DEVELOPMENT_ROOT)/include -I/usr/include/python2.7

gen_pro_sample : gen_pro_sample.cpp ComparableValue.h Scalar.h Aggregate.h
	$(CXX) $(FLAGS)  $(INCLUDE) -o gen_pro_sample gen_pro_sample.cpp 
