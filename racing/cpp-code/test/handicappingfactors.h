// handicappingfactors.h

#ifndef HANDICAPPING_FACTORS_INCLUDED
#define HANDICAPPING_FACTORS_INCLUDED

#include "horse.h"
#include "pastperformance.h"
#include <string>
#include <iostream>

#define BUFFER_SIZE 1024

class HandicappingFactor {
    std::string _name;
    int _matches, _wins;
    double _amount_won;

    HandicappingFactor(const HandicappingFactor&);
    HandicappingFactor& operator=(const HandicappingFactor&);

    protected:
        virtual bool is_matching(const horse& starter) = 0;

    public:

        HandicappingFactor(const std::string& name):
            _name(name), _matches(0), _wins(0), _amount_won(0.0)
        {
        }
        
        std::string tostring() const {
            char buffer [BUFFER_SIZE];
            sprintf (buffer, "%s: matches %d", _name.c_str(), _matches);
            return buffer;
        }

        virtual void update(const horse& starter) {
            if(is_matching(starter)){
                ++_matches;
                // Is missing the part of winning since I do
                // not have access to the db right now            
            }
            
        }
};

class ComingOfALayoff: public HandicappingFactor {
    public:
        ComingOfALayoff(): HandicappingFactor("ComingOfALayoff") {
        }
    protected:
        bool is_matching(const horse& starter) {
            if (starter.count_past_performances() == 0)
                return false;

            const pastperformance& last_per = starter.get_past_performance(0);
            return last_per.days_since_last() >= 30;
        }
};

class BigWinnerLastOut: public HandicappingFactor {
    public:
        BigWinnerLastOut(): HandicappingFactor("BigWinnerLastOut") {
            }

    protected:
        bool is_matching(const horse& starter) {
            if (starter.count_past_performances() == 0)
                return false;

            const pastperformance& last_per = starter.get_past_performance(0);
            return last_per.finish_position() == 1 && last_per.finish_lengths() > 5;
        }
};

class WinnerLastOut: public HandicappingFactor {
    public:
        WinnerLastOut(): HandicappingFactor("WinnerLastOut") {
        }
    protected:
        bool is_matching(const horse& starter) {
            if (starter.count_past_performances() == 0)
                return false;

            const pastperformance& last_per = starter.get_past_performance(0);
            return last_per.finish_position() == 1;
        }
};

class RunSecondLastOut: public HandicappingFactor {
    public:
        RunSecondLastOut(): HandicappingFactor("RunSecondLastOut") {
        }
    protected:
        bool is_matching(const horse& starter) {
            if (starter.count_past_performances() == 0)
                return false;

            const pastperformance& last_per = starter.get_past_performance(0);
            return last_per.finish_position() == 2;
        }
};



#endif
