#include <iostream>
#include <deque>
#include <thread>         
#include <mutex>          
#include <condition_variable>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
using namespace std;

mutex mu;
deque<int> d;
condition_variable cond;
int _is_done = 0;

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

#define LIMIT 180

void producer(int id) {
    unique_lock<mutex> locker(mu, defer_lock); 
    for(int i = 0; i < LIMIT + 1; ++i) {
        locker.lock();
        cout << "producer: " << id << endl;
        d.push_front(i);
        if(i == LIMIT)
            ++_is_done;
        locker.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));    
        cond.notify_all();
    }
}

void consumer(int id) {
    int v = -1;
    unique_lock<mutex> locker(mu, defer_lock); 
    while(v!=LIMIT){
        locker.lock();
        cond.wait(locker, [](){return !d.empty() || _is_done == 2;});
        if(_is_done == 2 && d.empty())
        {
            break;
        }
        v = d.back();
        d.pop_back();
        cout << "thread: " << id << " " << v << endl;
        locker.unlock();
    }
}

int main(){
    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;

    thread t1(producer,1);
    thread t11(producer,2);

    thread t2(consumer,3);
    thread t3(consumer,4);
    thread t4(consumer,5);

    t1.join();
    t11.join();

    t2.join();
    t3.join();
    t4.join();

    std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
}


