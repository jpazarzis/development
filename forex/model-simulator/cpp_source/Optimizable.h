// Optimizable.h
//
// Author        : John Pazarzis
// Creation date : Tue 08 Oct 2013 10:58:05 AM EDT
//
// Summary
//
//      Optimizable represents a chromosome consisting of a collection of
//      CloneableDoubles.
//
//      Should be used as a base class for any model that will later be
//      optimized by a genetic algorithm.
//
//      It holds the fields to optimize in a vector and can create new objects
//      based in their fitness.
//
//      The contained fields should be of CloneableDouble type since they will
//      be converted back and forth to binary representations
// Note
//      The term chromosome and Optimizable are used for the same purpose in
//      the documentation of this class
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

        // used for fancy printing
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

        // returns a string containing the values of the optimizable fields
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

        // standard override of the Object method
        virtual std::string to_string() const 
        {
            return "Optimizable";
        }

        // returns a binary string representing all the available fields
        std::string get_chromosome() const
        {
            std::string chromosome = "";
            for(register int i = 0; i < _fields.size(); ++i)
                chromosome += _fields[i]->to_binary_string();
            return chromosome;
        }

        // populates the available fields based in a binary string containing
        // their values
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

        // returns the fitness of the chromosome
        virtual double get_fitness() const 
        { 
            return _fitness; 
        }

        // sets the fitness of the chromosome
        virtual void set_fitness(double fitness) 
        { 
            _fitness = fitness; 
        } 

        // sets the roulette probability that will later be used for the random
        // selection of chromosomes for the crossover. Is set from the client
        // who has access to an array of chromosomes 
        virtual void set_roulette_probability(double roulette_probability) 
        { 
            _roulette_probability = roulette_probability;
        }

        // gets the roulette_probability that was assigned by the client, who
        // will use this probability to randomly select the chromosomes to clone
        virtual double get_roulette_probability() const 
        { 
            return _roulette_probability;
        }

        // sets the spread that will be used for the roulette selection
        virtual void set_roulette_spread(double roulette_spread) 
        { 
            _roulette_spread = roulette_spread; 
        }

        // gets the roulette_spread
        virtual double get_roulette_spread() const 
        { 
            return _roulette_spread;
        }

        // receives two chromosomes (that were randomly selected by the client)
        // and returns a binary string representing the new chromosome
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
