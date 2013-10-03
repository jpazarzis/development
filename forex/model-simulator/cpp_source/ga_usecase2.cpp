// ga_usecase2.cpp
//
//
#include "ga.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

using namespace std;

#define SAMPLE_DATA_SIZE 10

struct SampleData
{
    double x,y;
};

class Solver : public Populatebale<3>
{
        public:

                double a,b,c, fitness;

                virtual void populate(const Chromosome<3>& chromosome ) 
                {
                    a = chromosome._weight[0];
                    b = chromosome._weight[1];
                    c = chromosome._weight[2];
                }

                virtual double get_fitness() const 
                {
                    return fitness;
                }
        
                void calc_fitness(SampleData* p_sample_data)
                {
                    fitness = 0;
                    double t = 0.0;
                    for(int i = 0; i < SAMPLE_DATA_SIZE; ++i)
                    {
                        const double x = p_sample_data[i].x;
                        const double y = p_sample_data[i].y;
                        const double estimated_value = a * x *x + b * x + c;
                        t += fabs(estimated_value-y);
                    }

                    fitness = t ;
                }

};

#define NUMBER_OF_CHROMOSOMES 40

int main()
{
    srand ( time(NULL) );

    SampleData sd[SAMPLE_DATA_SIZE];

    sd[0].x = 0.1; sd[0].y = 22.232; sd[1].x = 0.2; sd[1].y = 23.358;
    sd[2].x = 0.3; sd[2].y = 24.688; sd[3].x = 0.4; sd[3].y = 26.222;
    sd[4].x = 0.5; sd[4].y = 27.96; sd[5].x = 0.6; sd[5].y = 29.902;
    sd[6].x = 0.7; sd[6].y = 32.048; sd[7].x = 0.8; sd[7].y = 34.398;
    sd[8].x = 0.9; sd[8].y = 36.952; sd[9].x = 1.0; sd[9].y = 39.71;
    sd[10].x = 1.1; sd[10].y = 42.672; sd[11].x = 1.2; sd[11].y = 45.838;
    sd[12].x = 1.3; sd[12].y = 49.208; sd[13].x = 1.4; sd[13].y = 52.782;
    sd[14].x = 1.5; sd[14].y = 56.56; sd[15].x = 1.6; sd[15].y = 60.542;
    sd[16].x = 1.7; sd[16].y = 64.728; sd[17].x = 1.8; sd[17].y = 69.118;
    sd[18].x = 1.9; sd[18].y = 73.712; sd[19].x = 2.0; sd[19].y = 78.51;
    sd[20].x = 2.1; sd[20].y = 83.512; sd[21].x = 2.2; sd[21].y = 88.718;
    sd[22].x = 2.3; sd[22].y = 94.128; sd[23].x = 2.4; sd[23].y = 99.742;
    sd[24].x = 2.5; sd[24].y = 105.56; sd[25].x = 2.6; sd[25].y = 111.582;
    sd[26].x = 2.7; sd[26].y = 117.808; sd[27].x = 2.8; sd[27].y = 124.238;
    sd[28].x = 2.9; sd[28].y = 130.872; sd[29].x = 3.0; sd[29].y = 137.71;
    sd[30].x = 3.1; sd[30].y = 144.752; sd[31].x = 3.2; sd[31].y = 151.998;
    sd[32].x = 3.3; sd[32].y = 159.448; sd[33].x = 3.4; sd[33].y = 167.102;
    sd[34].x = 3.5; sd[34].y = 174.96; sd[35].x = 3.6; sd[35].y = 183.022;
    sd[36].x = 3.7; sd[36].y = 191.288; sd[37].x = 3.8; sd[37].y = 199.758;
    sd[38].x = 3.9; sd[38].y = 208.432; sd[39].x = 4.0; sd[39].y = 217.31;
    sd[40].x = 4.1; sd[40].y = 226.392; sd[41].x = 4.2; sd[41].y = 235.678;
    sd[42].x = 4.3; sd[42].y = 245.168; sd[43].x = 4.4; sd[43].y = 254.862;
    sd[44].x = 4.5; sd[44].y = 264.76; sd[45].x = 4.6; sd[45].y = 274.862;
    sd[46].x = 4.7; sd[46].y = 285.168; sd[47].x = 4.8; sd[47].y = 295.678;
    sd[48].x = 4.9; sd[48].y = 306.392; sd[49].x = 5.0; sd[49].y = 317.31;
    sd[50].x = 5.1; sd[50].y = 328.432; sd[51].x = 5.2; sd[51].y = 339.758;
    sd[52].x = 5.3; sd[52].y = 351.288; sd[53].x = 5.4; sd[53].y = 363.022;
    sd[54].x = 5.5; sd[54].y = 374.96; sd[55].x = 5.6; sd[55].y = 387.102;
    sd[56].x = 5.7; sd[56].y = 399.448; sd[57].x = 5.8; sd[57].y = 411.998;
    sd[58].x = 5.9; sd[58].y = 424.752; sd[59].x = 6.0; sd[59].y = 437.71;
    sd[60].x = 6.1; sd[60].y = 450.872; sd[61].x = 6.2; sd[61].y = 464.238;
    sd[62].x = 6.3; sd[62].y = 477.808; sd[63].x = 6.4; sd[63].y = 491.582;
    sd[64].x = 6.5; sd[64].y = 505.56; sd[65].x = 6.6; sd[65].y = 519.742;
    sd[66].x = 6.7; sd[66].y = 534.128; sd[67].x = 6.8; sd[67].y = 548.718;
    sd[68].x = 6.9; sd[68].y = 563.512; sd[69].x = 7.0; sd[69].y = 578.51;
    sd[70].x = 7.1; sd[70].y = 593.712; sd[71].x = 7.2; sd[71].y = 609.118;
    sd[72].x = 7.3; sd[72].y = 624.728; sd[73].x = 7.4; sd[73].y = 640.542;
    sd[74].x = 7.5; sd[74].y = 656.56; sd[75].x = 7.6; sd[75].y = 672.782;
    sd[76].x = 7.7; sd[76].y = 689.208; sd[77].x = 7.8; sd[77].y = 705.838;
    sd[78].x = 7.9; sd[78].y = 722.672; sd[79].x = 8.0; sd[79].y = 739.71;
    sd[80].x = 8.1; sd[80].y = 756.952; sd[81].x = 8.2; sd[81].y = 774.398;
    sd[82].x = 8.3; sd[82].y = 792.048; sd[83].x = 8.4; sd[83].y = 809.902;
    sd[84].x = 8.5; sd[84].y = 827.96; sd[85].x = 8.6; sd[85].y = 846.222;
    sd[86].x = 8.7; sd[86].y = 864.688; sd[87].x = 8.8; sd[87].y = 883.358;
    sd[88].x = 8.9; sd[88].y = 902.232; sd[89].x = 9.0; sd[89].y = 921.31;
    sd[90].x = 9.1; sd[90].y = 940.592; sd[91].x = 9.2; sd[91].y = 960.078;
    sd[92].x = 9.3; sd[92].y = 979.768; sd[93].x = 9.4; sd[93].y = 999.662;
    sd[94].x = 9.5; sd[94].y = 1019.76; sd[95].x = 9.6; sd[95].y = 1040.062;
    sd[96].x = 9.7; sd[96].y = 1060.568; sd[97].x = 9.8; sd[97].y = 1081.278;
    sd[98].x = 9.9; sd[98].y = 1102.192; sd[99].x = 10.0; sd[99].y = 1123.31;

    const double A = 10.2;
    const double B = 8.2;
    const double C = 21.31;

    GeneticAlgorithm<NUMBER_OF_CHROMOSOMES,3, 10> ga;

    ga._min_max_values[0] = MinMaxValues(8.0,16.0);
    ga._min_max_values[1] = MinMaxValues(4.0,12.0);
    ga._min_max_values[2] = MinMaxValues(10.0,30.0);

    ga.randomize();

    Solver solver[NUMBER_OF_CHROMOSOMES];

    int reps = 0;
    for(;;)
    {
            ++reps;

            ga.exchange_data_from_array(solver);

            for(int i = 0; i < NUMBER_OF_CHROMOSOMES; ++i)
            {
                solver[i].calc_fitness(sd);
            } 

            ga.exchange_fitness(solver);

            //cout <<reps << " w: "<< ga.winner().to_string() << endl; 

            if(ga.winner()._fitness <= 0.3)
            {
                cout << "found the answer after: " << reps << " tries..." << endl;
                break;
            }
            
            ga.evolve();

            
    }

    Solver winner;
    ga.exchange_winner_data(&winner);

    cout <<"winner" << endl;
    cout << ga.winner().to_string() << endl; 

    cout << winner.a << " " <<winner.b << " " <<winner.c <<" " << endl; 

    return -1;
} 
