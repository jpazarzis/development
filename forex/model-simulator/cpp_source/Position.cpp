#include "Position.h"
#include "Tick.h"
#include "TickPool.h"
#include <memory>
#include <iostream>
using namespace std;

#define INVALID_PRICE -99999

class LongPosition : public Position {
        
    public:
        LongPosition(const Tick& tick, double number_of_lots, int lot_size):
            Position(tick.timestamp(), DATE_TIME(), tick.ask(),INVALID_PRICE, number_of_lots,lot_size) {
            //printf("opening long position: buy at %f lots#: %6.2f\n", tick.ask(), number_of_lots);
        }

        virtual void close(const Tick& current_tick) {
            _closing_timestamp = current_tick.timestamp();
            _sell_price = current_tick.bid();
        }

        double entering_cost() const  {
            return (_lot_size * _buy_price * _number_of_lots) / 50;
        }

        virtual double floating_profit(const Tick& tick) const  {
            return (tick.bid() - _buy_price) * _number_of_lots * _lot_size;
        }

        virtual std::string to_string() const {
            char buffer[1024];
            sprintf(buffer, "%20s %20s %10.4f %10.4f %10.2f %10.0f",
                             format_datetime(_opening_timestamp).c_str(),
                             format_datetime(_closing_timestamp).c_str(),
                            _buy_price, 
                            _sell_price,
                            _number_of_lots, 
                             pnl());
            return buffer;
        }
};

class ShortPosition : public Position {
    public:
        ShortPosition(const Tick& tick, double number_of_lots, int lot_size):
            Position(tick.timestamp(), DATE_TIME(),INVALID_PRICE,tick.bid(),number_of_lots,lot_size) {
        }

        virtual void close(const Tick& tick) {
            _buy_price = tick.ask();
        }

        double entering_cost() const  {
            return (_lot_size * _sell_price * _number_of_lots) / 50;
        }

        virtual double floating_profit(const Tick& tick) const  {
            return (_sell_price - tick.ask()) * _number_of_lots * _lot_size;;
        }

        virtual std::string to_string() const {
            char buffer[1024];
            sprintf(buffer, "Short Position: bought@ %8.6f sold@ %8.6f",_buy_price, _sell_price );
            return buffer;
        }

};


Position::Position(CONST_DATE_TIME_REF opening_timestamp, CONST_DATE_TIME_REF closing_timestamp, double buy_price, double sell_price, double number_of_lots, int lot_size):
    _opening_timestamp(opening_timestamp),
    _closing_timestamp(closing_timestamp),
    _buy_price(buy_price),
    _sell_price(sell_price),
    _number_of_lots(number_of_lots),
    _lot_size(lot_size){
}


double Position::pnl() const
{
    return (_sell_price - _buy_price) * _lot_size *  _number_of_lots;
}

std::unique_ptr<Position> long_position_factory(const Tick& tick, double number_of_lots, int lot_size)
{
    return std::unique_ptr<Position>(new LongPosition(tick,number_of_lots, lot_size));
}

std::unique_ptr<Position> short_position_factory(const Tick& tick, double number_of_lots, int lot_size)
{
    return std::unique_ptr<Position>(new ShortPosition(tick,number_of_lots, lot_size));
}


std::string Position::header() {
    std::string strg;
    char buffer[1024];
    sprintf(buffer, "%20s %20s %10s %10s %10s %10s\n", "opened", "closed", "buy", "sell", "lots", "pnl");
    strg += buffer;
    sprintf(buffer, "%20s %20s %10s %10s %10s %10s", "------", "------", "---", "----", "----", "---");
    strg += buffer;
    return strg;
}
