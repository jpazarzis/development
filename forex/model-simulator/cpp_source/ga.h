/////////////////////////////////////////////////////
// ga.h
// 
// Author: John Pazarzis
//
// Date  : Oct. 2 2013
//
// Summary: 
//    Implements a genetic algorithm to be used for model optimazation
//
// Notes:
//
////////////////////////////////////////////////////////////////////////

#ifndef GA_INCLUDED
#define GA_INCLUDED

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#include <string> 
using namespace std;

struct MinMaxValues
{
    double _min, _max;

    MinMaxValues(double min, double max) : _min(min), _max(max) {}

    MinMaxValues() {}

    double get_random_value() const
    {
        return (_max - _min) * ( (double)rand() / (double)RAND_MAX ) + _min;
    }
};
template <int CHROMOSOME_LENGTH>
struct Chromosome
{
    double _weight[CHROMOSOME_LENGTH];
    double _fitness;

    std::string to_string() const
    {
        std::string strg;
        char buffer[1024];
        for(int j = 0; j < CHROMOSOME_LENGTH; ++j)
        {
            sprintf (buffer, "%10.7f : ", _weight[j]);
            strg += std::string(buffer);
        }
        sprintf (buffer, "fit: %10.7f : ",  _fitness);
        strg += std::string(buffer);
        return strg;
    }

};

template <int CHROMOSOME_LENGTH> class Populatebale
{
    public:
        virtual void populate(const Chromosome<CHROMOSOME_LENGTH>&  chromosome) = 0;
        virtual double get_fitness() const = 0;
};

template <int CHROMOSOME_LENGTH>
int compare (const void * a, const void * b)
{
    Chromosome<CHROMOSOME_LENGTH>* p1 = (Chromosome<CHROMOSOME_LENGTH>*) a;
    Chromosome<CHROMOSOME_LENGTH>* p2 = (Chromosome<CHROMOSOME_LENGTH>*) b;

    if(p1->_fitness > p2->_fitness)
        return 1;
    else if(p1->_fitness < p2->_fitness)
        return -1;

    return 0;
}

template <int CHROMOSOME_LENGTH>
bool operator == (const Chromosome<CHROMOSOME_LENGTH>& lhp, const Chromosome<CHROMOSOME_LENGTH>& rhp)
{
    for(register int i = 0; i < CHROMOSOME_LENGTH; ++i)
    {
        if( fabs(lhp._weight[i] - rhp._weight[i]) > 0.0001)
        {
            return false;
        }
    }
    return true;
}

template <int POPULATIONSIZE, int CHROMOSOME_LENGTH, int ELITISM_FACTOR>
struct GeneticAlgorithm
{
    Chromosome<CHROMOSOME_LENGTH> _chromosome[POPULATIONSIZE];
    MinMaxValues _min_max_values[CHROMOSOME_LENGTH];

    void randomize()
    {
          for(register int i = 0; i < POPULATIONSIZE; ++i)
          {
                for(register int j = 0; j < CHROMOSOME_LENGTH; ++j)
                {
                     _chromosome[i]._weight[j] = _min_max_values[j].get_random_value();
                }

                _chromosome[i]._fitness = 0.0;
          }

    }

    int count_chromosomes() const { return POPULATIONSIZE;}

    int chromosome_length() const { return CHROMOSOME_LENGTH;}

    Chromosome<CHROMOSOME_LENGTH> winner()
    {
        sort();
        return _chromosome[0];
    }

    void mutate(int chromosome_index)
    {
        const int index = rand() % (CHROMOSOME_LENGTH);

        _chromosome[chromosome_index]._weight[index] = _min_max_values[index].get_random_value();
        _chromosome[chromosome_index]._fitness = 0.0;

    }

    template <class T>
    void exchange_data_from_array(T* data)
    {
        Populatebale<CHROMOSOME_LENGTH>* p[POPULATIONSIZE];
        for(int i = 0; i < POPULATIONSIZE; ++i)
        {

            Populatebale<CHROMOSOME_LENGTH>* p = (Populatebale<CHROMOSOME_LENGTH>*) & data[i];
            p->populate(_chromosome[i]);
        }
    }

    template <class T>
    void exchange_winner_data(T* data)
    {
        Populatebale<CHROMOSOME_LENGTH>* p = (Populatebale<CHROMOSOME_LENGTH>*)  data;
        p->populate(winner());
    }

    template <class T>
    void exchange_fitness(T* data)
    {
        Populatebale<CHROMOSOME_LENGTH>* p[POPULATIONSIZE];
        for(int i = 0; i < POPULATIONSIZE; ++i)
        {

            Populatebale<CHROMOSOME_LENGTH>* p = (Populatebale<CHROMOSOME_LENGTH>*) & data[i];
            _chromosome[i]._fitness = p->get_fitness();
        }
    }

    bool already_exists (int chromosome_index) const
    {
            for(register int j = 0; j < chromosome_index; ++j)
            {
                if(_chromosome[chromosome_index] == _chromosome[j])
                {
                    return true;
                }
            }
            return false;
     }       

    inline void sort()
    {
        qsort (_chromosome, POPULATIONSIZE, sizeof(Chromosome<CHROMOSOME_LENGTH>), compare<CHROMOSOME_LENGTH>); 
    }

    void evolve()
    {
        sort();

        for(register int i = ELITISM_FACTOR; i < POPULATIONSIZE; )
        {
            const int index1 = rand() % (ELITISM_FACTOR);
            const int index2 = rand() % (ELITISM_FACTOR);
            if(index1 == index2)
                continue;

            const int where_to_clone = (rand() % (CHROMOSOME_LENGTH - 2)) + 1;

            for(register int j = 0; j < CHROMOSOME_LENGTH; ++j)
            {

                if(j <= where_to_clone)
                {
                    _chromosome[i]._weight[j] = _chromosome[index1]._weight[j];
                }
                else
                {
                    _chromosome[i]._weight[j] = _chromosome[index2]._weight[j];
                }
            }
            // If the new chromosome already exists mutate 
            while(already_exists (i))
            {   
                mutate(i);
            }

            _chromosome[i]._fitness = 0.0;
             ++i;
        }

        for(register int i = ELITISM_FACTOR; i < POPULATIONSIZE; ++i)
        {
            if( (rand() % 100) >= 90 )
            {
                mutate(i);
                while(already_exists (i))
                {   
                        mutate(i);
                }
            }
        }

    }

    std::string to_string() const
    {
        std::string strg;
        char buffer[1024];
        for(int i = 0; i < POPULATIONSIZE; ++i)
        {
                const double* w = _chromosome[i]._weight;
                for(int j = 0; j < CHROMOSOME_LENGTH; ++j)
                {
                    sprintf (buffer, "%10.7f : ", w[j]);
                    strg += std::string(buffer);
                }
                sprintf (buffer, "fit: %10.7f : ",  _chromosome[i]._fitness);
                strg += std::string(buffer);
                strg += std::string("\n");
        }

        return strg;
    }
};

#endif


