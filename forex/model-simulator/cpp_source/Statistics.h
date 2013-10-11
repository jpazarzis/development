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

        void calculate(std::vector<double>& values_in)
        {


            std::vector<double> values;
            for(int i = 0; i < values_in.size(); ++i)
            {   
                if(values_in[i] > INVALID_FITNESS)
                {
                    values.push_back(values_in[i]);
                }
            }

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

    public:

        double get_mean() const { return _mean;}
        double get_variance() const { return _variance;}
        double get_stdev() const { return _stdev;}
        double get_median() const { return _median;}

        Statistics()
        {
                _mean = 0;
                _variance = 0;
                _stdev = 0;
                _median = 0;
        }

        Statistics(const Statistics& other):
            _mean(other._mean),
            _variance(other._variance),
            _stdev(other._stdev),
            _median(other._median)
            
        {
        }

        Statistics& operator=(const Statistics& other)
        {
            if(this != &other)
            {
                _mean = other._mean;
                _variance = other._variance;
                _stdev = other._stdev;
                _median = other._median;
            }
            return *this;
        }

        Statistics(std::vector<double>& values)
        {
            calculate(values);
        }

        Statistics(std::vector<int>& values)
        {
            std::vector<double> dvalues;
            for(int i = 0; i < values.size(); ++i)
                dvalues.push_back(values[i]);
            calculate(dvalues);
        }


        virtual std::string to_string() const 
        {
            char buffer[1024];
            sprintf(buffer, "%10.3f\t%10.3f\t%10.3f" , _mean, _stdev, _median);
            return buffer;
        }
        
};
#endif // STATISTICS_INCLUDED
