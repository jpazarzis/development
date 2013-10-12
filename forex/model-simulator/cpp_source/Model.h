// Model.h
//
// Author        : John Pazarzis
// Creation date : Fri 11 Oct 2013 08:50:37 AM EDT
//
// Summary
//      Base class for a model

#ifndef MODEL_INCLUDED
#define MODEL_INCLUDED

#include "std_include.h"
#include "TickProcessor.h"
#include "Order.h"
#include "Optimizable.h"
#include "ModelMetrics.h"

#define ACCOUNT_STARTING_POINT 10000
#define MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION 20

class Model : virtual public Object, virtual public Identifiable, public TickProcessor, public Optimizable
{
    public:

        Model() : 
            _p_metrics(NULL), 
            _p_tick_engine(NULL),
            _account_starting_point (ACCOUNT_STARTING_POINT),
            _account_balance(ACCOUNT_STARTING_POINT)
        {
        }

        virtual ~Model() 
        { 
            stop_feed(); 
            delete _p_metrics;
            _p_metrics = NULL;
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
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);
        }
       
        void stop_listening()
        {
            stop_feed();
            calculate_metrics();
            _orders.clear();
            _p_tick_engine = NULL;
        }

        double get_max_drawdown() const
        {
            return NULL != _p_metrics ? _p_metrics->get_max_drawdown() : 0;
            
        }

        double get_unnormalized_fitness() const
        {
            return NULL != _p_metrics ? _p_metrics->get_unnormalized_fitness() : 0;
        }



        double pnl() const
        {
            return NULL != _p_metrics ? _p_metrics->pnl() : 0;
        }

        int orders_count() const
        {
            return NULL != _p_metrics ? _p_metrics->orders_count() : 0;
        }

        void normalize_fitness(double min_fitness)
        {
            assert(NULL != _p_metrics);
            if(_p_metrics->orders_count() < MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION)
            {
                set_fitness(0);
                return;
            }
            const double f = _p_metrics->calc_unnormalized_fitness();
            if(min_fitness <=0)
            {
                set_fitness(f + fabs(min_fitness) + 1.0);
            }
            else
            {
                set_fitness(f + 1.0);
            }
            assert(get_fitness() > 0);
        }

        void add_unormalized_fitness(std::vector<double>& unnormalized_fitness)
        {
            assert(NULL != _p_metrics);
            if(_p_metrics->orders_count() >= MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION)
            {
                unnormalized_fitness.push_back(_p_metrics->calc_unnormalized_fitness());
            }
        }

        virtual void calculate_metrics()
        {
            const int orders_count = _orders.size();
            _account_balance = _account_starting_point;
            std::vector<double> account_balance_curve;
            for(int i = 0; i < orders_count; ++i)
            {
                _account_balance += _orders[i]->get_pnl();
                if(_account_balance <= 0)
                {
                    _account_starting_point += ACCOUNT_STARTING_POINT;
                    _account_balance += ACCOUNT_STARTING_POINT;

                }
                account_balance_curve.push_back(_account_balance);
            }
            if(NULL != _p_metrics)
            {
                delete _p_metrics;
            }
            const double pnl = _account_balance - _account_starting_point;
            _p_metrics = new ModelMetrics(pnl, orders_count, account_balance_curve);
        }


    protected:

        virtual void initialize_optimizable_fields() = 0;


    private:
        std::vector<ORDER_PTR> _orders;
        TickEngine* _p_tick_engine;
        ModelMetrics* _p_metrics;
        double _account_starting_point;
        double _account_balance;
};
#endif // MODEL_INCLUDED
