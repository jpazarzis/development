# location of the Python header files
 
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
#EXTENTION = C
EXTENTION = cpp

INCLUDE=-I$(DEVELOPMENT_ROOT)/include -I/usr/include
VPATH = $(DEVELOPMENT_ROOT)/include


FLAGS=-g -D DEBUG -std=c++0x -pthread  
 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib
 
# compile mesh classes
#TARGET = hello_ext

TARGET = World

multithread_python_calling:multithread_python_calling.cpp
	g++ $(FLAGS) $(INCLUDE) -o   multithread_python_calling multithread_python_calling.cpp -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython2.7

calling_process_tick:calling_process_tick.cpp
	g++ $(FLAGS) $(INCLUDE) -o   calling_process_tick calling_process_tick.cpp -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython2.7

call_python:call_python.cpp
	g++ $(FLAGS) -o call_python call_python.cpp -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython2.7


vanila_call_python:vanila_call_python.cpp
	g++ $(FLAGS) -o vanila_call_python vanila_call_python.cpp -I/usr/include/python2.7 -L/usr/lib/python2.7 -lpython2.7



 
$(TARGET).so: $(TARGET).o
	g++ -shared -Wl,--export-dynamic $(TARGET).o -L$(BOOST_LIB) -lboost_python-py27 -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o $(TARGET).so
 
$(TARGET).o: $(TARGET).$(EXTENTION)
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(TARGET).$(EXTENTION)
