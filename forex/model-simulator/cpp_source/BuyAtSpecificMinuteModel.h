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

/*****************************  Notes  ****************************************
 
        The criteria to open an buy order:

        - Current minute should be the selected minute

        - The current BID price should be larger than the opening by spread

        In this case I create a SELL order (executed at the BID price) which will be followed by a BUY order (at ASK price) when:

        - The ASK price rises more than the STOP LOSS

        - The ASK price drops more than the PROFIT TAKE

        Example:

        MINUTE_TO_TRADE: 40
        SPREAD:          10 
        TAKE PROFIT:     10
        STOP LOSS:       10

        Open price for current hour:

        BID:  1.3112

        At 40 minute I have

        BID:  1.3123
        ASK:  1.3128

        SPREAD = 1.3123 - 1.3112 = 11

        Since 11 > 10 then go ahead and SELL at 1.3123

        Now wait until one of the following happens:

        -  ASK price goes more than 1.3123 + 0.0010 = 1.3133 where the STOP LOSS is reached

        or 

        -  ASK price goes more than 1.3123 - 0.0010 = 1.3113 where the TAKE PROFIT is reached

        Let's assume that some time later the ASK price has reached: 1.3135 
        This will trigger a BUY at this price which will close the order which now will look like this:

        SELL AT: 1.3123
        BUY  AT: 1.3135
        -----------------
        LOSS   : 0.0012

        PNL = -1 * LOSS * STANDARD_CONTRACT
            = -1 * 0.0012 * 100,000
            = USD -120
****************************************************************************/ 

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
                    _triggering_delta(1,100,2),
                    _stop_loss(4,100,2),
                    _take_profit(5,100,2),  
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
                _open_price = tick.bid;
                _triggered_for_current_hour = false;
            }
            else
            {
                    if(_triggered_for_current_hour || tick.minute != (int)_minute_to_buy)
                        return;

                    const double delta_in_pips = (tick.bid - _open_price) * 10000;

                    if(delta_in_pips >= (double)_triggering_delta)
                    {
                        add_order(Order::make( SELL, "NONE", (double)_stop_loss, (double)_take_profit, tick.bid));
                    }

                    _triggered_for_current_hour = true;
            }
        }

        virtual std::string to_string() const
        {
                using namespace std;
                char buffer[1024];
                sprintf(buffer, 
                        "id: %lu min: %d  trigger: %10.6f stop: %10.6f profit: %10.6f pnl %10.2f orders: %10d fitness: %15.3f mdd: %15.3f unf: %15.3f ", 
                        _id, (int)_minute_to_buy, (double)_triggering_delta, 
                        (double)_stop_loss, (double)_take_profit, pnl(), orders_count(), get_fitness(), get_max_drawdown() , get_unnormalized_fitness());
                return buffer;
        }
};

#endif
