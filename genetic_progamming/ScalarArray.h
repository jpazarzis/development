// ScalarArray.h
//
// Author        : John Pazarzis
// Creation date : Thu 07 Nov 2013 08:20:02 AM EST
//
// Summary
//      Holds array data that will be presented to a genetic program
//

#ifndef SCALARARRAY_INCLUDED
#define SCALARARRAY_INCLUDED

#include "ComparableValue.h"
#include <vector>

class ScalarArray
{
        std::vector<VALUE> _values;    
        CONST_STRING _name;


    public:
        ScalarArray(CONST_STRING_REF name):
            _name(name)
        {
        }

        STRING name() const
        {
            return _name;
        }

        void add(VALUE value)
        {
            _values.push_back(value);
        }

        const std::vector<VALUE>& values() const
        {
            return _values; 
        }

        void clear()
        {
            _values.clear();
        }

        ScalarArray(const ScalarArray& other) = delete;

        ScalarArray& operator=(const ScalarArray& other) = delete;
};


#endif // SCALARARRAY_INCLUDED
