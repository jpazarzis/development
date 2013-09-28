#ifndef TICK_PROCESSOR_INCLUDED
#define TICK_PROCESSOR_INCLUDED
#define TICK_PROCESSOR_INCLUDED

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

