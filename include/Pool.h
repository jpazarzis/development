// Pool.h
//
// Author        : John Pazarzis
// Creation date : Mon 28 Oct 2013 05:27:41 PM EDT
//
// Summary
//
//      Used to create a pool of reusable objects stored in a 
//      static array to avoid calling new and delete. Useful when we can
//      approximate the maximum number of objects we might need.
//
//      Call get to get the next available object which is returned as a pointer
//      and when done with its use call put_back to return it to the pool
//
//      A full example can be seen here:
//      [root]/samples/cpp-samples/pool_sample.cpp
//

#ifndef POOL_INCLUDED
#define POOL_INCLUDED

#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <mutex>
#include <thread>         
using namespace std;

template<typename T, int SIZE>
class Pool final
{
            static T _pool[SIZE];
            std::vector<T*> _available;
            mutex _block_mutex;

            void initialize(){
                _available.clear();
                for(register int i = 0; i < SIZE; ++i)
                    _available.push_back(&_pool[i]);
            }

    public:
            Pool() { 
                initialize();
            }

            ~Pool() {}

            Pool(const Pool& other) = delete;

            Pool& operator=(const Pool& other) = delete;

            int size() {
                unique_lock<mutex> locker(_block_mutex);                 
                return _available.size();
            }

            T* get(){
                unique_lock<mutex> locker(_block_mutex);                 
                T* pT = NULL;
                if(_available.size() > 0) {
                    pT =_available.back();
                    _available.pop_back();
                }
                return pT;
            }

            void put_back(T* t){
                 if(t != NULL) {
                    unique_lock<mutex> locker(_block_mutex); 
                    _available.push_back(t);
                 }
            }
};

template <class T, int SIZE> T Pool<T, SIZE>::_pool[SIZE];

#endif // POOL_INCLUDED
