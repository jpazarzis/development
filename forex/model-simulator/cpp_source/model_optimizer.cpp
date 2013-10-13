#include "Utilities.h"
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

void backtest();


void backtest_models(GeneticAlgorithm<BuyAtSpecificMinuteModel>& ga)
{
    TickEngine te;
    for (int i = 0; i< ga.size(); ++i)
        ga[i]->start_listening(&te);
    te.run("../../historical-ticks/EUR_USD.csv", 1000000);
    for (int i = 0; i < ga.size(); ++i)
    {
        ga[i]->calc_fitness();
        ga[i]->stop_listening();
    }
    Order::clear_order_pool();
}

int main()
{ 
    cout << timestamp() << endl;
    srand ( time(NULL) );
    backtest();
    return 0;

    try
    {
            GeneticAlgorithm<BuyAtSpecificMinuteModel> ga(200);

            int i = 0;
            for(;;)
            {
                cout << "generation: " << ++i << " " << cout << timestamp() <<endl;
                backtest_models(ga);
                if (ga.evolve())
                    break;
            }

            cout << "done" << endl;
    }
    catch (const char* psz)
    {
        std::cout << psz << std::endl;
    }
    cout << timestamp() << endl;

    return 0;
}



void backtest()
{

/*
  id      min.        delta     stoploss   profittake   #orders             fitness            drawdown             balance
10064        22      2.42000     63.50000     27.91000      1837         46629.56606             0.49664         76621.00000
10147        22      2.59000     68.20000     27.51000      1808         44931.25410             0.51815         75436.00000
10099        22      2.43000     68.20000     27.91000      1837         44829.24606             0.54743         77501.00000
*/

/*
10112        25      3.78000     90.30000     37.23000      1622        110700.26628             0.36980        160233.00000         68343.00000
*/

//  35      7.15000     14.37000     40.44000 

    BuyAtSpecificMinuteModel model;
    model.set_values(  35      ,7.15000     ,14.37000     ,40.44000 );
    TickEngine te;
    model.start_listening(&te);
    te.run("../../historical-ticks/EUR_USD.csv",40000000 ,10000000);
    model.calc_fitness(true);
    cout << model.to_string() << endl << endl;

    cout << model.get_full_description() << endl;
    Order::clear_order_pool();
}

/*
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
