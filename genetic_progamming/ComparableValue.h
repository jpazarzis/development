// ComparableValue.h
//
// Author        : John Pazarzis
// Creation date : Wed 06 Nov 2013 09:03:15 PM EST
//
// Summary
//      Base class for comparable values to use in a genetic program

#ifndef COMPARABLEVALUE_INCLUDED
#define COMPARABLEVALUE_INCLUDED
#include<string>
#include<memory>

using STRING = std::string;
using CONST_STRING = const std::string;
using CONST_STRING_REF = const std::string&;
using VALUE = double;


class ComparableValue {
        CONST_STRING _name;
    public:
        ComparableValue(CONST_STRING_REF name):
            _name(name)
        {
        }


        virtual ~ComparableValue()
        {
        }

        STRING name() const
        {
            return _name;
        }

        virtual VALUE value() const = 0;

        ComparableValue(const ComparableValue& other) = delete;

        ComparableValue& operator=(const ComparableValue& other) = delete;

};

#endif // COMPARABLEVALUE_INCLUDED
