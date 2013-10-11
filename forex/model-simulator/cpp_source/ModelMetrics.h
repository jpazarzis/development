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


class ModelMetrics
{   
    private:
        const double _pnl;
        const int _orders_count;
        const std::vector<double> _pnl_curve;

        ModelMetrics(double pnl, int orders_count, const std::vector<double>& pnl_curve):
            _pnl(pnl), _orders_count(orders_count), _pnl_curve(pnl_curve)
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
        
        int orders_count() const
        {
            return _orders_count;
        }

        double get_fitness() const
        {
            return _pnl;
        }
};


#endif // MODELMETRICS_INCLUDED
