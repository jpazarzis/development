// Statistics.h
//
// Author        : John Pazarzis
// Creation date : Wed 09 Oct 2013 03:00:54 PM EDT
//
// Summary
//      A utility to calculate basic statistics for a provided collection
//      of values...
//

#ifndef STATISTICS_INCLUDED
#define STATISTICS_INCLUDED

#include "Object.h"
#include "std_include.h"
#include <numeric>


template <typename T> double mean(const std::vector<T>& v)
{
    return std::accumulate(v.begin(), v.end(), 0.0) / (v.size() * 1.0);
}

template <typename T> double variance(const std::vector<T>& v)
{
    const double m = mean(v);
    const int size = v.size();
    double temp = 0;
    for(int i = 0; i < size; i++)
        temp += pow(v[i] - m,2 );
   return temp / (size-1);
}

template <typename T> double stdev(const std::vector<T>& v)
{
    return sqrt(variance(v));
}

template <typename T> T median(std::vector<T> v)
{
    const int size = v.size();
    sort(v.begin(), v.end());
    if (size  % 2 == 0)
    {
          return (v[size / 2 - 1] + v[size / 2]) / 2;
    }
    else 
    {
          return v[size / 2];
    }
}


double max_drawdown(const std::vector<double>& v)
{
    const int size = v.size();

    if(size<=1)
        return 0;

    double maxdrawdown = 0.0, max_so_far = v[0];
    
    for(register int i = 1; i < size; ++i)
    {
        if(v[i] > max_so_far)
        {
            max_so_far = v[i];
        }
        else
        {
            if(max_so_far > 0)
            {
                const double dd = (v[i] - max_so_far) / max_so_far;
                if(dd < maxdrawdown)
                    maxdrawdown =dd;
            }
            
        }
    }
    return fabs(maxdrawdown);
}
#endif // STATISTICS_INCLUDED
