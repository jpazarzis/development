// FreeMarginModel.h
//
// Author        : John Pazarzis
// Creation date : Fri 08 Nov 2013 08:32:37 AM EST
//
// Summary
//      Implements a sample Free Margin Model
//

#ifndef FREEMARGINMODEL_INCLUDED
#define FREEMARGINMODEL_INCLUDED

#include "Position.h"
#include "TickPool.h"
#include <memory>
#include <vector>
#include <assert.h>

#include "Model.h"
#include "CloneableDouble.h"
#include "TickPool.h"
#include "FitnessStatistics.h"
#include "FreeMarginExecutor.h"
#include <iostream>

class FreeMarginModel: public Model {
        CloneableDouble _minute_to_buy;
        CloneableDouble _triggering_delta;
        FitnessStatistics _fitness_statistics;
        double _balance;
        std::string _name;
        const double _starting_balance;
        std::vector<FreeMarginExecutorSmartPtr> _exetutors;
        std::vector<double> _balance_curve;

    protected:
        virtual void initialize_optimizable_fields() {
            add_optimizable_field(&_minute_to_buy);
            add_optimizable_field(&_triggering_delta);
        }

    public:
        FreeMarginModel():
            _minute_to_buy(20, 60, 1),
            _triggering_delta(2, 12, 2),
            _name("Testing Free Marging Model"),
            _starting_balance(100000) {
            initialize_optimizable_fields();
        }

        void set_values(double minute, double delta) {
            _minute_to_buy.read_from_double(minute);
            _triggering_delta.read_from_double(delta);
        }

        virtual ~FreeMarginModel() {
        }

        bool entered_new_hour(const Tick& tick, int current_day, int current_hour){
            const int day = (int)tick.timestamp().date().day();
            const int hour = (int)tick.timestamp().time_of_day().hours();
            return day != current_day || hour != current_hour;
        }
        
        void calculate_fitness() {
            TickPool& tp = TickPool::singleton();
            const int number_of_ticks = tp.size();
            int current_day = -1, current_hour = -1;
            double open_price = -1, high_for_the_hour = -1;

            _exetutors.clear();
            _balance_curve.clear();
            _balance = _starting_balance;
            _balance_curve.push_back(_balance);

            for(register int i = 0; i < number_of_ticks; ++i) {
                const Tick& tick = tp[i];

                if(entered_new_hour(tick, current_day,current_hour)){
                    current_day = (int)tick.timestamp().date().day();
                    current_hour= (int)tick.timestamp().time_of_day().hours();
                    open_price = tick.bid();
                    high_for_the_hour = tick.bid();
                    continue;
                }
                else if(tick.bid() > high_for_the_hour){
                    high_for_the_hour = tick.bid();
                }

                if((int) tick.timestamp().time_of_day().minutes() != (int)_minute_to_buy) {
                    continue;
                }

                const double delta_in_pips = (high_for_the_hour - open_price) * 10000;

                if(delta_in_pips >= (double)_triggering_delta) {
                    FreeMarginExecutorSmartPtr executor(new FreeMarginExecutor(_balance, 
                                                            50 , 10, long_position_factory));
                    i = executor->start_processing(i);
                    _balance = executor->balance();
                    _exetutors.push_back(std::move(executor));
                    _balance_curve.push_back(_balance);
                }

                if(_balance <= 0) {
                    break;
                }
            }

            //_fitness_statistics = FitnessStatistics::make(orders);
            //set_fitness(_fitness_statistics.fitness());
        }

        virtual void process(const Tick& tick) {
        }



        std::string get_full_description() const {
            std::string strg;
            strg += sformat("id:", "%20s");
            strg += sformat((int)_id, "%20d");
            strg += "\n";
            strg += sformat("minute to trade:", "%20s");
            strg += sformat((int)_minute_to_buy, "%20d");
            strg += "\n";
            strg += sformat("delta:", "%20s");
            strg += sformat((double)_triggering_delta, "%20.5f");
            strg += "\n";
            strg += _fitness_statistics.get_full_description();
            return strg;
        }

        static std::string printing_header() {
            std::string strg;
            strg += sformat("id", "%5s");
            strg += sformat("min.", "%10s");
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

        virtual std::string to_string() const {
            char buffer[1024];
            std::string strg;
            strg + "=============================================================================\n";
            strg += "Model Name:" + _name + "\n\n";
            sprintf(buffer, "The model entered the market %i times\n", _exetutors.size());
            strg += buffer;
            sprintf(buffer, "Starting Balance: %10.0f\n", _starting_balance);
            strg += buffer;
            sprintf(buffer, "Final Balance   : %10.0f\n\n", _balance);
            strg += buffer;
            strg += "Balance Curve\n\n";

            for(int i = 0; i < _balance_curve.size(); ++i) {
                if(i >= 5 && i % 5 == 0) {
                    strg += "\n";
                }

                sprintf(buffer, "%10.0f", _balance_curve[i]);
                strg += buffer;

                if(i < _balance_curve.size() - 1) {
                    strg += ",";
                }
            }

            strg += "\n\n";

            for(auto& e : _exetutors) {
                strg += e->to_string() + "\n";
            }

            /*
            strg += sformat(_id, "%5d");
            strg += sformat((int)_minute_to_buy, "%10d");
            strg += sformat((double)_triggering_delta, "%13.5f");
            strg += sformat(orders_count(), "%10d");
            strg += " fitness: ";
            strg += sformat(get_fitness(), "%20.5f");
            strg += sformat(get_pnl(), "%20.5f");
            strg += sformat(get_max_drawdown() , "%20.5f");
            strg += sformat(get_account_balance(), "%20.5f");
            strg += sformat(get_absolute_low(), "%20.5f");
            strg += sformat(get_winning_trades_count(), "%20d");
            strg += sformat(get_loosing_trades_count(), "%20d");
            */
            strg + "\n=============================================================================\n";
            return strg;
        }
};
#endif // FREEMARGINMODEL_INCLUDED
