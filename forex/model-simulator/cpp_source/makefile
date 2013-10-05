CXX=g++
#FLAGS=-g -D DEBUG -DDISABLE_LOGGING
FLAGS=-g -D DEBUG 
#FLAGS=-Ofast

sample_optimizer: sample_optimizer.cpp ga.h Models.h TickEngine.h TickProcessor.h Order.o BuyAtSpecificMinuteModel.h Logger.o
	$(CXX) $(FLAGS) -o sample_optimizer sample_optimizer.cpp Order.o Logger.o

ga_user_sample: ga_user_sample.cpp ga.h
	$(CXX) $(FLAGS) -o ga_user_sample ga_user_sample.cpp


main:main.cpp Models.h TickEngine.h TickProcessor.h Order.o 
	$(CXX) $(FLAGS) -o main main.cpp Order.o


Order.o: Order.cpp Order.h TickEngine.h TickProcessor.h
	$(CXX) $(FLAGS) -c Order.cpp

test1: test1.cpp 
	$(CXX) $(FLAGS) -o test1 test1.cpp 

Logger.o: Logger.h Logger.cpp
	$(CXX) $(FLAGS) -c Logger.cpp

clean:
	rm *.o



    
