// ParallelProcessor.h
//
// Author        : John Pazarzis
// Creation date : Tue 29 Oct 2013 01:46:26 AM EDT
//
// Summary
//
//      Allows parallel execution of a function implemented as a class member.
//      Used when we have a container of objects and we want to call the same
//      function to all of them asynchronously (meaning each run does not depend
//      to the others.
//
//      We provide the specific type as a template parameter and the function to
//      call and the number of threads to create in the constructor.
//
//      We call the add method to add the objects to call. 
//
//      We start the parallel processing by calling go. This will block the
//      calling thread until all the processing is completed.
//
//      A complete example of it use can be found under [root]/samples/cpp-samples
//
// Notes
//

#ifndef PARALLELPROCESSOR_INCLUDED
#define PARALLELPROCESSOR_INCLUDED

#include <stdio.h>      
#include <stdlib.h>     
#include <memory>
#include <vector>
#include <assert.h>
#include <mutex>
#include <condition_variable>
#include <thread> 
#include <deque>
using namespace std;

template <typename T>
class ParallelProcessor final
{
    public:

        ParallelProcessor(function<void(T*)> processor_function, int number_of_threads = 1):
            _processor_function(processor_function),
            _number_of_threads(number_of_threads) 
        {
        }

        ParallelProcessor(const ParallelProcessor& other) = delete;

        ParallelProcessor& operator=(const ParallelProcessor& other) = delete; 

        void go()
        {
                unique_lock<mutex> locker(_mutex, defer_lock);     
                locker.lock();
                std::vector<thread> threads;
                for(int i = 0; i <_number_of_threads; ++i)
                    threads.push_back(thread(bind(&ParallelProcessor::calculator,this)));
                locker.unlock();
                for(int i = 0; i <_number_of_threads; ++i)
                    threads[i].join();
        }

        void add(T* object){
            _to_be_processed.push_front(object);
        }

        private:

                void calculator() {
                    for(;;){
                        unique_lock<mutex> locker(_mutex, defer_lock);     
                        locker.lock();
                        if(_to_be_processed.empty())
                            break;
                        T* pT = _to_be_processed.back();
                        _to_be_processed.pop_back();
                        locker.unlock();
                        function<void(void)> processor = bind(_processor_function, pT);
                        processor();
                    }
                }

                std::mutex _mutex;
                std::deque<T*> _to_be_processed;
                function<void(T*)> _processor_function;
                int _number_of_threads;
};




#endif // PARALLELPROCESSOR_INCLUDED
