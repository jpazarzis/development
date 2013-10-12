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
#include "GeneticAlgorithm.h"
using namespace std;


void calc_fitness(GeneticAlgorithm<BuyAtSpecificMinuteModel>& ga)
{
    // At this point the models have already run and now I need to assign their
    // fitnesses. This is a two step process: since the fitness function should
    // always be a positive number I need to normilize it

    for (int i = 0; i < ga.size(); ++i)
         ga[i]->calc_fitness();

    std::vector<double> unnormalized_fitness;
    for (int i = 0; i < ga.size(); ++i)
        unnormalized_fitness.push_back(ga[i]->get_fitness());

    if(unnormalized_fitness.size() < 10)
        throw "cannot continue the processing. It seems that very few models are actually creating orders";

    const double min_fitness = *std::min_element(unnormalized_fitness.begin(), unnormalized_fitness.end());

    for (int i = 0; i < ga.size(); ++i)
         ga[i]->normalize_fitness(min_fitness);
}

void backtest_models(GeneticAlgorithm<BuyAtSpecificMinuteModel>& ga)
{
    TickEngine te;
    for (int i = 0; i< ga.size(); ++i)
        ga[i]->start_listening(&te);
    te.run("../../historical-ticks/EUR_USD.csv", 10000000);
    calc_fitness(ga);
    for (int i = 0; i < ga.size(); ++i)
         ga[i]->stop_listening();
    Order::clear_order_pool();
}

int main()
{ 
    time_t current_time;
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;
    srand ( time(NULL) );

    try
    {
            GeneticAlgorithm<BuyAtSpecificMinuteModel> ga(200);

            int i = 0;
            for(;;)
            {
                cout << "generation: " << ++i << endl;
                backtest_models(ga);
                if (ga.evolve())
                    break;
            }

            cout << "done" << endl;
            current_time = time(NULL);
            cout << ctime(&current_time) << endl;
    }
    catch (const char* psz)
    {
        std::cout << psz << std::endl;
    }

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
