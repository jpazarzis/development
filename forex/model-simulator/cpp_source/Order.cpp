#include "Order.h"
#include <iostream>
#include <string>
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Logger.h"
#include "TickPool.h"
#include <math.h>
#include "Pool.h"
using namespace std;

#define UNINITIALIZED_PRICE -9999999.9
#define DEFAULT_EXPIRATION_MINUTES 35

class OrderPool
{
    public:
        std::vector<Order*> _pool;

        ~OrderPool()
        {
            clear_order_pool();
        }

        void clear_order_pool()
        {
            for(int i = 0; i < _pool.size(); ++i)
            {
                delete _pool[i];
                _pool[i] = NULL;
            }

            _pool.clear();
        }

};

OrderPool Order::_order_pool;

void Order::clear_order_pool()
{
    _order_pool.clear_order_pool();
}

int Order::orders_count()
{
    return _order_pool._pool.size();
}


Order::Order()
{
}

Order::~Order()
{
}

std::string Order::to_string() const
{
    using namespace std;
    char buffer[1024];
    string order_type, order_status;

    if(_order_type == BUY)
    {
        order_type = "BUY";
    }
    else if(_order_type == SELL)
    {
        order_type = "SELL";
    }

    if(_order_status == OPEN)
    {
        order_status = "OPEN";
    }
    else if(_order_status == CLOSED)
    {
        order_status = "CLOSED";
    }

    sprintf(buffer,
            "id: %lu [%s] [%s] SL: %10.6f TP: %10.6f BUYPRICE: %10.6f SELLPRICE: %10.6f",
            _id,
            order_type.c_str(),
            order_status.c_str(),
            _stop_loss,
            _take_profit,
            _buy_price,
            _sell_price);
    return buffer;
}

void Order::process_until_closing(int current_tick_index)
{
    TickPool& tp = TickPool::singleton();
    const int number_of_ticks = tp.size();
    const int last_tick_index = number_of_ticks - 1;

    for(register int i = current_tick_index;
            i < number_of_ticks && _order_status != CLOSED;
            ++i)
    {
        process_tick(tp[i], i == last_tick_index);
    }

    assert(_order_status == CLOSED);
}

void Order::process_tick(const Tick& tick, bool is_the_last_tick)
{
    if(_order_status == CLOSED)
    {
        return ;
    }

    if(_order_type == BUY)
    {
    }
    else if(_order_type == SELL)
    {
        /// I need to buy so I can close the order. Let's see if we reached
        //either the stop loss or the take profit
        const double current_price = tick.ask();

        if(current_price == 0.0)
        {
            std::cout << "Whats going on ? " <<   tick.timestamp() << endl;
        }

        const double delta = fabs((_sell_price -  current_price) * 10000);

        if(current_price > _sell_price && delta >= _stop_loss)
        {
            // sorry! the stop loss was reached.. we have to buy at a loss
            _buy_price = current_price;
            _order_status = CLOSED;
            assert(_buy_price > _sell_price);
        }
        else if(current_price < _sell_price && delta >= _take_profit)
        {
            // Good news! The price went down, so now we can buy at a lower
            // level closing the order for a profit!!
            _buy_price = current_price;
            _order_status = CLOSED;
            assert(_buy_price < _sell_price);
        }
        else if(tick.timestamp() >= _expiration_time || is_the_last_tick)
        {
            //cout << "order was expired, created at: " << _creation_time << " current time: " << _expiration_time << " " << tick.hour << " "<< tick.minute << " " << tick.second << endl;
            _was_expired = true;
            _buy_price = current_price;
            _order_status = CLOSED;
        }
    }
}

void Order::process(const Tick& tick)
{
    if(_order_status == CLOSED)
    {
        return ;
    }

    if(_order_type == BUY)
    {
    }
    else if(_order_type == SELL)
    {
        /// I need to buy so I can close the order. Let's see if we reached
        //either the stop loss or the take profit
        const double current_price = tick.ask();

        if(current_price == 0.0)
        {
            std::cout << "Whats going on ? " <<   tick.timestamp() << endl;
        }

        const double delta = fabs((_sell_price -  current_price) * 10000);

        if(current_price > _sell_price && delta >= _stop_loss)
        {
            // sorry! the stop loss was reached.. we have to buy at a loss
            _buy_price = current_price;
            _order_status = CLOSED;
            assert(_buy_price > _sell_price);
        }
        else if(current_price < _sell_price && delta >= _take_profit)
        {
            // Good news! The price went down, so now we can buy at a lower
            // level closing the order for a profit!!
            _buy_price = current_price;
            _order_status = CLOSED;
            assert(_buy_price < _sell_price);
        }
        else if(tick.timestamp() >= _expiration_time)
        {
            //cout << "order was expired, created at: " << _creation_time << " current time: " << _expiration_time << " " << tick.hour << " "<< tick.minute << " " << tick.second << endl;
            _was_expired = true;
            _buy_price = current_price;
            _order_status = CLOSED;
        }
    }

    if(_order_status == CLOSED)
    {
        stop_feed();
    }
}


void Order::populate(OrderType order_type,
                     double stop_loss,
                     double take_profit,
                     const Tick& tick,
                     int expiration_minutes)
{
    _order_type = order_type;
    _stop_loss = stop_loss;
    _take_profit = take_profit;
    _order_status = OPEN;
    _buy_price = UNINITIALIZED_PRICE;
    _sell_price = UNINITIALIZED_PRICE;
    _creation_time = tick.timestamp();
    _expiration_time =  DATE_TIME(tick.timestamp() + boost::posix_time::minutes(expiration_minutes));
    _was_expired = false;

    if(order_type == BUY)
    {
        _buy_price = tick.ask();
    }
    else if(order_type == SELL)
    {
        _sell_price = tick.bid();
    }
}




Pool<Order, 400000> block_of_orders;


ORDER_PTR Order::make(OrderType order_type,
                      double stop_loss,
                      double take_profit,
                      const Tick& tick,
                      int expiration_minutes)
{
    Order* p_order = block_of_orders.get();
    p_order->populate(order_type, stop_loss, take_profit, tick, expiration_minutes);
    return p_order;
}

void Order::release(ORDER_PTR pOrder)
{
    block_of_orders.put_back(pOrder);
}


double Order::get_stop_loss() const
{
    return _stop_loss;
}

double Order::get_take_profit() const
{
    return _take_profit;
}

OrderStatus Order::get_order_status() const
{
    return _order_status;
}


bool Order::was_expired() const
{
    return _was_expired;
}

double Order::get_pnl() const
{
    if(_order_status == CLOSED)
    {
        double pnl = (_sell_price - _buy_price) * 100000.0;

        if(pnl > 10000)
        {
            std::cout << "Danger: " <<  _sell_price << " " << _buy_price << std::endl;
        }

        return (_sell_price - _buy_price) * 100000.0;
    }
    else
    {
        return 0.0;
    }
}

ORDER_RESULT Order::get_win_or_loss() const
{
    if(_order_status == CLOSED)
    {
        if(_sell_price > _buy_price)
        {
            return WIN;
        }
        else  if(_sell_price < _buy_price)
        {
            return LOSS;
        }
        else
        {
            return REMAINS_OPEN;
        }
    }
    else
    {
        return REMAINS_OPEN;
    }
}
