// Scalar.h
//
// Author        : John Pazarzis
// Creation date : Wed 06 Nov 2013 09:05:29 PM EST
//
// Summary
//      Represents a single double named value to use in a genetic program

#ifndef SCALAR_INCLUDED
#define SCALAR_INCLUDED

#include "ComparableValue.h"
#include <memory>

class Scalar: public ComparableValue {
        VALUE _value;

    public:

        Scalar(CONST_STRING_REF name):
            ComparableValue(name)
        {
        }

        VALUE value() const
        {
            return _value;
        }

        void set(VALUE value)
        {
            _value = value;
        }

        void operator=(VALUE value)
        {
            _value = value;
        }
};

#endif // SCALAR_INCLUDED
