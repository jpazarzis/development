#ifndef MODELS_INCLUDED
#define MODELS_INCLUDED
#define MODELS_INCLUDED

/////////////////////////////////////////////////////
// BuyBasedInNN.h
//
// Author: John Pazarzis
//
// Date  :  Mon Nov 18 09:20:56 EST 2013
//
// Summary:
//    Receives a buy singal from a neural network and executes flat trading
//

#include "CandleStick.h"
#include "CandleStickCollection.h"
#include "TickPool.h"
#include "toolkit.h"
#include <floatfann.h>
#include <fann.h>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "std_include.h"

/*
correct_ones: 134
wrong_ones: 12
 * */

class BuyBasedInNN
{
    // we need to keep the last 24 hours in memory so we can each of them as a
    // candle stick to create the input for the neural networ

    std::vector<CandleStick> _history;

    bool get_signal(){
        assert(_history.size() == 24);

       LOG << EOL << "-------------------------------" << EOL; 
       for(auto& cs: _history){
            LOG << cs.timestamp() << " ";
            LOG << cs.open() << " ";
            LOG << cs.high() << " ";
            LOG << cs.close() << " ";
            LOG << cs.low() << " ";
        }
        LOG << EOL << "-------------------------------" << EOL;


        auto normalized_history = trivial_normalizer(_history);
        const string neural_network_file = "trained_nn.net";
        fann_type *calc_out;
        fann_type input[96]; // change this to be dynamic instead of hardcoded
        struct fann *ann = fann_create_from_file(neural_network_file.c_str());
        int i = 0;
        for(auto& cs: normalized_history){
            input[i++] = cs.open();
            input[i++] = cs.high();
            input[i++] = cs.close();
            input[i++] = cs.low();
            LOG << cs.timestamp() << " ";
            LOG << cs.open() << " ";
            LOG << cs.high() << " ";
            LOG << cs.close() << " ";
            LOG << cs.low() << " ";
        }
        assert(i == 96);
        LOG << EOL << "-------------------------------" << EOL;
        calc_out = fann_run(ann, input);
        double d = (double)calc_out[0];
        LOG << d << EOL;
        fann_destroy(ann);
        //std::cout << d << std::endl;
        return d > 0.5;
    }


    void export_balance_curve(const std::string& filename, const std::vector<double> bc) {
            using namespace std;

            ofstream balance_file;
            balance_file.open(filename.c_str());
            for(auto b:bc){
                balance_file << b << endl;
            }
            balance_file.close();
        }

    double execute_trading(int index, double lots){
        using namespace std;

        TickPool& tp = TickPool::singleton();
        const double delta = 0.003;
        double buy_price = tp[index].ask();


        double sell_price = -1;
        auto t = tp[index].timestamp();
        auto expiration =  DATE_TIME(t + boost::posix_time::hours(24));
        ++index;

        if(index >= tp.size()){
            return 0.0;
        }
        
        for(;;){
            const Tick& current_tick = tp[index];
            if(current_tick.bid() - buy_price > delta){
               sell_price = current_tick.bid();
               break;
            }

            if(current_tick.timestamp() >= expiration){
                sell_price = current_tick.bid();
               break;
            }

            //if ((current_tick.bid() - buy_price) * 100000.0 < -00){
            if (current_tick.bid() - buy_price  < -0.0011){
               sell_price = current_tick.bid();
               break;

            }

            ++index;
            if(index >= tp.size()){
                sell_price = current_tick.bid();    
                break;
            }
        }

        return (sell_price - buy_price) * 100000.0 * lots; 
    }
    
    public:

/***********
        void calculate_fitness2() {
            TickPool& tp = TickPool::singleton();
            const int number_of_ticks = tp.size();
            std::vector<CandleStick> history;
            int hour = -1;
            double open = -1, high= -1, low= -1, close= -1;
            string timestamp;

            for(register int i = 0; i < number_of_ticks; ++i) {
                const Tick& tick = tp[i];
                const int current_hour = (int)tick.timestamp().time_of_day().hours();
                if(current_hour != hour){
                    if(open > 0){
                        history.push_back(CandleStick(timestamp, open, high, low, close));
                    }
                    open = high = low = close = tick.bid();
                    timestamp = to_string(tick.timestamp());
                    current_hour = hour;
                }
                else {
                    if(high < tick.bid()){
                        high = tick.bid();
                    }
                    if(low > tick.bid()){
                        low = tick.bid();
                    }
                    close = tick.bid();
                }

                if(history.size() == 24){ // OK, we have enough historical data to try to get a signal

                    
                }

        
            }
 */
        void calculate_fitness() {
            using namespace std;
            TickPool& tp = TickPool::singleton();
            const int number_of_ticks = tp.size();
            
            int current_hour = -1;
            int current_day = -1;

            double open, high, low, close; // for current hour
            string timestamp;
            bool is_first_hour = true;

            double pnl = 0.0;

            //int wins = 0, loses = 0;

            vector<double> wins;
            vector<double> loses;

            const double start_balance = 100000;
            double balance = start_balance;
            double min_balance = balance;
            double max_balance = balance;
            std::vector<double> bc;

            int index = 0;
            for(; index <number_of_ticks; ++index){
                const Tick& tick = tp[index];
                int hour = (int)tick.timestamp().time_of_day().hours();
                if(hour == 8){
                    break;
                }
            }



            for(register int i = index; i < number_of_ticks; ++i)
            {
                const Tick& tick = tp[i];
                int hour = (int)tick.timestamp().time_of_day().hours();
                if(hour != current_hour) {
                    if(_history.size() == 24) {
                        if(get_signal()){
                            double lots = (balance / start_balance) ;
                            double r = execute_trading(i,lots);
                            pnl += r;
                            balance += r;
                            if(r>0) wins.push_back(r);
                            if(r<0) loses.push_back(r);
                            if(balance > max_balance){ max_balance = balance; }
                            if(balance < min_balance){ min_balance = balance; }
                            bc.push_back(balance);
                            if(balance <= 0)
                                break;
                        }

                        //_history.erase(_history.begin());
                        //assert(_history.size() == 23);
                        _history.clear();
                        is_first_hour = true;
                        current_hour = -1;

                    }
                    if(!is_first_hour){
                        _history.push_back(CandleStick(timestamp, open, high, close, low));
                    }
                    is_first_hour = false;
                    open = high =  low = close = tick.bid();
                    timestamp = format_datetime(tick.timestamp());
                    current_hour = hour;
                }
                else {
                    if(high < tick.bid()){
                        high = tick.bid();
                    }
                    if(low > tick.bid()){
                        low = tick.bid();
                    }
                    close = tick.bid();
                }
            }

            export_balance_curve("balance_curve.csv", bc);

            cout << "pnl: " << pnl << endl;
            cout << "balance: " << balance << endl;
            cout << "balance: " << balance << endl;
            cout << "min_balance: " << min_balance << endl;
            cout << "max_balance: " << max_balance << endl;
            
            cout << "wins: " << wins.size() << " " << mean(wins) << endl;
            cout << "loses: " << loses.size() << " " << mean(loses) << endl;


            
            
        }
};

#endif
