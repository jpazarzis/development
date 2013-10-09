// Statistics.h
//
// Author        : John Pazarzis
// Creation date : Wed 09 Oct 2013 03:00:54 PM EDT
//
// Summary
//      A utility class to calculate basic statistics for a provided collection
//      of values...
//

#ifndef STATISTICS_INCLUDED
#define STATISTICS_INCLUDED

#include "Object.h"
#include "std_include.h"

class Statistics : public Object
{
    double _mean;
    double _variance;
    double _stdev;
    double _median;

    public:

        double get_mean() const { return _mean;}
        double get_variance() const { return _variance;}
        double get_stdev() const { return _stdev;}
        double get_median() const { return _median;}

        Statistics(std::vector<double>& values)
        {
            double sum = 0;
            const int size = values.size();
            for(int i = 0; i < size; ++i)
                sum += values[i];
            _mean = sum / size;

            double temp = 0;
            for(int i = 0; i < size; i++)
            {
                 temp += (values[i] - _mean) * (values[i] - _mean) ;

            }
            _variance = temp / (size-1);
            _stdev = sqrt(_variance);

            sort(values.begin(), values.end());

            if (size  % 2 == 0)
            {
                  _median = (values[size / 2 - 1] + values[size / 2]) / 2;
            }
            else 
            {
                  _median = values[size / 2];
            }
        }

        virtual std::string to_string() const 
        {
            char buffer[1024];
            sprintf(buffer, "mean: %10.6f stdev: %10.6f median: %10.6f" , _mean, _stdev, _median);
            return buffer;
        }
        
};
#endif // STATISTICS_INCLUDED
