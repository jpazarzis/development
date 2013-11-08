// #include "poolable.h"
#ifndef POOL_INCLUDED
#define POOL_INCLUDED
#include <string>
template <class T, int SIZE> class poolable {
    public:
        virtual ~ poolable()
        {}

        typedef   T* T_ptr;

        inline static T_ptr get()
        {
            if(!_initialized) {
                initialize();
            }

            return _stack_index >= 0 ? _stack[_stack_index--] : 0;
        }

        inline static void put_back(T_ptr p)
        {
            if(0 != p && _stack_index < SIZE - 1) {
                _stack[_stack_index++] = p;
            }
        }

        static std::string status()
        {
            char buffer[1024];
            sprintf(buffer, "_stack_index = %d", _stack_index);
            return buffer;
        }

    private:

        inline static void initialize()
        {
            for(register int i = 0; i < SIZE; ++i) {

                _stack[i] = &_pool[i];
            }

            _stack_index = SIZE - 1;
            _initialized = true;
        }

        static T _pool[SIZE];
        static T_ptr _stack[SIZE];
        static int _stack_index;
        static bool _initialized;
};

template <class T, int SIZE> T poolable<T, SIZE>::_pool[SIZE];
template <class T, int SIZE> T* poolable<T, SIZE>::_stack[SIZE];
template <class T, int SIZE> int poolable<T, SIZE>::_stack_index = 0;
template <class T, int SIZE> bool poolable<T, SIZE>::_initialized = false;

#endif // POOL_INCLUDED 
