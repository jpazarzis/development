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
#include <memory>
#include <vector>

using AGGREGATE_FUNCTION = std::function<VALUE(const std::vector<VALUE>& )>;

class Aggregate: public ComparableValue {
    std::vector<VALUE> _values;
    AGGREGATE_FUNCTION _function;
    protected:

        Aggregate(CONST_STRING_REF name, AGGREGATE_FUNCTION function):
            ComparableValue(name), _function(function)
        {
        }
    public:
        static std::unique_ptr<Aggregate> make(CONST_STRING_REF name, AGGREGATE_FUNCTION function)
        {
            return std::unique_ptr<Aggregate>(new Aggregate(name,function));
        }

        void clear()
        {
            _values.clear();
        }

        void push_back(VALUE value)
        {
            _values.push_back(value);
        }

        VALUE value() const
        {
            return 0;
        }
};

#endif // AGGREGATE_INCLUDED
