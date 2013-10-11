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


class Model : virtual public Object, virtual public Identifiable, public TickProcessor, public Optimizable
{
    public:

        Model() : 
            _p_metrics(NULL), _p_tick_engine(NULL) 
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
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);
        }
       
        void stop_listening()
        {
            stop_feed();
            calculate_metrics();
            _orders.clear();
        }

        ModelMetrics* metrics() const
        {
            return _p_metrics;
        }


    protected:

        virtual void initialize_optimizable_fields() = 0;


        virtual void calculate_metrics()
        {
            const int orders_count = _orders.size();
            double pnl = 0.0;
            std::vector<double> pnl_curve;
            for(int i = 0; i < orders_count; ++i)
            {
                const double order_pnl = _orders[i]->get_pnl();
                pnl_curve.push_back(pnl);
                pnl += order_pnl;
            }

            if(NULL != _p_metrics)
            {
                delete _p_metrics;
            }

            _p_metrics = new ModelMetrics(pnl, orders_count, pnl_curve);

            set_fitness(_p_metrics->get_fitness());
        }

    private:
        
        std::vector<ORDER_PTR> _orders;
        TickEngine* _p_tick_engine;
        ModelMetrics* _p_metrics;

};

#endif // MODEL_INCLUDED
