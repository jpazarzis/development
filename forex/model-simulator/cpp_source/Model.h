// Model.h
//
// Author        : John Pazarzis
// Creation date : Fri 11 Oct 2013 08:50:37 AM EDT
//
// Summary
//      Base class for a model. All the models should derive from this class

#ifndef MODEL_INCLUDED
#define MODEL_INCLUDED

#include "std_include.h"
#include "TickProcessor.h"
#include "Order.h"
#include "Optimizable.h"
#include "FitnessCalculators.h"

#define ACCOUNT_STARTING_POINT 10000
#define MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION 20

class Model : virtual public Object, virtual public Identifiable, public TickProcessor, public Optimizable
{
    public:

        Model() : 
            _p_tick_engine(NULL),
            _pnl(0),
            _number_of_orders(0),
            _max_draw_down(0),
            _unnormalized_fitness(0)
            
        {
        }

        virtual ~Model() 
        { 
            stop_feed(); 
        }

        Model(const Model& other);

        Model& operator=(const Model& other);

        void add_order(ORDER_PTR p_order)
        {
            _p_tick_engine->register_processor(p_order);  
            _orders.push_back(p_order);
        }

        void start_listening(TickEngine* p_tick_engine)
        {
            unmark_stop_feed();
            _pnl = 0;
            _number_of_orders = 0;
            _max_draw_down = 0;
            _unnormalized_fitness = 0;
            set_fitness(0);
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);
        }
       
        void stop_listening()
        {
            stop_feed();
            _orders.clear();
            _p_tick_engine = NULL;
        }

        double get_unnormalized_fitness() const
        {
            return _unnormalized_fitness;
        }

        double get_max_drawdown() const
        {
            return _max_draw_down;
            
        }

        double pnl() const
        {
            return _pnl;
        }

        int orders_count() const
        {
            return _number_of_orders;
        }

        void normalize_fitness(double delta)
        {
            _unnormalized_fitness = get_fitness();
            if(_number_of_orders < MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION)
            {
                set_fitness(0);
            }
            else
            {
                    if(delta <=0)
                    {
                        set_fitness(get_fitness() + fabs(delta));
                    }
            }

            assert(get_fitness() >= 0);
        }

        virtual void calc_fitness()
        {
            _number_of_orders = _orders.size();

            if(_number_of_orders < MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION)
            {
                _number_of_orders = 0;
                _pnl = 0;
                _max_draw_down = 0;
                set_fitness(0);
            }
            else
            {
                    double account_starting_point = ACCOUNT_STARTING_POINT;
                    double account_balance = ACCOUNT_STARTING_POINT;

                    std::vector<double> account_balance_curve;
                    for(int i = 0; i < _number_of_orders; ++i)
                    {
                        account_balance += _orders[i]->get_pnl();
                        if(account_balance <= 0)
                        {
                            account_starting_point += ACCOUNT_STARTING_POINT;
                            account_balance += ACCOUNT_STARTING_POINT;

                        }
                        account_balance_curve.push_back(account_balance);
                    }
                    _max_draw_down = max_drawdown(account_balance_curve);
                    _pnl = account_balance - account_starting_point;
                    double f = FitnessCalculators::calc_fitness(_pnl, account_balance_curve);
                    set_fitness(f);
            }

            _orders.clear();
        }

    protected:
        virtual void initialize_optimizable_fields() = 0;

    private:
        std::vector<ORDER_PTR> _orders;
        TickEngine* _p_tick_engine;
        double _pnl;
        double _number_of_orders;
        double _max_draw_down;
        double _unnormalized_fitness;
};
#endif // MODEL_INCLUDED
