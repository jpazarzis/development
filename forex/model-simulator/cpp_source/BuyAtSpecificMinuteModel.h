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
#include <fstream>
#include "GeneticAlgorithm.h"
#include "Logger.h"
#include "Identifiable.h"
#include "std_include.h"



class BuyAtSpecificMinuteModel: public TickProcessor, public Optimizable, virtual public Identifiable
{

    CloneableDouble _minute_to_buy;
    CloneableDouble _triggering_delta; // in pips
    CloneableDouble _stop_loss;        // in pips
    CloneableDouble _take_profit;      // in pips

    int _current_hour;
    double _open_price;
    bool _triggered_for_current_hour;
    TickEngine* _p_tick_engine;
    std::vector<ORDER_PTR> _orders;

    double _pnl;
    double _max_drop_down;
    
    void initialize()
    {
        add_optimizable_field(&_minute_to_buy);
        add_optimizable_field(&_triggering_delta);
        add_optimizable_field(&_stop_loss);
        add_optimizable_field(&_take_profit);
     }

        virtual void assign_max_drop_down() 
        {
            const int orders_count = _orders.size();
            if(orders_count == 0)
            {
                _max_drop_down =  INVALID_FITNESS;
                return;
            }
            double max_pnl = -9999999999, min_pnl = 9999999999, pnl = 0.0;
            for(int i = 0; i < orders_count; ++i)
            {
                pnl += _orders[i]->get_pnl();
                if(pnl > max_pnl)
                    max_pnl = pnl;
                if(pnl < min_pnl)
                    min_pnl = pnl;
            }

            const double delta = fabs(max_pnl - min_pnl);
            if(delta < 0.01)
                _max_drop_down =  INVALID_FITNESS;
            else
                _max_drop_down =  delta;
        }


        void assign_pnl() 
        {
            const int orders_count = _orders.size();

            if(orders_count == 0)
            {
                _pnl = INVALID_FITNESS;
                return;
            }

            double pnl = 0.0;
            for(int i = 0; i < orders_count; ++i)
            {
                pnl += _orders[i]->get_pnl();
            }
            _pnl = pnl;
        }


    public:
        BuyAtSpecificMinuteModel():
                     _minute_to_buy(20,60,1),_triggering_delta(10,30,2),_stop_loss(5,20,2),_take_profit(5,20,2),  
                    _current_hour(-1),
                    _open_price(0.0),
                    _triggered_for_current_hour(false),
                    _p_tick_engine(NULL)
        {
            initialize();
        }

        BuyAtSpecificMinuteModel(const BuyAtSpecificMinuteModel& other );

        BuyAtSpecificMinuteModel& operator=(const BuyAtSpecificMinuteModel& other );


        virtual ~BuyAtSpecificMinuteModel()
        {
            stop_feed();
        }

        void clear()
        {
            _orders.clear();
        }


        void dump_pnl_curve(std::ofstream& stream)
        {
            const int orders_count = _orders.size();

            double pnl = 0.0;

            for(int i = 0; i < orders_count; ++i)
            {
                try
                {
                    pnl += _orders[i]->get_pnl();
                    stream << pnl << ",";        
                }
                catch(char* psz)
                {
                    cout << psz << endl;
                }
            }
            stream << endl;
        }

        static std::string  csv_header()
        {
            return  "id,minute,trigger,stop_loss, profit_take, fitness,pnl,order_count";
        }

        virtual std::string to_csv()const
        {
                using namespace std;
                char buffer[1024];

                const int orders_count = _orders.size();
                
                sprintf(buffer, 
                        "%lu,%d,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%d ", 
                        _id, (int)_minute_to_buy, (double)_triggering_delta, (double)_stop_loss, (double)_take_profit, get_fitness(), get_pnl(), orders_count);

                return buffer;
        }

    

        int number_of_orders() const
        {
            return _orders.size();
        }

        virtual std::string to_string() const
        {
                using namespace std;
                char buffer[1024];

                const int orders_count = _orders.size();
                
                sprintf(buffer, 
                        "id: %lu min: %d  trigger: %10.6f stop: %10.6f profit: %10.6f fitness: %10.6f PNL: %10.6f count orders: %d ", 
                        _id, (int)_minute_to_buy, (double)_triggering_delta, (double)_stop_loss, (double)_take_profit, get_fitness(), get_pnl(), orders_count);

                return buffer;
        }


    
        double get_pnl() const
        {
            return _pnl;
        }



        void assign_fitness()
        {
            assign_max_drop_down();
            assign_pnl();

            if((long)_max_drop_down == (long)INVALID_FITNESS || (long)_pnl == (long)INVALID_FITNESS)
            {
                set_fitness(INVALID_FITNESS);
            }

            set_fitness(_pnl);
        }

        
        void bind_tick_engine(TickEngine* p_tick_engine)
        {
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);

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

                        ORDER_PTR p_order = Order::make( BUY, "NONE", (double)_stop_loss, (double)_take_profit, tick.ask);
                        _p_tick_engine->register_processor(p_order);  
                        _orders.push_back(p_order);
                    }

                    _triggered_for_current_hour = true;
            }
        }
};

#endif
