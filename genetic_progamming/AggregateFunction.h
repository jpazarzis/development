// AggregateFunction.h
//
// Author        : John Pazarzis
// Creation date : Thu 07 Nov 2013 08:22:10 AM EST
//
// Summary
//      Wraps an 'aggregate' function in an object

#ifndef AGGREGATEFUNCTION_INCLUDED
#define AGGREGATEFUNCTION_INCLUDED

#include "ComparableValue.h"
#include "ScalarArray.h"


using AGGREGATE_FUNCTION = std::function<VALUE(const std::vector<VALUE>& )>;

class AggregateFunction
{
        CONST_STRING _name;
        AGGREGATE_FUNCTION _func;

    public:
        AggregateFunction(CONST_STRING_REF name, AGGREGATE_FUNCTION func):
            _name(name), _func(func)
        {
        }

        VALUE call_it(const ScalarArray* p_array) 
        {
            return _func(p_array->values());
        }

        STRING name() const
        {
            return _name;
        }
};

#endif // AGGREGATEFUNCTION_INCLUDED
