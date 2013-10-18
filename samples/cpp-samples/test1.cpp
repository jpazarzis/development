#include <iostream>
#include <deque>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <condition_variable>
using namespace std;

mutex mu;
deque<int> d;
condition_variable cond;


void producer() {
    unique_lock<mutex> locker(mu, defer_lock); 
    for(int i = 0; i < 9; ++i) {
        locker.lock();
        d.push_front(i);
        locker.unlock();
        cond.notify_one();
    }
}

void consumer() {
    int v = -1;
    unique_lock<mutex> locker(mu, defer_lock); 
    while(v!=8){
        locker.lock();
        cond.wait(locker, [](){return !d.empty();});
        v = d.back();
        d.pop_back();
        cout << v << endl;
        locker.unlock();
    }
}

int main(){
    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
}

