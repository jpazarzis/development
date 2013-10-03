CXX=g++
#FLAGS=-g -D DEBUG
FLAGS=-Ofast


ga_user_sample: ga_user_sample.cpp ga.h
	$(CXX) $(FLAGS) -o ga_user_sample ga_user_sample.cpp


test_cloning: test_cloning.cpp cloner.h
	$(CXX) $(FLAGS) -o test_cloning test_cloning.cpp

main:main.cpp Models.h TickEngine.h TickProcessor.h Order.o 
	$(CXX) $(FLAGS) -o main main.cpp Order.o


Order.o: Order.cpp Order.h TickEngine.h TickProcessor.h
	$(CXX) $(FLAGS) -c Order.cpp


    
