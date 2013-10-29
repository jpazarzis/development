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
#include <time.h>
#include "Pool.h"
#include "ParallelProcessor.h"
using namespace std;

mutex _print_mutex;

void show(const std::string& s){
    unique_lock<mutex> locker(_print_mutex); 
    cout << s << endl;
}


class A
{
    int i,j, sum;
public:

    A()
    {
        i = rand() % 10;
        j = rand() % 10;
        sum = -9999;
    }

    void randomize()
    {
        i = rand() % 10;
        j = rand() % 10;
        sum = -9999;
    }

    A(const A& other) = delete;
    A& operator=(const A& other) = delete;

    string to_string() const
    {
        char buffer[1024];
        sprintf(buffer,"i:%d j:%d sum:%d ", i, j, sum);
        return buffer;
    }

    void calculate()
    {
        sum = i+j;
    }
};

int main(){

        // Nice bug!!
        // Since Poll is using a static pool, dispite the fact is using rand
        // to initialize variables, it will always have the same values since
        // it is initialized BEFORE the srand!
        srand ( time(NULL) );
        ParallelProcessor<A> pp(&A::calculate,2);
        Pool<A,1000> pool;
        vector<A*> v;
        for(int i = 0; i < 300; ++i){
            v.push_back(pool.get());    
        }
    
        for(int i = 0; i < 300; ++i){
            v[i]->randomize();
            pp.add(v[i]);
        }

        pp.go();
        for(int i = 0; i < 300; ++i)
        {
            cout << v[i]->to_string() << endl;
        }
}

