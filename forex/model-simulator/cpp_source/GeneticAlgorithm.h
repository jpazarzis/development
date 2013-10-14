// GeneticAlgorithm.h
//
// Author        : John Pazarzis
// Creation date : Wed 09 Oct 2013 02:58:31 PM EDT
//
// Summary
//
//     Implements a Genetic Algorithm.  Constructing a GeneticAlgorithm object
//     we need to provide the class that will be used as a place holder for the
//     parameters to optimize and the size of the population. This class should
//     derive from Optimizable which means that it will have a list of Cloneable
//     fields. The purpose of the GeneticAlgorithm class is to take these fields
//     and based in their fitnesses to create the next generation which will
//     continue until a solution is found...
//
// Notes
//     - For the algorithm to work the provided fitness has always to be a
//     positive value otherwise the roulete selection will fail. In other words
//     we always need to maximize fitness, so when you have a problem requiring
//     minimazation you need to workout a fitness algorithm reversing its
//     effect.
//
//     - The previous generation is kept in memory in a form of binary string -
//     corresponding fitness, so each time a cloning results to a population
//     with lesses fitness we roll back to the previous cloning again.

#ifndef GENETICALGORITHM_INCLUDED
#define GENETICALGORITHM_INCLUDED

#include "std_include.h"
#include "Statistics.h"
#include "Optimizable.h"

using namespace std;


// Compares two optimable objects using their fitness
bool compare_optimizable(const Optimizable* lph, const Optimizable* rph) 
{ 
    assert(lph != rph);
    return lph->get_fitness() > rph->get_fitness() ; 
}

// GeneticAlgorithm is defined as a template whos type needs to derive from the
// Optimazable class
template <class T>
class GeneticAlgorithm
{
    public:

        typedef T* T_PTR;
    
        // In the constructor we need to provide the size of the population
        // which is constant form the whole life span of the class. The provided
        // Optimazable class needs to have a public constructor as this class
        // will construct a vector of pointers to it
        GeneticAlgorithm(int size) : _generation_index(0), _last_productive_generation(0)
        {
            for(register int i = 0; i < size; ++i)
                _population.push_back(new T());
        }

        // The destructor is NOT VIRTUAL, so please do NOT DERIVE from this
        // class. A memory clean-up is performed here by deleting the objects
        // that were allocated in the constructor
        ~GeneticAlgorithm()
        {
            int size = _population.size();
            for(register int i = 0; i < size; ++i)
                delete _population[i];
        }

        // overides the [] operator providing an array-like behavior to the
        // class returning the contained chromosomes by index
        T_PTR operator[](int index)
        {
            return _population[index];
        }

        // returns the size of the population
        int size() const
        {
            return _population.size();
        }

        // the most interesting method. Based in the fitness that was provided
        // by the client code it is using roulete selection to create the new
        // generation who's fitness will next be calculated by the client
        bool evolve(bool print_best_chromosomes = false)
        {
            print_header_if_needed();
            ++_generation_index;
            bool rolled_back = false;
            if(current_average_fitness_is_less_than_previous())
            {  
                if(is_stucked())
                {
                    return true;
                }
                else
                {
                    roll_back_to_previous_generation();
                    rolled_back = true;
                }
            }
            store_current_generation();
            sort();
            
            if(!rolled_back)  
            {
                if(print_best_chromosomes)
                    print_all();
                log_best_chromosome();
                _last_productive_generation = _generation_index;
            }    
            assign_roulette_probabilities(get_total_fitness());
            create_new_population();
            return false;
        }

    private:

        bool current_average_fitness_is_less_than_previous()
        {
            double current_mean = calc_mean_fitness();
            return _generation_index > 1 && current_mean <= _previous_mean;
        }

        void print_all()
        {

            //std::cout  << BuyAtSpecificMinuteModel::printing_header()<< endl; 
            int size = _population.size();
            for(register int i = 0; i < size; ++i)
               std::cout  << _population[i]->to_string() << endl; 
        }

        void create_new_population()
        {
            int size = _population.size();
            std::vector<std::string> current_chromosomes;
            current_chromosomes.push_back(_population[0]->get_chromosome());

            while(current_chromosomes.size() < size)
            {
                const T_PTR t1 = select_randomly();
                const T_PTR t2 = select_randomly(t1);
                std::string new_chromosome = Optimizable::clone(t1, t2);
                if (std::find(current_chromosomes.begin(), current_chromosomes.end(), new_chromosome) == current_chromosomes.end())
                {
                    current_chromosomes.push_back(new_chromosome);
                }
            }

            assert(current_chromosomes.size() == _population.size());

            for(int i = 0; i < size; ++i)
            {
                _population[i] -> populate_from_chromosome(current_chromosomes[i]);
            }
        }

        void roll_back_to_previous_generation()
        {
            for(int i = 0; i < _previous_chromosomes.size(); ++i)
            {
                 _population[i] -> populate_from_chromosome(_previous_chromosomes[i]);
                 _population[i] -> set_fitness (_previous_fitness[i]);
             }
        }

        void store_current_generation()
        {
            _previous_chromosomes.clear();
            _previous_fitness.clear();
             for(register int i = 0; i < _population.size(); ++i)
             {
                _previous_chromosomes.push_back(_population[i]->get_chromosome());
                assert(_population[i]->get_fitness() >= 0);
                _previous_fitness.push_back(_population[i]->get_fitness());
             }
             _previous_mean = calc_mean_fitness();
             const int step = _generation_index - _last_productive_generation;
             _productive_generations_step.push_back(step);
        }

        void log_best_chromosome()
        {
            LOG << sformat(_generation_index) << "\t" 
                << sformat(_previous_mean) << "\t" 
                << sformat(_population[0]->get_fitness()) << "\t" 
                << _population[0]->values_to_string()<< EOL;
        }

        void verify_fitness()
        {
            for(register int i = 0; i < _population.size(); ++i)
            {
                assert(_population[i] ->get_fitness() >= 0);
            }
        }

        double get_total_fitness()
        {
            double total_fitness = 0.0;
            for(register int i = 0; i < _population.size(); ++i)
            {
                assert(_population[i] -> get_fitness() >= 0);
                total_fitness += _population[i] -> get_fitness();
            }
            return total_fitness;
        }

        void assign_roulette_probabilities(double total_fitness)
        {
            verify_fitness();

            for(register int i = 0; i < _population.size(); ++i)
                _population[i] -> set_roulette_probability(_population[i] -> get_fitness() / total_fitness);

            double t = 0.0;
            for(register int i = 0; i < _population.size(); ++i)
            {
               t += _population[i] -> get_fitness() / total_fitness;
                _population[i] -> set_roulette_spread(t);
            }
        }

        void sort()
        {
            std::sort(_population.begin(), _population.end(), compare_optimizable);
        }

        double calc_mean_fitness()
        {
            const int size = _population.size();
            std::vector<double> fitness;
            for(int i = 0; i < size; ++i)
            {
                assert(_population[i]->get_fitness() >= 0);
                fitness.push_back(_population[i]->get_fitness());
            }
            return mean(fitness);
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

        bool is_stucked() 
        {
                if(_productive_generations_step.size() >= 5)
                {

                    const double m = mean(_productive_generations_step);
                    const double sdev = stdev(_productive_generations_step);
                    
                    if( _generation_index - _last_productive_generation > m + 2 * sdev + 10)
                    {
                        return true;
                    }
                }

                return false;
        }


        void print_header_if_needed() 
        {
           if(0 == _generation_index)
            {
                LOG << sformat("#") << "\t" << sformat("avgfit") << "\t" << sformat("winfit") 
                    << "\t" << _population[0] -> values_headers_to_string() << EOL;    
            }
        }

        std::vector<T_PTR> _population;
        int _generation_index ;

        std::vector<std::string> _previous_chromosomes;
        std::vector<double> _previous_fitness;
        double _previous_mean;

        // Keep the generation ids that caused an improvement in fitness will be
        // later used to stop the evolution process..
        std::vector<int> _productive_generations_step;
        int _last_productive_generation;
};

#endif // GENETICALGORITHM_INCLUDED
