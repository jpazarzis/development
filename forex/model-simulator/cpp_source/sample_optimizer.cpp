#include "TickEngine.h"
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Order.h"
#include "BuyAtSpecificMinuteModel.h"
#include "Logger.h"
using namespace std;



#define NUMBER_OF_MODELS 50
#define OPTIMIZABLE_FIELDS_COUNT 4

void run_models(BuyAtSpecificMinuteModel* models)
{
    TickEngine te;

    for (int i = 0; i <NUMBER_OF_MODELS; ++i)
    {
        models[i].bind_tick_engine(&te);
    }   

    
    te.start("../../historical-ticks/EUR_USD.csv", 10000000);
    Order::clear_order_pool();

}

/*
void one_pass()
{
    BuyAtSpecificMinuteModel model;
    model._minute_to_buy = 40;
    model._triggering_delta = 0.001;
    model._stop_loss = 0.001; 
    model._take_profit= 0.001;


    TickEngine te;
    
    for (int i = 0; i <NUMBER_OF_MODELS; ++i)
    {
        model.bind_tick_engine(&te);
    }   
    
    te.start("../../historical-ticks/EUR_USD.csv", 1000000);
    cout << "number of orders created: " << Order::orders_count() << endl;

    cout <<"fitness: " << model.get_fitness() << endl;

    Order::clear_order_pool();
}
*/



int main()
{ 
    time_t current_time;
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;

    srand ( time(NULL) );


    BuyAtSpecificMinuteModel models[NUMBER_OF_MODELS];
    GeneticAlgorithm<NUMBER_OF_MODELS, OPTIMIZABLE_FIELDS_COUNT, 8 , MAXIMIZE_FITNESS> ga;

    ga._min_max_values[0] = MinMaxValues(30,50);
    ga._min_max_values[1] = MinMaxValues(6.0, 26.0);
    ga._min_max_values[2] = MinMaxValues(6.0, 26.0);
    ga._min_max_values[3] = MinMaxValues(6.0, 26.0);

    ga.randomize();

    
    for(int reps = 0; reps < 100;++reps)
    {
            cout << reps << endl;
            ga.exchange_data_from_array(models);
            run_models(models);

            LOG << "+++++++++" << EOL;
            for(int i = 0; i < NUMBER_OF_MODELS; ++i)
            {
                LOG << models[i].to_string() << EOL;
            }
            LOG << "+++++++++" << EOL;

            ga.exchange_fitness(models);
            
            LOG << "WINNER SO FAR" << EOL << EOL << ga.winner().to_string() << EOL;

            
            ga.evolve();
            
    }

    cout << "done" << endl;
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;


    return 0;
}
