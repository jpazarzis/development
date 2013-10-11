// Optimizable.h
//
// Author        : John Pazarzis
// Creation date : Tue 08 Oct 2013 10:58:05 AM EDT
//
// Summary
//      Optimizable represents a chromosome consisting of a collection of
//      CloneableDoubles. 
//
// Notes
//
//
//

#ifndef OPTIMIZABLE_INCLUDED
#define OPTIMIZABLE_INCLUDED

#include "Object.h"
#include "CloneableDouble.h"
#include "std_include.h"

class Optimizable : virtual public Object
{
        std::vector<CloneableDouble*> _fields;
        double _fitness;
        double _roulette_probability;
        double _roulette_spread;

    public:

        Optimizable() : _fitness(0.0), _roulette_probability(0.0)  {}

        virtual ~Optimizable() {}

        virtual std::string values_headers_to_string() const 
        {
            std::string s;
            char buffer[1024];
            for(int i = 0; i < _fields.size(); ++i)
            {
                sprintf(buffer, "FIELD_%d", i);
                s+= sformat(buffer);    

                if(i <_fields.size() - 1)
                    s+= "\t";

            }
            return s;
        }

        virtual std::string values_to_string() const 
        {
            std::string s;
            for(int i = 0; i < _fields.size(); ++i)
            {
                s += sformat(_fields[i]->get_value());
                if(i <_fields.size() - 1)
                    s+= "\t";
            }
            return s;
        }

        
        virtual std::string to_string() const 
        {
            return "Optimizable";
        }

        std::string get_chromosome() const
        {
            std::string chromosome = "";
            for(register int i = 0; i < _fields.size(); ++i)
                chromosome += _fields[i]->to_binary_string();
            return chromosome;
        }

        void populate_from_chromosome(const std::string& chromosome)
        {
             int expected_length = 0;
             for(register int i = 0; i < _fields.size(); ++i)
                expected_length += _fields[i]->max_length();

            assert(expected_length == chromosome.length());

            std::string c = chromosome;

            for(register int i = 0; i < _fields.size(); ++i)
            {
                c = _fields[i] -> read_from_binary(c);
            }

            _fitness = 0.0;
            _roulette_probability = 0.0;
            _roulette_spread = 0.0;

        }

        virtual double get_fitness() const 
        { 
            return _fitness; 
        }

        virtual void set_fitness(double fitness) 
        { 
            assert(fitness >= 0);
            _fitness = fitness; 
        } 

        virtual void set_roulette_probability(double roulette_probability) 
        { 
            _roulette_probability = roulette_probability;
        }

        virtual double get_roulette_probability() const 
        { 
            return _roulette_probability;
        }

        virtual void set_roulette_spread(double roulette_spread) 
        { 
            _roulette_spread = roulette_spread; 
        }

        virtual double get_roulette_spread() const 
        { 
            return _roulette_spread;
        }

        static std::string  clone(const Optimizable* parent1, const Optimizable* parent2)
        {
            using namespace std;
            assert(parent1 != NULL);
            assert(parent2 != NULL);
            string c1 = parent1->get_chromosome();
            string c2 = parent2->get_chromosome();
            assert(c1.length() == c2.length());
            const int max = c2.length();
            const int cross_over_point =  rand() % max;
            string new_chromosome1;

            if(cross_over_point == 0)
                    new_chromosome1 = c1;
            else if (cross_over_point == max)
                    new_chromosome1 = c2;
            else
                new_chromosome1 = c1.substr(0,cross_over_point) + c2.substr(cross_over_point);

            mutate(new_chromosome1);
            return new_chromosome1;
        }

    protected:

        void add_optimizable_field(CloneableDouble* optimizable)
        {
            assert(optimizable != NULL);
            _fields.push_back(optimizable);
        }

    private:

        static void mutate(std::string& c)
        {
            for(register int i = 0; i < c.length(); ++i)
            {
                const double p = (double)rand()/(double)RAND_MAX;
                if(p <= 0.03)
                {
                    c[i] = c[i] == '0' ? '1' : '0';
                    return;
                }
            }
        }

        Optimizable(const Optimizable& other);        
        
        Optimizable& operator=(const Optimizable& other);
};

#endif // OPTIMIZABLE_INCLUDED
