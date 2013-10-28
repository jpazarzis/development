#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <mutex>
#include <thread>         // std::thread
#include "Pool.h"

using namespace std;


mutex _print_mutex;


class A
{
public:
    int i,j;
};

Pool<A,100> block_of_A;

void show(const std::string& s){
    unique_lock<mutex> locker(_print_mutex); 
    cout << s << endl;
}

void foo(){
    for(int i = 0; i < 99; ++i)
    {
        A* p = block_of_A.get();
        p->i = i;
        p->j = i*2;
        char buffer[1024];
        sprintf(buffer,"i: %d  available: %d", p->i,  block_of_A.size());
        show(buffer);
        block_of_A.put_back(p);

    }
}


int main(){
        vector<thread> threads;

        for(int i = 0; i <40; ++i)
            threads.push_back(thread(foo));


        for(auto& t: threads)
        {
            t.join();
        }
}

