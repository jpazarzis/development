// ga_usecase.cpp
//
//


#include "cloner.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

using namespace std;

struct SampleData
{
    double x,y;
};

#define SOLVERS_COUNT 50

struct Solver
{
    DECLARE_OPTIMIZABLE_DOUBLE(a, 0.0, 100.0)
    DECLARE_OPTIMIZABLE_DOUBLE(b, 0.0, 100.0)
    DECLARE_OPTIMIZABLE_DOUBLE(c, 0.0, 100.0)

    double fitness = 0.0;

    Solver()
    {
        RANDOMIZE_VARIABLE(a)
        RANDOMIZE_VARIABLE(b)
        RANDOMIZE_VARIABLE(c)
    }   
};

BEGIN_EQUALITY_TEST(Solver)
    CHECK_EQUALITY_FOR_DOUBLE(a)
    CHECK_EQUALITY_FOR_DOUBLE(b)
    CHECK_EQUALITY_FOR_DOUBLE(c)
END_EQUALITY_TEST


void calc_fitness(Solver* p_solver, SampleData* p_sample_data, size_t length)
{
    p_solver->fitness = 0;

    double t = 0.0;
    for(int i = 0; i < length; ++i)
    {
        const double x = p_sample_data->x;
        const double y = p_sample_data->y;

        double estimated_value = p_solver->a * x *x + p_solver->b * x + p_solver->c;
        t += fabs(estimated_value);
        
    }

    p_solver->fitness += t / (1.0 * length);
}

int compare_solvers (const void * a, const void * b)
{
    Solver* p1 = *((Solver**) a);
    Solver* p2 = *((Solver**) b);
    //cout <<p1->fitness - p2->fitness << endl;
    
    if(fabs(p1->fitness) > fabs(p2->fitness))
        return 1;

    if(fabs(p1->fitness) < fabs(p2->fitness))
        return -1;
    
    return 0;
   
}

int main()
{
        srand ( time(NULL) );
        SampleData sd[100];

        sd[0].x = 0.1; sd[0].y = 22.232; sd[1].x = 0.2; sd[1].y = 23.358;
        sd[2].x = 0.3; sd[2].y = 24.688; sd[3].x = 0.4; sd[3].y = 26.222;
        sd[4].x = 0.5; sd[4].y = 27.96; sd[5].x = 0.6; sd[5].y = 29.902;
        sd[6].x = 0.7; sd[6].y = 32.048; sd[7].x = 0.8; sd[7].y = 34.398;
        sd[8].x = 0.9; sd[8].y = 36.952; sd[9].x = 1.0; sd[9].y = 39.71;

        double A = 10.2;
        double B = 8.2;
        double C = 21.31;

        
        Solver solvers[SOLVERS_COUNT];

        Solver* p_solvers[SOLVERS_COUNT];

        for(int i = 0; i < SOLVERS_COUNT; ++i)
        {
            p_solvers[i] = &solvers[i];
        }


   
        for(;;)
        {
                for(int i = 0; i < SOLVERS_COUNT; ++i)
                {
                    calc_fitness(p_solvers[i], sd, 10);
                }


                qsort (p_solvers, SOLVERS_COUNT, sizeof(Solver*), compare_solvers);

                cout << "====== after" << endl;

                for(int i = 0; i < SOLVERS_COUNT; ++i)
                {

                    cout << p_solvers[i]->a << " " <<  p_solvers[i]->b << " " << p_solvers[i]->c << " " << p_solvers[i] ->fitness << endl;
                }

                Solver* winner = p_solvers[0];

                cout << "winner so far: " << winner->a << " " <<  winner->b << " " << winner->c << " " << winner ->fitness << endl;

                if (!create_next_generation<Solver,SOLVERS_COUNT,10>(p_solvers))
                {
                    cout << "failed" << endl;
                    break;
                }

        } 

        

    
      
    return -1;
} 
