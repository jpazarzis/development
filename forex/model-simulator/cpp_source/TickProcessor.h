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
//
#include "Tick.h"

class TickProcessor
{
        bool _marked_to_stop_feed;
    public:
        TickProcessor() : _marked_to_stop_feed(false) {}

        virtual ~ TickProcessor() {}

        virtual void process(const Tick& tick) = 0;

        virtual bool is_marked_to_stop_feed() const
        {
            return _marked_to_stop_feed;
        }

        virtual void stop_feed()
        {
            _marked_to_stop_feed = true;
        }

        virtual void unmark_stop_feed()
        {
            _marked_to_stop_feed = false;
        }
};
#endif

