CXX=g++
#FLAGS=-g -D DEBUG
FLAGS=-Ofast

main:main.cpp TickEngine.h TickProcessor.h Order.o
	$(CXX) $(FLAGS) -o main main.cpp Order.o


Order.o: Order.cpp Order.h TickEngine.h TickProcessor.h
	$(CXX) $(FLAGS) -c Order.cpp


    
