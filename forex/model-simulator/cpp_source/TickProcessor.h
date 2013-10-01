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



struct Tick
{
    int day, month, year, hour, minute, second;
    double bid, ask;
};

enum PROCESSOR_RESULT
{
    CONTINUE_PROCESSING = 0,
    STOP_PROCESSING = 1
};

class TickProcessor
{
    public:
        virtual PROCESSOR_RESULT process(const Tick& tick) = 0;
};

#endif

