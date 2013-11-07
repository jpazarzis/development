// GenProEnvironment.h
//
// Author        : John Pazarzis
// Creation date : Thu 07 Nov 2013 10:43:51 AM EST
//
// Summary
//      Contains all the data and functions to be used by a genetic program
//

#ifndef GENPROENVIRONMENT_INCLUDED
#define GENPROENVIRONMENT_INCLUDED

#include "ComparableValue.h"
#include "ScalarArray.h"
#include "AggregateFunction.h"
#include "Scalar.h"
#include "Aggregate.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdio.h>


class GenProEnvironment final
{
    std::map<std::string, Scalar*> _scalars;
    std::map<std::string, ScalarArray*> _arrays;
    std::map<std::string, AggregateFunction*> _aggregation_functions;

    public:

        GenProEnvironment()
        {
        }

        ~GenProEnvironment()
        {
            for (auto& k : _scalars ) 
            {
                delete k.second;
            }

            for (auto& k : _arrays ) 
            {
                delete k.second;
            }

            for (auto& k : _aggregation_functions ) 
            {
                delete k.second;
            }
        }

        void add_scalar(CONST_STRING_REF name)
        {
            if(_scalars.find(name) == _scalars.end())
            {
                _scalars[name] = new Scalar(name);
            }
        }

        void add_array(CONST_STRING_REF name)
        {
            if(_arrays.find(name) == _arrays.end())
            {
                _arrays[name] = new ScalarArray(name);
            }
        }

        void add_aggregation_function(CONST_STRING_REF name, AGGREGATE_FUNCTION func)
        {
            if(_aggregation_functions.find(name) == _aggregation_functions.end())
            {
                _aggregation_functions[name] = new AggregateFunction(name,func);
            }
        }

        Scalar* get_scalar(CONST_STRING_REF name)
        {
            return _scalars[name];
        }

        ScalarArray* get_array(CONST_STRING_REF name)
        {
            return _arrays[name];
        }

        AggregateFunction* get_aggregation_function(CONST_STRING_REF name)
        {
            return _aggregation_functions[name];
        }

        Scalar* get_random_scalar()
        {
            auto it = _scalars.begin();
            advance(it, rand() % _scalars.size());
            return it->second;
        }

        ScalarArray* get_random_array()
        {
            auto it = _arrays.begin();
            advance(it, rand() % _arrays.size());
            return it->second;
        }

        AggregateFunction* get_random_aggregate_function()
        {
            auto it = _aggregation_functions.begin();
            advance(it, rand() % _aggregation_functions.size());
            return it->second;
        }

        GenProEnvironment(const GenProEnvironment&) = delete;

        GenProEnvironment& operator=(const GenProEnvironment&) = delete;
};

#endif // GENPROENVIRONMENT_INCLUDED
