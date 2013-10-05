#include "Order.h"
#include <iostream>
#include <string>
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Logger.h"
#include <math.h>

using namespace std;

#define UNINITIALIZED_PRICE -9999999.9

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

int Order::_id_seed = 100000;


void Order::clear_order_pool()
{
    _order_pool.clear_order_pool();
}


int Order::orders_count() 
{
    return _order_pool._pool.size(); 
}


Order::Order(   OrderType order_type,  
                const std::string& instrument, 
                double stop_loss, 
                double take_profit,
                double enter_price) : 

            _order_type(order_type),
            _instrument(instrument),
            _stop_loss(stop_loss), 
            _take_profit(take_profit), 
            _order_status(OPEN),
            _buy_price(UNINITIALIZED_PRICE),
            _sell_price(UNINITIALIZED_PRICE),
            _id(++_id_seed)
            
{
    if(order_type == BUY)
    {
        _buy_price = enter_price;
    }
    else if(order_type == SELL)
    {
        _sell_price = enter_price;
    }
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
            order_type = "BUY";
        else if(_order_type == SELL)
            order_type = "SELL";


        if(_order_status == OPEN)
            order_status = "OPEN";
        else if(_order_status == CLOSED)
            order_status = "CLOSED";

        sprintf(buffer, 
                "id: %d [%s] [%s] SL: %10.6f TP: %10.6f BUYPRICE: %10.6f SELLPRICE: %10.6f", 
                _id, 
                order_type.c_str(), 
                order_status.c_str(), 
                _stop_loss, 
                _take_profit, 
                _buy_price, 
                _sell_price);

        return buffer;
}

void Order::process(const Tick& tick) 
{
    if(_order_status == CLOSED)
        return ;

    if(_order_type == BUY)
    {
        // Now I need to sell to close the order
        //

        const double current_price = tick.bid;
        const double delta = (current_price - _buy_price) * 10000;
        

        if(delta > 0 && fabs(delta) >= _stop_loss)
        {    
            _sell_price = current_price;
            _order_status = CLOSED;
        }
        else if(delta < 0 && fabs(delta) >= _take_profit)
        {
            _sell_price = current_price;
            _order_status = CLOSED;
        }
        
    }
    else if(_order_type == SELL)
    {
        const double current_price = tick.ask;
        const double delta = _sell_price -  current_price;
        
        if(delta >= _stop_loss || delta <= _take_profit)        
        {
            _buy_price = current_price;
            _order_status = CLOSED;
        }
    }
        
    if(_order_status == CLOSED)
    {
        stop_feed();
    }
}


ORDER_PTR Order::make(  OrderType order_type,
                     const std::string& instrument,  
                     double stop_loss, 
                     double take_profit, 
                     double enter_price)
{
        Order* p_order = new Order(order_type,instrument, stop_loss, take_profit, enter_price);
        _order_pool._pool.push_back(p_order);
        return p_order;
}

std::string Order::get_instrument() const { return _instrument; }

double Order::get_stop_loss() const { return _stop_loss; }

double Order::get_take_profit() const { return _take_profit; }

OrderStatus Order::get_order_status() const { return _order_status; }

double Order::get_pnl() const
{
    if( _order_status == CLOSED)
    {
        return (_sell_price - _buy_price) * 100000.0;
    }
    else
    {
        return 0.0;
    }
}
