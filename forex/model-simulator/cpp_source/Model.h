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
#include "Utilities.h"
#include <stdio.h>
#include <stdlib.h>

#define ACCOUNT_STARTING_POINT 100000
#define MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION 20

class Model : virtual public Object, virtual public Identifiable, public TickProcessor, public Optimizable
{
    public:

        Model() : 
            _p_tick_engine(NULL),
            _number_of_orders(0),
            _max_draw_down(0),
            _account_balance(0),
            _absolute_low(0),
            _winning_trades(0),
            _lossing_trades(0),
            _consequtive_wins(0),
            _consequtive_losses(0)

            
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
            initialize();
            set_fitness(0);
            unmark_stop_feed();
            _p_tick_engine = p_tick_engine;
            _p_tick_engine->register_processor(this);
        }
       
        void stop_listening()
        {
            stop_feed();
            _orders.clear();
            _p_tick_engine = NULL;
        }

        int get_winning_trades_count() const
        {
            return _winning_trades;
        }

        int get_loosing_trades_count() const
        {
            return _lossing_trades;
        }

        double get_pnl() const
        {
            return _account_balance - ACCOUNT_STARTING_POINT;
        }

        double get_absolute_low() const
        {
            return _absolute_low;
        }
       
       
        double get_account_balance() const
        {
            return _account_balance;
        }

        double get_max_drawdown() const
        {
            return _max_draw_down;
        }

        int orders_count() const
        {
            return _number_of_orders;
        }

        virtual void calc_fitness(bool save_account_balance_curve = false)
        {
            using namespace std;
            _number_of_orders = _orders.size();
            if(_number_of_orders <= MIN_NUMBER_OF_ORDERS_TO_USE_FOR_OPTIMAZATION)
            {
               initialize(); 
               set_fitness(0);
                _orders.clear();
            }
            else
            {
                    _account_balance = ACCOUNT_STARTING_POINT;
                    
                    _consequtive_wins = 0;
                    _consequtive_losses = 0;
                    std::vector<double> account_balance_curve;

                    account_balance_curve.push_back(_account_balance);

                    for(int i = 0; i < _number_of_orders; ++i)
                    {
                        const double trade_pnl = _orders[i]->get_pnl();
                        if(trade_pnl > 0)
                            ++_winning_trades;
                        else if(trade_pnl < 0)
                             ++_lossing_trades;

                        _account_balance += trade_pnl;

                        if(_account_balance <= 0)
                        {   
                            // sorry, we have gotten broke
                            initialize();
                            _absolute_low = 0;
                            set_fitness(0);
                            _orders.clear();
                            return;
                        }

                        account_balance_curve.push_back(_account_balance);
                    }


                    _max_draw_down = max_drawdown(account_balance_curve);
                    _absolute_low = *std::min_element(account_balance_curve.begin(), account_balance_curve.end()); 

                    if(save_account_balance_curve)
                    {
                        std::string filename;
                        char buffer [33];
                        sprintf(buffer,"%d",(int)_id);
                        filename = "SellAtSpecificMin" + std::string(buffer) + ".csv";

                        save_curve(account_balance_curve, filename);
                    }

                    assert(_account_balance > 0);
                    assert(_max_draw_down >= 0);

                    // In case that the max drawdown is zero just ignore the chromosome
                    // this is a case that will happen when there is not a single loosing
                    // trade something that needs a very small number of trades
                    // Note that a very low _max_draw_down will result to a high
                    // fitness creating a bias for models creating very few
                    // orders (since they are more possible to create a very
                    // small amount of lossing trades..
                    if(_max_draw_down <= 0.001)
                    {
                            initialize();
                            set_fitness(0);
                    }
                    else
                    {
                        set_fitness((_account_balance*1.0) / exp(_max_draw_down) );
                        //set_fitness((_account_balance*1.0) / (_max_draw_down *100.0) ) ;
                    }
            }
            _orders.clear();
        }

    protected:
        virtual void initialize_optimizable_fields() = 0;

        void initialize()
        {
            _number_of_orders = 0;
            _max_draw_down = 0;
            _account_balance = 0;
            _absolute_low = 0;
            _winning_trades = 0;
            _lossing_trades = 0;
            _consequtive_wins = 0;
            _consequtive_losses = 0;
        }

    private:
        std::vector<ORDER_PTR> _orders;
        TickEngine* _p_tick_engine;
        double _number_of_orders;
        double _max_draw_down;
        double _account_balance;
        double _absolute_low;
        int _winning_trades;
        int _lossing_trades;
        int _consequtive_wins;
        int _consequtive_losses;

};
#endif // MODEL_INCLUDED
