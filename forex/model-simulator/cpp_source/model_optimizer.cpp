#include "TickEngine.h"
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Order.h"
#include "BuyAtSpecificMinuteModel.h"
#include "Logger.h"
#include "Identifiable.h"
#include <fstream>
using namespace std;




void calculate_fitness_for_models(GeneticAlgorithm<BuyAtSpecificMinuteModel>& ga)
{
    TickEngine te;

    for (int i = 0; i< ga.size(); ++i)
    {
        ga[i]->bind_tick_engine(&te);
    }   

    te.start("../../../historical-ticks/EUR_USD.csv", 100000);

    for (int i = 0; i <ga.size(); ++i)
    {
        ga[i]->assign_fitness();
    }

    

    for (int i = 0; i <ga.size(); ++i)
    {
        cout << ga[i]->to_string() << endl;
         ga[i]->clear();
    }





}

int main()
{ 
    time_t current_time;
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;
    srand ( time(NULL) );

    GeneticAlgorithm<BuyAtSpecificMinuteModel> ga(200);

    int i = 0;
    for(;;)
    {
        cout << "generation: " << ++i << endl;
        calculate_fitness_for_models(ga);
        if (ga.evolve())
            break;
        Order::clear_order_pool();
    }

    cout << "done" << endl;
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;

    return 0;
}


/*
void backtest()
{
    BuyAtSpecificMinuteModel model(38.7475331 , 30.1929909 ,10.4478687 ,17.8370198 );
    TickEngine te;
    model.bind_tick_engine(&te);
    te.start("../../historical-ticks/EUR_USD.csv");
    cout << model.to_string() << endl;
    Order::clear_order_pool();
}


void dump_generation(int generation, BuyAtSpecificMinuteModel* models)
{
    char filename[1024];
    sprintf(filename, "generation_%i", generation);
    std::ofstream  f;
    f.open (filename);
    f << BuyAtSpecificMinuteModel::csv_header() << endl;
    for (int i = 0; i <NUMBER_OF_MODELS; ++i)
    {
        f << models[i].to_csv() << endl;
    }   
}
*/
