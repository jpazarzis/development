// Aggregate.h
//
// Author        : John Pazarzis
// Creation date : Wed 06 Nov 2013 09:07:36 PM EST
//
// Summary
//      Represents an array of doubles that can be used by a genetic program
//

#ifndef AGGREGATE_INCLUDED
#define AGGREGATE_INCLUDED

#include "ComparableValue.h"
#include "AggregateFunction.h"
#include "ScalarArray.h"

#include <memory>
#include <vector>

class Aggregate: public ComparableValue {
    const ScalarArray* _p_array;
    mutable AggregateFunction _function;
    protected:

        Aggregate(CONST_STRING_REF name, AggregateFunction function, ScalarArray* p_array):
            ComparableValue(name), _function(function), _p_array(p_array)
        {
        }

    public:


        Aggregate(AggregateFunction function, ScalarArray* p_array):
            ComparableValue(function.name()), _function(function), _p_array(p_array)
        {
        }


        VALUE value() const
        {
            return _function.call_it(_p_array);
        }
};

#endif // AGGREGATE_INCLUDED
