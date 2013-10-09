
// GeneticAlgorithm.h
//
// Author        : John Pazarzis
// Creation date : Wed 09 Oct 2013 02:58:31 PM EDT
//
// Summary
//      Summary goes here
//
// Notes
//

#ifndef GENETICALGORITHM_INCLUDED
#define GENETICALGORITHM_INCLUDED

#include "std_include.h"
#include "Statistics.h"
#include "Optimizable.h"

using namespace std;

bool compare_optimizable(const Optimizable* lph, const Optimizable* rph) 
{ 
    return lph->get_fitness() > rph->get_fitness() ; 
}


template <class T>
class GeneticAlgorithm
{
    public:

        typedef T* T_PTR;
    
        GeneticAlgorithm(int size) : _generation_index(0)
        {
            for(register int i = 0; i < size; ++i)
                _population.push_back(new T());
        }

        ~GeneticAlgorithm()
        {
            int size = _population.size();
            for(register int i = 0; i < size; ++i)
                delete _population[i];
        }

        T_PTR operator[](int index)
        {
            return _population[index];
        }

        int size() const
        {
            return _population.size();
        }

        bool evolve()
        {
            ++_generation_index;
            cout << endl << endl << " GENERATION # " << _generation_index << endl;
            calculate_fitness_stats();
            sort();
            double total_fitness = 0.0;
            int size = _population.size();
            for(register int i = 0; i < size; ++i)
                total_fitness += _population[i] -> get_fitness();

            for(register int i = 0; i < size; ++i)
                _population[i] -> set_roulette_probability(_population[i] -> get_fitness() / total_fitness);

            double t = 0.0;
            for(register int i = 0; i < size; ++i)
            {
               t += _population[i] -> get_fitness() / total_fitness;
                _population[i] -> set_roulette_spread(t);
            }

            cout << _population[0]->to_string() << endl;

            //for(register int i = 0; i < size; ++i)
            //   cout << _population[i]->get_chromosome() << " " << _population[i]-> get_roulette_probability() << " " << _population[i]-> get_fitness() << endl;

            std::vector<std::string> new_chromosomes;
            new_chromosomes.push_back(_population[0]->get_chromosome());

            while(new_chromosomes.size() < size)
            {
                const T_PTR t1 = select_randomly();
                const T_PTR t2 = select_randomly(t1);
                std::string new_chromosome = Optimizable::clone(t1, t2);
                if (std::find(new_chromosomes.begin(), new_chromosomes.end(), new_chromosome) == new_chromosomes.end())
                {
                    new_chromosomes.push_back(new_chromosome);
                }
            }

            assert(new_chromosomes.size() == _population.size());

            for(int i = 0; i < size; ++i)
                _population[i] -> populate_from_chromosome(new_chromosomes[i]);

            return false;
        }

    private:

        void sort()
        {
            std::sort(_population.begin(), _population.end(), compare_optimizable);
        }

        void calculate_fitness_stats()
        {
            const int size = _population.size();

            std::vector<double> fitness;
            for(int i = 0; i < size; ++i)
                fitness.push_back(_population[i]->get_fitness());

            Statistics stats(fitness);
            cout << "Fitness statistics: " << stats.to_string() << endl;
        }


        const T_PTR select_randomly(const T_PTR pExclude = NULL) const
        {
                 const double p = (double)rand()/(double)RAND_MAX;
                 int size = _population.size();
                 for(register int i = 0; i < size; ++i)
                 {
                        if( p <= _population[i] -> get_roulette_spread())
                        {
                            if(_population[i] == pExclude)
                            {
                                return select_randomly(pExclude);
                            }

                            return _population[i];
                        }
                 }   
                return NULL;
        }

        std::vector<T_PTR> _population;
        int _generation_index ;

};

#endif // GENETICALGORITHM_INCLUDED
