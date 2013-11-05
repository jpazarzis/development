
// OpenMarginModel.h
//
// Author        : John Pazarzis
// Creation date : Tue 05 Nov 2013 06:28:50 AM EST
//
// Summary
//      Summary goes here
//
// Notes
//

#ifndef OPENMARGINMODEL_INCLUDED
#define OPENMARGINMODEL_INCLUDED


#include "Model.h"
#include "CloneableDouble.h"
#include "TickPool.h"
#include "FitnessStatistics.h"


class OpenMarginModel : public Model
{
    const int _leverage;
    double _balance;


    protected:
        virtual void initialize_optimizable_fields()
        {
            add_optimizable_field(&_minute_to_buy);
            add_optimizable_field(&_triggering_delta);
            add_optimizable_field(&_stop_loss);
            add_optimizable_field(&_take_profit);
            add_optimizable_field(&_expriration_minutes);
        }



    public:

        OpenMarginModel(int leverage):
            _leverage(leverage)
            
        {
        }

        virtual ~OpenMarginModel();


        void calculate_fitness()
        {
            TickPool& tp = TickPool::singleton();
            const int number_of_ticks = tp.size();
            std::vector<Order*> orders;

            for(register int i = 0; i < number_of_ticks; ++i)
            {
                process_tick(tp[i], orders, i);
            }

            _fitness_statistics = FitnessStatistics::make(orders);;
            set_fitness(_fitness_statistics.fitness());

            for(auto order_ptr : orders)
            {
                Order::release(order_ptr);
            }
        }


        virtual void process(const Tick& tick)
        {
            // Do nothing.....
        }

        void process_tick(const Tick& tick, std::vector<Order*>& orders, int current_tick_index)
        {
            if((int)tick.timestamp().time_of_day().hours() != _current_hour)
            {
                _current_hour = (int)tick.timestamp().time_of_day().hours();
                _open_price = tick.bid();
                _triggered_for_current_hour = false;
                _high_for_the_hour = tick.bid();
            }
            else
            {
                if(tick.bid() > _high_for_the_hour)
                {
                    _high_for_the_hour = tick.bid();
                }

                if(_triggered_for_current_hour || (int) tick.timestamp().time_of_day().minutes() != (int)_minute_to_buy)
                {
                    return;
                }

                const double delta_in_pips = (_high_for_the_hour - _open_price) * 10000;

                if(delta_in_pips >= (double)_triggering_delta)
                {
                    Order* pOrder = Order::make(SELL, (double)_stop_loss, (double)_take_profit, tick, (int)((double)_expriration_minutes));
                    assert(NULL != pOrder);
                    orders.push_back(pOrder);
                    pOrder->process_until_closing(current_tick_index);
                }

                _triggered_for_current_hour = true;
            }
        }


        

        virtual std::string to_string() const 
        {
            return "Object of type OpenMarginModel";
        }

        OpenMarginModel(const OpenMarginModel& other) = delete;
        
        OpenMarginModel& operator=(const OpenMarginModel& other) = delete;


    private:

};

#endif // OPENMARGINMODEL_INCLUDED
