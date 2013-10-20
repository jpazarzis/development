CXX=g++
#FLAGS=-g -D DEBUG
FLAGS=-Ofast

multicap_reader: utilities.o Starter.o multicap_reader.cpp
	$(CXX) $(FLAGS) -o multicap_reader multicap_reader.cpp utilities.o Starter.o

utilities.o: utilities.cpp utilities.h
	$(CXX) $(FLAGS) -c utilities.cpp

Starter.o: Starter.cpp Starter.h
	$(CXX) $(FLAGS) -c Starter.cpp

clean:
	rm *.o

    
