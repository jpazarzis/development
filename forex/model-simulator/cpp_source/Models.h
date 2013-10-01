#ifndef MODELS_INCLUDED
#define MODELS_INCLUDED
#define MODELS_INCLUDED

/////////////////////////////////////////////////////
// Models.h
// 
// Author: John Pazarzis
//
// Date  : Sep. 28 2013
//
// Summary: 
//    Implements a sample model
////////////////////////////////////////////////////////////////////////


#include "TickProcessor.h"
#include "Order.h"
#include <vector>
#include <iostream>


class BuyAtSpecificMinuteModel: public TickProcessor
{
    const std::string _instrument;
    const int _minute_to_buy;
    const double _triggering_delta;
    const double _stop_loss; 
    const double _take_profit;
    int _current_hour;
    double _open_price;
    bool _triggered_for_current_hour;
    TickEngine* _p_tick_engine;
    std::vector<ORDER_PTR> _orders;
    
    public:

        BuyAtSpecificMinuteModel(   const std::string& instrument,
                                    int minute_to_buy,
                                    double delta,
                                    double stop_loss,
                                    double take_profit):
                    _instrument(instrument),
                    _minute_to_buy(minute_to_buy),
                    _triggering_delta(delta),
                    _stop_loss(stop_loss),
                    _take_profit(take_profit),
                    _current_hour(-1),
                    _open_price(0.0),
                    _triggered_for_current_hour(false),
                    _p_tick_engine(NULL)
        
        {
            
        }

        void show_stats() const
        {
            using namespace std;
            const int orders_count = _orders.size();
            cout << "number of orders: " << orders_count << endl;
            double pnl = 0.0;
            for(int i = 0; i < orders_count; ++i)
            {
                try
                {
                    pnl += _orders[i]->get_pnl();
                }
                catch(char* psz)
                {
                    cout << psz << endl;
                }
            }
            cout << "pnl: " << pnl << endl;

        }

        void bind_tick_engine(TickEngine* p_tick_engine)
        {
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);

        }
                
        virtual PROCESSOR_RESULT process(const Tick& tick)
        {
            if(tick.hour != _current_hour)
            {

                _current_hour = tick.hour;
                _open_price = tick.ask;
                _triggered_for_current_hour = false;
               
                return CONTINUE_PROCESSING;
            }

            if(_triggered_for_current_hour)
                return CONTINUE_PROCESSING;

            double delta = tick.ask - _open_price;

            if(delta >= _triggering_delta)
            {
                
                ORDER_PTR p_order = Order::make( BUY, _instrument, _stop_loss, _take_profit, tick.ask);
                _p_tick_engine->register_processor(p_order);    
                _triggered_for_current_hour = true;
                _orders.push_back(p_order);
            }

            return CONTINUE_PROCESSING;
        }

};


#endif
