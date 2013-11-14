// CandleStickCollection.h
//
// Author        : John Pazarzis
// Creation date : Tue 12 Nov 2013 01:46:55 PM EST
//
// Summary
//      Summary goes here

#ifndef CANDLESTICKCOLLECTION_INCLUDED
#define CANDLESTICKCOLLECTION_INCLUDED

#include "CandleStick.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <functional>
#include <assert.h>
using namespace std;

using CANDLE_STICK_VECTOR = std::vector<CandleStick>;
using NORMALIZER = std::function<CANDLE_STICK_VECTOR(const CANDLE_STICK_VECTOR&)>;

#define NORMALIZE(value,min,max) (value-min) / (max-min)

std::string to_string(const CANDLE_STICK_VECTOR& candle_sticks) {
    std::string strg;
    for(auto& cs: candle_sticks){
        strg += cs.to_string();
    }
    return strg;
}

std::string get_target_for_high_move(const CANDLE_STICK_VECTOR& candle_sticks, double delta) {
    if(candle_sticks.size() == 0){
        return "0";
    }
    const double target = candle_sticks[0].open() + delta;
    for(auto& cs: candle_sticks){
        if(cs.high() > target){
            return "1";
        }
    }
    return "0";
}


CANDLE_STICK_VECTOR trivial_normalizer(const CANDLE_STICK_VECTOR& candle_sticks){

    if(candle_sticks.size() <=1){
        return candle_sticks;
    }
    
    double min = candle_sticks[0].low(), max = candle_sticks[0].high();
    for(auto& candle_stick: candle_sticks){
        if(candle_stick.low() < min) {
            min = candle_stick.low();
        }

        if(candle_stick.high() > max) {
            max = candle_stick.high();
        }
    }
    assert(min < max);
    CANDLE_STICK_VECTOR normalized;
    for(auto& cs: candle_sticks){
        normalized.push_back(CandleStick( NORMALIZE(cs.open(), min,max),
                                          NORMALIZE(cs.high(), min,max),
                                          NORMALIZE(cs.close(), min,max),
                                          NORMALIZE(cs.low(), min,max)
                                    ));
    }
    assert(normalized.size() == candle_sticks.size());
    return normalized;
}

class CandleStickCollection final {
    private:
        CANDLE_STICK_VECTOR _candle_sticks;

    public:

        CandleStickCollection(const std::string& filename) {
            load(filename);
        }

        CandleStickCollection() {
        }

        ~CandleStickCollection(){
        }

        int size() const {
            return _candle_sticks.size();
        }

        CANDLE_STICK_VECTOR get_window( int i0, 
                                        int i1,
                                        NORMALIZER normalizer = nullptr ) const{
            assert(i0 <= i1);
            assert(i0 >= 0);
            assert(i1 < _candle_sticks.size());
            CANDLE_STICK_VECTOR v;
            for(int i = i0; i < i1; ++i){
                v.push_back(_candle_sticks[i]);
            }
            return normalizer != nullptr ? normalizer(v) : v;
        }

        void load(const std::string& filename){
            _candle_sticks.clear();
            std::ifstream myfile (filename.c_str());
            std::string line;
            while ( getline (myfile,line)) {
                std::vector<std::string> strs;
                boost::split(strs, line, boost::is_any_of(","));
                assert(strs.size() == 6);
                
                const double open = atof(strs[2].c_str());
                const double high = atof(strs[3].c_str());
                const double close = atof(strs[4].c_str());
                const double low = atof(strs[5].c_str());

                assert(open <= high);
                assert(open >= low);
                assert(close <= high);
                assert(close >= low);

               _candle_sticks.push_back(CandleStick(open, high, close, low)); 
             }
             myfile.close();
        }

        CandleStickCollection(const CandleStickCollection& other) = delete;        
        
        CandleStickCollection& operator=(const CandleStickCollection& other)  = delete;
};

#endif // CANDLESTICKCOLLECTION_INCLUDED
