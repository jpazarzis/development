#include "GenProEnvironment.h"
#include <iostream>
#include <string>
#include <vector>

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

using Operator = int;

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



void populate(GenProEnvironment* pGenProEnvironment)
{
    pGenProEnvironment->get_scalar("bid")->set(1.231);
    pGenProEnvironment->get_scalar("ask")->set(1.24);
    pGenProEnvironment->get_scalar("hour")->set(14);
    pGenProEnvironment->get_scalar("minute")->set(23);
    pGenProEnvironment->get_scalar("second")->set(18);

    pGenProEnvironment->get_array("price_curve")->clear();
    pGenProEnvironment->get_array("price_curve")->add(1.12);
    pGenProEnvironment->get_array("price_curve")->add(1.13);
    pGenProEnvironment->get_array("price_curve")->add(1.14);
    pGenProEnvironment->get_array("price_curve")->add(1.15);
    pGenProEnvironment->get_array("price_curve")->add(1.16);
}


class BinaryExpression
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
            //sprintf(buffer, "(%s %s %s )", operator_name[_operator].c_str(), _left->to_string().c_str(), _right->to_string().c_str() );
            return buffer;
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

int main()
{
    using namespace std;
    srand(time(NULL));

    auto AVERAGE = AggregateFunction("average",average);

    GenProEnvironment env;
    env.add_scalar("bid");
    env.add_scalar("ask");
    env.add_scalar("hour");
    env.add_scalar("minute");
    env.add_scalar("second");

    env.add_array("price_curve");

    env.add_aggregation_function("mimimum",mimimum);
    env.add_aggregation_function("maximun",maximun);
    env.add_aggregation_function("average",average);
    

    populate(&env);

    for(int i = 0; i < 1000; ++i)
    {
            auto s = env.get_random_scalar();
            cout << s->name() << " " << s->value() << endl;
            auto arr = env.get_random_array();
            cout << arr->name() << endl;
    }
//    auto min_price = new Aggregate(MINIMUM, price_curve);

}
