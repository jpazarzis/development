// FitnessCalculators.h
//
// Author        : John Pazarzis
// Creation date : Fri 11 Oct 2013 11:16:32 AM EDT
//
// Summary
//      Holds the metrics of a model (like pnl, drop down etc)

#ifndef FITNESS_CALCULATORS_INCLUDED
#define FITNESS_CALCULATORS_INCLUDED

#include <vector>
#include "Statistics.h"


class FitnessCalculators
{   
    public:

        static double calc_fitness( double account_balance, 
                                    const std::vector<double>& account_balance_curve)
        {
            return account_balance / (max_drawdown(account_balance_curve) + 1.0);
        }
};


#endif // FITNESS_CALCULATORS_INCLUDED
