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

#include "Model.h"
#include "CloneableDouble.h"

class BuyAtSpecificMinuteModel: public Model
{
    CloneableDouble _minute_to_buy;
    CloneableDouble _triggering_delta; // in pips
    CloneableDouble _stop_loss;        // in pips
    CloneableDouble _take_profit;      // in pips

    int _current_hour;
    double _open_price;
    bool _triggered_for_current_hour;

    protected:
        virtual void initialize_optimizable_fields()
        {
                add_optimizable_field(&_minute_to_buy);
                add_optimizable_field(&_triggering_delta);
                add_optimizable_field(&_stop_loss);
                add_optimizable_field(&_take_profit);
         }

    public:
        BuyAtSpecificMinuteModel():
                    _minute_to_buy(20,60,1),
                    _triggering_delta(10,30,2),
                    _stop_loss(5,20,2),
                    _take_profit(5,20,2),  
                    _current_hour(-1),
                    _open_price(0.0),
                    _triggered_for_current_hour(false)
        {
            initialize_optimizable_fields();
        }


        virtual ~BuyAtSpecificMinuteModel()
        {
        }

        virtual void process(const Tick& tick)
        {
            using namespace std;
            
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

                    if(delta_in_pips >= (double)_triggering_delta)
                    {
                        add_order(Order::make( BUY, "NONE", (double)_stop_loss, (double)_take_profit, tick.ask));
                    }

                    _triggered_for_current_hour = true;
            }
        }

        virtual std::string to_string() const
        {
                using namespace std;
                char buffer[1024];
                sprintf(buffer, 
                        "id: %lu min: %d  trigger: %10.6f stop: %10.6f profit: %10.6f ", 
                        _id, (int)_minute_to_buy, (double)_triggering_delta, 
                        (double)_stop_loss, (double)_take_profit);
                return buffer;
        }

};

#endif
