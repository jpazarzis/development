#ifndef TICK_PROCESSOR_INCLUDED
#define TICK_PROCESSOR_INCLUDED
#define TICK_PROCESSOR_INCLUDED

///////////////////////////////////////////////////////
// TickProcessor.h 
// 
// Author: John Pazarzis
//
// Date  : Sep. 28 2013
//
// Summary: 
//      Defines the Tick structure and the the TickProcessor ineterface
//      which needs to be implemented for any observer of the TickEngine
//      to receive callbaks on new ticks
////////////////////////////////////////////////////////////////////////


#include <string>
#include <stdio.h>


struct Tick
{
    int day, month, year, hour, minute, second;
    double bid, ask;
    
    std::string  to_string() const
    {
        using namespace std;
        char buffer[1024];

        sprintf(buffer, 
                " %d/%d/%d %d:%d:%d bid: %10.6f ask: %10.6f", 
                day, month, year, hour, minute, second,bid, ask);

        return buffer;
    }   
    
};

class TickProcessor
{
        bool _marked_to_stop_feed;

    public:
        TickProcessor() : _marked_to_stop_feed(false) {}
        
        virtual ~ TickProcessor() {}

        virtual void process(const Tick& tick) = 0;

        virtual bool is_marked_to_stop_feed() const { return _marked_to_stop_feed; }

        virtual void stop_feed() { _marked_to_stop_feed = true; }
};
        
#endif

