// Tick.h
//
// Author        : John Pazarzis

#ifndef TICK_INCLUDED
#define TICK_INCLUDED

#include "std_include.h"

class Tick final
{
    private:

        double _bid;
        double _ask;
        DATE_TIME _date;

    public:

        Tick() {}

        Tick(const Tick& other) = delete;

        Tick& operator = (const Tick& other) = delete;

        inline void assign_values(int day, int month, int year, int hour, int minute, int second, double bid, double ask)
        {
            _date = DATE_TIME(DATE(year, month, day), hours(hour) + minutes(minute) + seconds(second));
            _bid = bid, _ask = ask;
        }

        inline CONST_DATE_TIME_REF timestamp() const
        {
            return _date;
        }

        inline double bid() const
        {
            return _bid;
        }

        inline double ask() const
        {
            return _ask;
        }
};

#endif // TICK_INCLUDED
