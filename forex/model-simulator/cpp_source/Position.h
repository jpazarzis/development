// Position.h
//
// Author        : John Pazarzis
// Creation date : Fri 08 Nov 2013 09:16:12 AM EST
//
// Summary
//      Summary goes here
//
// Notes
//

#ifndef POSITION_INCLUDED
#define POSITION_INCLUDED
#include "std_include.h"
#include <memory>
class Tick;

class Position {
    protected:
        DATE_TIME _opening_timestamp, _closing_timestamp;
        double _buy_price, _sell_price;
        const double _number_of_lots;
        const int _lot_size;
        Position(CONST_DATE_TIME_REF opening_timestamp, CONST_DATE_TIME_REF closing_timestamp, double buy_price, double sell_price, double number_of_lots, int lot_size);
    public:
        virtual double entering_cost() const = 0;
        virtual double floating_profit(const Tick& current_tick) const = 0;
        virtual void close(const Tick& current_tick) = 0;
        virtual double pnl() const; 
        virtual std::string to_string() const = 0;

        static std::string header();
};

using POSITION_FACTORY = std::function< std::unique_ptr<Position>(const Tick& tick,double number_of_lots, int lot_size)>;

std::unique_ptr<Position> long_position_factory(const Tick& tick, double number_of_lots, int lot_size);
std::unique_ptr<Position> short_position_factory(const Tick& tick, double number_of_lots, int lot_size);


#endif // POSITION_INCLUDED
