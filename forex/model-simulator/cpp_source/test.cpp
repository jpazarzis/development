#include "std_include.h"
#include "Statistics.h"
#include <numeric>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
using namespace std;


void test_drawdown()
{
        vector<double> v;
        v.push_back(100000);
        v.push_back(99287);
        v.push_back(99353);
        v.push_back(99418);
        v.push_back(99485);
        v.push_back(99551);
        v.push_back(99617);
        v.push_back(99684);
        v.push_back(99750);
        v.push_back(99816);
        v.push_back(99109);
        v.push_back(99174);
        v.push_back(99240);
        v.push_back(99307);
        v.push_back(99372);
        v.push_back(99439);
        v.push_back(99504);
        v.push_back(99569);
        v.push_back(99635);
        v.push_back(98929);
        v.push_back(99000);
        v.push_back(99065);
        v.push_back(99131);
        cout <<max_drawdown(v) << endl;
        
}

void task1(string msg)
{
    cout << "task1 says: " << msg << endl;
}

void test_thread()
{

}

int main()
{

     //Starting the thread and running the function at the same time as we
    //continue running the rest the function (main()) we are already in.
    thread t1(task1, "Hello");

    //Forces the original thread and the new thread we split off to come back 
    //and merge together. What this means is that our default program thread
    //waits continually until the new thread (ti) finishes executing.
    t1.join();
}




