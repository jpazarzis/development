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

class Scalar;

std::vector<Scalar*> scalars;

using Operator = int;


class Expression
{
public:
    virtual double value() = 0;
    virtual bool evaluate() = 0;
    virtual std::string to_string() = 0;
};


class Scalar: public Expression
{
    double _value;
    const std::string _name;

    public:

        Scalar(const std::string& name):
            _name(name)
        {
        }


        virtual double value() 
        {
            return _value;
        }

        void operator=(double v)
        {
            _value = v;
        }

        virtual std::string to_string() 
        {
            return _name.c_str();
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "%10.6f",_value);
            return buffer;
        }

        virtual bool evaluate() 
        {
            throw "not supported";
        }
};

class Aggregate: public Expression
{
    std::vector<double> _value;

    function<double(const std::vector<double>&)> _foo;

    public:
        Aggregate(function<double(const std::vector<double>&)> foo):
            _foo(foo)
            {
            }

        virtual double value() 
        {
            return _foo(_value);
        }

        virtual std::string to_string() 
        {
            return "array";
        }


        void operator=(const std::vector<double>& v)
        {
            _value = v;
        }

        virtual bool evaluate() 
        {
            throw "not supported";
        }
};


class BinaryExpression: public Expression
{
    Operator _operator;
    Expression* _left;
    Expression* _right;
    public:

        BinaryExpression(Operator op, Expression* left, Expression* right):
            _operator(op), _left(left), _right(right)
        {
 
        }

        virtual std::string to_string() 
        {
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "(%s %s %s )", operator_name[_operator].c_str(), _left->to_string().c_str(), _right->to_string().c_str() );
            return buffer;
        }


        virtual double value() 
        {
            throw "not supported";
        }

        virtual bool evaluate() 
        {
            switch(_operator)
            {
                case LESS:
                    return _left->value() < _right->value();
                case MORE:
                    return _left->value() > _right->value();
                case EQUAL:
                    return fabs(_left->value() - _right->value()) <= 0.000001;
                case AND:
                    return _left->evaluate() && _right->evaluate();
                case OR:
                    return _left->evaluate() || _right->evaluate();
                default:
                    throw "Unknown operator";

            }
        }

};



double sum (const std::vector<double>& v)
{
    double s = 0;
    for(auto d: v)
        s += d;
    return s;
}

double mimimum (const std::vector<double>& v)
{
    double min = v[0];
    for(auto d: v)
        if(d < min) min = d;
    return min;
}

double maximun (const std::vector<double>& v)
{
    double max = v[0];
    for(auto d: v)
        if(d > max) max = d;
    return max;
}

double average (const std::vector<double>& v)
{
    return v.size() > 0 ? sum(v) / v.size() : 0.0;
}


Operator get_random_operator()
{
    return (Operator)rand() % 5;
}

Scalar* get_random_scalar()
{
    return scalars[rand() % scalars.size()];
}

Expression* create_random_expression()
{
    Operator op =  get_random_operator();
    switch(op)
    {
            case LESS:
            case MORE:
            case EQUAL:
                    return new BinaryExpression(op, get_random_scalar(), get_random_scalar());
            case AND:
            case OR:
                    return new BinaryExpression(op, create_random_expression(), create_random_expression());
    }
}

int main()
{
    srand ( time(NULL) );
    Scalar  current_minute("current_minute"), 
            minute_to_trade("minute_to_trade"), 
            open_price("open_price"), 
            current_price("current_price");

    Aggregate SUM(sum), MINIMUM(mimimum), MAXIMUM(maximun), 
              AVERAGE(average);


    scalars = { &current_minute, &minute_to_trade, &open_price, &current_price };

    for(auto s: scalars)
    {
        cout <<s->to_string() << endl;
    }
    

    BinaryExpression bex(EQUAL, &current_minute, &minute_to_trade);

    cout << bex.to_string() << endl;

    current_minute = 40;
    minute_to_trade = 40;

    open_price = 101.2;
    current_price = 102.2;

    if(bex.evaluate())
    {
        cout <<"Equal" << endl;
    }
    else
    {
        cout <<"NOT Equal" << endl;
    }

    BinaryExpression bex2(MORE, &current_price, &open_price);

    if(bex2.evaluate())
    {
        cout <<"More" << endl;
    }
    else
    {
        cout <<"Less" << endl;
    }

    BinaryExpression bex3(AND, &bex, &bex2);

    if(bex2.evaluate())
    {
        cout <<"BOTH TRUE" << endl;
    }
    else
    {
        cout <<"NOT" << endl;
    }


    Expression* ex = create_random_expression();

    cout << ex->to_string() << endl;


    if(ex->evaluate())
    {
        cout <<"TRUE" << endl;
    }
    else
    {
        cout <<"FALSE" << endl;
    }




}
