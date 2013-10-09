#include "std_include.h"
#include "GeneticAlgorithm.h"
#include "Statistics.h"
using namespace std;

#define SAMPLE_DATA_SIZE 5

struct SampleData
{
    double x,y;
};

class EquationSolver: public Optimizable
{
    private:
        CloneableDouble _a;
        CloneableDouble _b;

        void initialize()
        {
            add_optimizable_field(&_a);
            add_optimizable_field(&_b);
        }

    public:
        EquationSolver(): _a(0,15,6),_b(0,20,6)
        {
            initialize();
        }

        EquationSolver(const CloneableDouble& a,const CloneableDouble& b): _a (a),_b(b)
        {
            initialize();
        }


        virtual std::string to_string() const 
        {
            char buffer[1024];
            sprintf(buffer, "EquationSolver: Fitness: %10.6f a: %10.6f b: %10.6f  probability: %10.6f spread: %10.6f" , 
                                get_fitness(), 
                                _a.get_value(),
                                _b.get_value(),
                                 get_roulette_probability(),
                                 get_roulette_spread());
            return buffer;
        }

        void calculate_fitness(SampleData* sd)
        {
            std::vector<double> f;

            for(int i = 0; i < SAMPLE_DATA_SIZE; ++i)
            {
                double x = sd[i].x;
                double y = _a.get_value() * x * x + _b.get_value() *x ;
                f.push_back(fabs(y - sd[i].y));
            }
            Statistics s(f);
            set_fitness( 1.0 / s.get_mean());
        }
};

int main()
{
    srand ( time(NULL) );
    SampleData sd[10];

    sd[0].x = 7.78838828923; sd[0].y = 682.586503839;
    sd[1].x = 15.0370867784; sd[1].y = 2429.66669514;
    sd[2].x = 11.2544301191; sd[2].y = 1384.2407395;
    sd[3].x = 11.9935703253; sd[3].y = 1565.57371398;
    sd[4].x = 21.2817269471; sd[4].y = 4794.21155986;
    sd[5].x = 6.65220514797; sd[5].y = 505.916782186;
    sd[6].x = 12.9140264754; sd[6].y = 1806.97023114;
    sd[7].x = 9.59627817679; sd[7].y = 1017.99274048;
    sd[8].x = 20.5899704383; sd[8].y = 4493.09596062;
    sd[9].x = 5.73453848063; sd[9].y = 382.449517716;
    
    const double A = 10.2;
    const double B = 8.2;

    CloneableDouble a(0,15,6), b(0,10,6);
    a.read_from_double(A);
    b.read_from_double(B);

    EquationSolver desired_solution(a,b);
    desired_solution.calculate_fitness(sd);
    GeneticAlgorithm<EquationSolver> population(30);
    for(;;)
    {
        const int size = population.size();
        for(int i = 0; i < size; ++i)
        {
            
            population[i]->calculate_fitness(sd);
        }

        //cout << "desired_solution: " << desired_solution.get_chromosome() << " " << desired_solution.get_fitness() << endl;
        if (population.evolve())
            break;
    }
}
