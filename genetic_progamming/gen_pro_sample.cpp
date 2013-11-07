#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <memory>
#include <stdio.h>      
#include <stdlib.h>     

using namespace std;


enum 
{
    LESS = 0,
    MORE,
    EQUAL,
    AND,
    OR
};

std::vector<std::string> operator_name = { "LESS", "MORE", "EQUAL", "AND", "OR"};

#define BUFFER_SIZE 1024

using STRING = std::string;
using CONST_STRING = const std::string;
using CONST_STRING_REF = const std::string&;
using VALUE = double;


class Comparable
{
    CONST_STRING _name;
public:
    Comparable(CONST_STRING_REF name): 
        _name(name} 
   {
   }

    virtual ~Comparable()
    {
    }

    STRING get_name() const 
    {
        return _name;
    }

    VALUE value() const = 0;
};

class Scalar: public Comparable
{
    VALUE _value;

    public:

        Scalar(CONST_STRING_REF name):
            Comparable(name)
        {
        }

        VALUE value() const 
        {
            return _value;
        }

        void operator=(VALUE value)
        {
            _value = value;
        }

    
};



int main()
{
    srand ( time(NULL) );

    Scalar bid("bid");

    bid = 1.253;

    cout << bid.value() << endl;



}
