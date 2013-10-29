#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <mutex>
#include <condition_variable>
#include <thread> 
#include <deque>
#include "Pool.h"
using namespace std;

mutex _print_mutex;
mutex mu;

void show(const std::string& s){
    unique_lock<mutex> locker(_print_mutex); 
    cout << s << endl;
}



class A
{
    int i,j, sum, _thread_id;
public:

    A(): i (rand() % 10), j (rand() % 10), sum(-99999)
    { 
    }

    string to_string() const
    {
        char buffer[1024];
        sprintf(buffer,"i:%d j:%d sum:%d threadid: %d", i, j, sum, _thread_id);
        return buffer;
    }

    void calculate(int thread_id)
    {
        sum = i+j;
        _thread_id = thread_id;
    }
};


deque<A*> to_be_processed;

Pool<A,100> pool;


void calculator(int thread_id) {
    for(;;){
        unique_lock<mutex> locker(mu, defer_lock);     
        locker.lock();
        if(to_be_processed.empty())
            break;
        A* pa = to_be_processed.back();
        to_be_processed.pop_back();
        locker.unlock();
        char buffer[1024];
        sprintf(buffer,"working thread: %d", thread_id);
        show(buffer);
        pa->calculate(thread_id);
        std::chrono::milliseconds dura( 20 );
        std::this_thread::sleep_for( dura );
    }
}

#define NUMBER_OF_THREADS 3
#define NUMBER_OF_OBJECTS 20 


int main(){
        srand ( time(NULL) );

        vector<A*> the_objects;
        for(int i = 0; i < NUMBER_OF_OBJECTS; ++i)
            the_objects.push_back(pool.get());

        for(int i = 0; i < NUMBER_OF_OBJECTS; ++i)
            to_be_processed.push_front(the_objects[i]);

        unique_lock<mutex> locker(mu, defer_lock);     
        locker.lock();
        vector<thread> threads;
        for(int i = 0; i <NUMBER_OF_THREADS; ++i){
            threads.push_back(thread(calculator, i+1));
        }
        locker.unlock();
        for(int i = 0; i <NUMBER_OF_THREADS; ++i){
            threads[i].join();
        }
        for(auto x: the_objects)
            cout << x->to_string() << endl;
}

