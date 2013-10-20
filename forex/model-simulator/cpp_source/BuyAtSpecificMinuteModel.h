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
        LOSS   : -0.0012

        PNL =  LOSS * STANDARD_CONTRACT
            =  -0.0012 * 100,000
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
    double _high_for_the_hour;
    double _low_for_the_hour;

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
                    _triggering_delta(2,12,2),
                    _stop_loss(8,100,2),
                    _take_profit(10,100,2),  
                    _current_hour(-1),
                    _open_price(0.0),
                    _triggered_for_current_hour(false)
        {
            initialize_optimizable_fields();
        }


        void set_values(double minute, double delta, double sl, double pt)
        {
            _minute_to_buy.read_from_double(minute);
            _triggering_delta.read_from_double(delta);
            _stop_loss.read_from_double(sl);
            _take_profit.read_from_double(pt);
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
                _high_for_the_hour = -99999;
                _low_for_the_hour = 99999;
            }
            else
            {
                    if(tick.bid > _high_for_the_hour)
                        _high_for_the_hour = tick.bid;

                    if(tick.bid < _low_for_the_hour)
                        _low_for_the_hour = tick.bid;



                    if(_triggered_for_current_hour || tick.minute != (int)_minute_to_buy)
                        return;

                    //const double delta_in_pips = (tick.bid - _open_price) * 10000;
                    const double delta_in_pips = (_high_for_the_hour - tick.bid) * 10000;

                    if(delta_in_pips >= (double)_triggering_delta)
                    {
                        add_order(Order::make( SELL, "NONE", (double)_stop_loss, (double)_take_profit, tick.bid, tick.timestamp()));
                        LOG << tick.timestamp() << " " << tick.bid << " " << tick.ask << EOL;
                    }

                    _triggered_for_current_hour = true;
            }
        }


        std::string get_full_description() const
        {
            std::string strg;

            strg += sformat("id:", "%20s");
            strg += sformat((int)_id,"%20d"); 
            strg += "\n";


            strg += sformat("minute to trade:", "%20s");
            strg += sformat((int)_minute_to_buy,"%20d"); 
            strg += "\n";

            strg += sformat("delta:", "%20s");
            strg += sformat((double)_triggering_delta, "%20.5f"); 
            strg += "\n";

            strg += sformat("stop loss:", "%20s");
            strg += sformat((double)_stop_loss, "%20.5f"); 
            strg += "\n";

            strg += sformat("profit take:", "%20s");
            strg += sformat((double)_take_profit, "%20.5f"); 
            strg += "\n";

            strg += sformat("number of orders:", "%20s");
            strg += sformat(orders_count(),"%20d"); 
            strg += "\n";

            strg += sformat("final balance:", "%20s");
            strg += sformat(get_account_balance(), "%20.2f");
            strg += "\n";

            strg += sformat("final pnl:", "%20s");
            strg += sformat(get_pnl(), "%20.2f");
            strg += "\n";


            strg += sformat("low:", "%20s");
            strg += sformat(get_absolute_low(), "%20.2f");
            strg += "\n";

            strg += sformat("winning trades:", "%20s");
            strg += sformat(get_winning_trades_count(), "%20d");
            strg += "\n";

            strg += sformat("losing trades:", "%20s");
            strg += sformat(get_loosing_trades_count(), "%20d");
            strg += "\n";

            strg += sformat("max drawdown:", "%20s");
            strg += sformat(get_max_drawdown()*100.0, "%20.2f");
            strg += "\n";


            return strg;


        }


        static std::string printing_header()
        {
                std::string strg;
                strg += sformat("id", "%5s");
                strg += sformat("min.", "%10s");
                strg += sformat("delta", "%13s");
                strg += sformat("stoploss", "%13s");
                strg += sformat("profittake", "%13s");
                strg += sformat("#orders", "%10s");
                strg += sformat("fitness", "%20s");
                strg += sformat("PNL", "%20s");
                strg += sformat("drawdown", "%20s");
                strg += sformat("balance", "%20s");
                strg += sformat("abs_low", "%20s");
                strg += sformat("win_count", "%20s");
                strg += sformat("losse_count", "%20s");
                return strg;
        }

        virtual std::string to_string() const
        {
                std::string strg;
                strg += sformat(_id,"%5d"); 
                strg += sformat((int)_minute_to_buy,"%10d"); 
                strg += sformat((double)_triggering_delta, "%13.5f"); 
                strg += sformat((double)_stop_loss, "%13.5f"); 
                strg += sformat((double)_take_profit, "%13.5f");
                strg += sformat(orders_count(),"%10d"); 
                strg += sformat(get_fitness(), "%20.5f"); 
                strg += sformat(get_pnl(), "%20.5f"); 
                strg += sformat(get_max_drawdown() , "%20.5f"); 
                strg += sformat(get_account_balance(), "%20.5f");
                strg += sformat(get_absolute_low(), "%20.5f");
                strg += sformat(get_winning_trades_count(), "%20d");
                strg += sformat(get_loosing_trades_count(), "%20d");
                return strg;
        }
};

#endif
