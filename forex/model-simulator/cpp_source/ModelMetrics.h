// ModelMetrics.h
//
// Author        : John Pazarzis
// Creation date : Fri 11 Oct 2013 11:16:32 AM EDT
//
// Summary
//      Holds the metrics of a model (like pnl, drop down etc)

#ifndef MODELMETRICS_INCLUDED
#define MODELMETRICS_INCLUDED

#include "Object.h"
#include <vector>
#include "Statistics.h"


class ModelMetrics
{   
    private:
        const double _pnl;
        const int _orders_count;
        const std::vector<double> _account_balance_curve;
        double _max_drawdown;
        double _unnormalized_fitness;

        ModelMetrics(double pnl, int orders_count, const std::vector<double>& account_balance_curve):
            _pnl(pnl), 
            _orders_count(orders_count), 
            _account_balance_curve(account_balance_curve),
            _max_drawdown(0),
            _unnormalized_fitness(0)

        {
        }

        friend class Model;

    public:

        ModelMetrics(const ModelMetrics& other);        
        
        ModelMetrics& operator=(const ModelMetrics& other);

        double pnl() const 
        {
            return _pnl;
        }

        double get_max_drawdown() const
        {
            return _max_drawdown;
        }

        double get_unnormalized_fitness() const
        {
            return _unnormalized_fitness;
        }
        
        int orders_count() const
        {
            return _orders_count;
        }

        double calc_unnormalized_fitness()
        {
            _max_drawdown = max_drawdown(_account_balance_curve);
            _unnormalized_fitness = _pnl / (_max_drawdown + 1.0);
            return _unnormalized_fitness;
        }
};


#endif // MODELMETRICS_INCLUDED
