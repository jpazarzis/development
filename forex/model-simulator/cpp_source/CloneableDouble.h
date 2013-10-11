// CloneableDouble.h
//
// Author        : John Pazarzis
// Creation date : Tue 08 Oct 2013 10:57:03 AM EDT
//
// Summary
//      Holds a double that can be used by a genetic algorithm. 
//      It provides the necessary functionality to export the double value as a
//      binary string and also can receive its value from a binary string. Note
//      that internally the double value is converted to a long using the
//      specified decimal point precession. 
//
// Notes
//

#ifndef CLONEABLEFIELD_INCLUDED
#define CLONEABLEFIELD_INCLUDED

#include "Object.h"
typedef unsigned long ULONG;

class CloneableDouble : virtual public Object
{
    public:

        // This constructor provides the min and max values to be used for the random
        // creation and the number of decimal to use. 
        //
        // It is important to know that both min and max should be positive as this
        // class does not handle the sign bit.  
        CloneableDouble(double min, double max, int decimals=2);

        virtual ~CloneableDouble();

        // Overrides the Object to_string() providing descriptive data about the object.
        // Useful for debugging and logging purposes
        virtual std::string to_string() const;

        // Returns the double value that can either be assigned by the constructor
        // (randomly), directly by the user passing a double value or by the genetic
        // algorithm passing a binary string. 
        double get_value() const;

        // For convinience overload the behavior of the double and int operator..
        operator double() const;

        operator int() const;

        // Returns the double value as a binary string. 
        std::string to_binary_string() const;

        // The most interesting function. It receives a binary string, which can be of
        // any length, containing many more encoded values. This class knows the
        // expected length that is needed to be initialized properly and it reads it
        // from the string populating the underlined double value. 
        // 
        // It returns a new string containing the remainder of the provided one, so the
        // process can continue for other values.
        std::string read_from_binary(const std::string& bin_string);

        // Sets the double values. Mostly used for debugging purposes
        void read_from_double(double v);

        // Returns the maximum length of the encoded binary value. Note that both the
        // min and the max values are used for the calculation of the maximum length.
        // For example if we provide min: 10000 and max: 10,500 the maximum length will
        // be sufficient only for the delta (500).
        int max_length() const;

        // Implemented only for debugging purposes
        CloneableDouble(const CloneableDouble& other);        

        // Not implemented, prohibits assignments
        CloneableDouble& operator=(const CloneableDouble& other);       

    private:
        const double _min;
        const double _max;
        const int _decimals;
        const int _max_length;
        ULONG _long_value;
};

// converts a string of bits to a long
extern ULONG bits_to_long(const std::string& bits);

// converts a long to a string of bits 
extern std::string long_to_bits(ULONG l);

#endif // CLONEABLEFIELD_INCLUDED
