#include "Order.h"
#include <iostream>
using namespace std;


#define UNINITIALIZED_PRICE -9999999.9

class OrderPool
{
    public:
        std::vector<Order*> _pool;

    ~OrderPool()
    {
        for(int i = 0; i < _pool.size(); ++i)
        {
            delete _pool[i];
            _pool[i] = NULL;
        } 
    }
};

OrderPool Order::_order_pool;


int Order::orders_count() 
{
    return Order::_order_pool._pool.size(); 
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
            _sell_price(UNINITIALIZED_PRICE)
            
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

PROCESSOR_RESULT Order::process(const Tick& tick) 
{
    if(_order_status == CLOSED)
        return STOP_PROCESSING;

    if(_order_type == BUY)
    {
        const double current_price = tick.bid;
        const double delta = current_price - _buy_price;

        if(delta <= _stop_loss || delta >= _take_profit)        
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

    return _order_status == CLOSED ? STOP_PROCESSING : CONTINUE_PROCESSING;
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
        return 0.0;
    }
    cout << _sell_price << " " << _buy_price << endl;
    return (_sell_price - _buy_price) * 100000.0;
}




