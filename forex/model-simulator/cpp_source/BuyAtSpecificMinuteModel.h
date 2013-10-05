#ifndef MODELS_INCLUDED
#define MODELS_INCLUDED
#define MODELS_INCLUDED

/////////////////////////////////////////////////////
// BuyAtSpecificMinuteModel.h
// 
// Author: John Pazarzis
//
// Date  : Oct. 3 2013
//
// Summary: 
//    Implements a sample model
////////////////////////////////////////////////////////////////////////

#include "TickProcessor.h"
#include "Order.h"
#include <vector>
#include <iostream>
#include "ga.h"
#include "Logger.h"

#define OPTIMIZABLE_FIELDS_COUNT 4 

class BuyAtSpecificMinuteModel: public TickProcessor, Populatebale<OPTIMIZABLE_FIELDS_COUNT>
{

    double _minute_to_buy;
    double _triggering_delta; // in pips
    double _stop_loss;        // in pips
    double _take_profit;      // in pips
    int _current_hour;
    double _open_price;
    bool _triggered_for_current_hour;
    TickEngine* _p_tick_engine;
    std::vector<ORDER_PTR> _orders;


    public:
        BuyAtSpecificMinuteModel():
                    _current_hour(-1),
                    _open_price(0.0),
                    _triggered_for_current_hour(false),
                    _p_tick_engine(NULL)
        {
        }

        virtual ~BuyAtSpecificMinuteModel()
        {
            stop_feed();
        }

        virtual std::string to_string() const
        {
                using namespace std;
                char buffer[1024];
                
                sprintf(buffer, 
                        "min: %d trigger: %10.6f stop: %10.6f profit: %10.6f fitness: %10.6f ", 
                        (int)_minute_to_buy, _triggering_delta, _stop_loss, _take_profit, get_fitness());

                return buffer;
        }

        virtual void populate(const Chromosome<OPTIMIZABLE_FIELDS_COUNT>& chromosome ) 
        {

            _minute_to_buy = chromosome._weight[0];
            _triggering_delta = chromosome._weight[1];
            _stop_loss = chromosome._weight[2]; 
            _take_profit = chromosome._weight[3];

            _orders.clear();
            _current_hour = -1;
            _open_price = 0.0;
            _triggered_for_current_hour = false;
        }

        virtual double get_fitness() const 
        {
            const int orders_count = _orders.size();
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
            return pnl;
        }

        void bind_tick_engine(TickEngine* p_tick_engine)
        {
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);

        }
        virtual void process(const Tick& tick)
        {
            if(tick.hour != _current_hour)
            {

                _current_hour = tick.hour;
                _open_price = tick.ask;
                _triggered_for_current_hour = false;
            }
            else
            {
                    if(_triggered_for_current_hour || tick.minute != (int)_minute_to_buy)
                        return;

                    const double delta_in_pips = (tick.ask - _open_price) * 10000;

                    if(delta_in_pips >= _triggering_delta)
                    {
                        ORDER_PTR p_order = Order::make( BUY, "NONE", _stop_loss, _take_profit, tick.ask);
                        _p_tick_engine->register_processor(p_order);  
                        _orders.push_back(p_order);
                    }

                    _triggered_for_current_hour = true;
            }
        }
};

#endif
