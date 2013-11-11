///////////////////////////////////////////////////////////////////////////////////
// bftd.cpp - Build Forcast Training Data
// Author: John Pazarzis, Mon Nov 11 12:31:52 EST 2013
//
//
// This program prepares the training data to be used by a FANN to optimize its
// output. 
// /////////////////////////////////////////////////////////////////////////////////
//
// NOTES
//
// Price Normalization
//
//      Applies to a vector of prices. For example the bid prices of all the
//      ticks of the day V = {p0, p1, ... pn}
//
//      If MAX = MAX(V) and MIN = MIN(V)
//
//      Then the normalized representation of each data point in V will be as
//      follows:
//
//      NORM[i] (V[i] - MIN) / (MAX-MIN);
//
//      The open - high - low - close of the normal data will look as follows:
//
//      NORM[0] - 1 - 0 - NORM[n]
//
//      Based on this we only need the normal open - close to have a full
//      description of what is called candle data (since high and low are always
//      1 and 0)
//      
///////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "toolkit.h"
#include "TickPool.h"

using namespace std;

using TICK_VEC = vector<Tick*>;
using CONST_TICK_VEC_REF = const TICK_VEC&;

struct CandleStick {
    double open, high, low, close;
};

void normalize_prices(CONST_TICK_VEC_REF ticks, vector<double>& prices, CandleStick& cs){
    prices.clear();
    double min = ticks[0]->bid(), max = ticks[0]->bid();
    for(auto& t: ticks){
        const double p = t->bid();
        if(p < min) min = p;
        if(p > max) max = p;
        prices.push_back(p);    
    }
    for(register int i =0; i < prices.size(); ++i){
        prices[i] = (prices[i] - min) / (max-min);
    }
    min = prices[0], max = prices[0];
    for(auto d: prices){
        if(d>max) max = d;
        if(d<min) min = d;
    }
    cs.open = prices[0];
    cs.close = prices[prices.size()-1];
    cs.high = max;
    cs.low = min;
}

void load_ticks() {
    TickPool& tp = TickPool::singleton();
    cout << timestamp() << endl;
    cout << "loading ticks..." << endl;
    auto from_date = make_date("20110101");
    auto to_date = make_date("20110601");
    tp.load("/home/john/projects/forex/historical-ticks/EUR_USD.csv", from_date, to_date);
    cout << timestamp() << endl;
    cout << tp.size() << endl;
}

void print_data(CONST_DATE_REF date, CONST_TICK_VEC_REF previous, CONST_TICK_VEC_REF today){
    if(previous.size() < 30000 || today.size()< 30000 ){
        return;
    }
    vector<double> prices;
    CandleStick cs;
    normalize_prices(previous, prices, cs);
    cout << date << " " << previous.size() << " " << today.size() << " " << 
         cs.open << " " << cs.high << " " << cs.low << " " << cs.close << endl;
}

int main(){
    load_ticks();
    TickPool& tp = TickPool::singleton();
    int current_day = -1;
    TICK_VEC previous_days_ticks, current_days_ticks;
    DATE current_date;
    for(int i=0; i < tp.size();++i){
        if(current_day != tp[i].timestamp().date().day()){
            if(previous_days_ticks.size()>0 && current_days_ticks.size()>0){
                print_data(current_date, previous_days_ticks, current_days_ticks);
            }
            previous_days_ticks = current_days_ticks;
            current_days_ticks.clear();
            current_day = (int)tp[i].timestamp().date().day();
            current_date = tp[i].timestamp().date();
        }
        current_days_ticks.push_back(&tp[i]);
    }
}
