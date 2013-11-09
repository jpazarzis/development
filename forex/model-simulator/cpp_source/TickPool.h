// TickPool.h
//
// Author        : John Pazarzis
// Creation date : Sun 27 Oct 2013 06:08:16 PM EDT
//
// Summary
//      Creates a preloaded pool of ticks

#ifndef TICKPOOL_INCLUDED
#define TICKPOOL_INCLUDED

#include "std_include.h"
#include "Tick.h"


#define MAX_SIZE_OF_TICK_POOL 10000000

class TickPool final
{
        Tick _pool[MAX_SIZE_OF_TICK_POOL];
        int _size;
        TickPool() {}
        void parse_tick(char* psz);
        static TickPool _the_singleton;
    public:
        static TickPool& singleton();
        TickPool(const TickPool& other) = delete;
        TickPool& operator = (const TickPool& other) = delete;
        void load(const std::string& filename, CONST_DATE_REF from_date, CONST_DATE_REF to_date);
        int size() const;
        Tick& operator[](int index) ;
};
#endif // TICKPOOL_INCLUDED
