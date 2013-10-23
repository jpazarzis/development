#include "Utilities.h"
#include "TickEngine.h"
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Order.h"
#include "SellBasedInDelta.h"
#include "Logger.h"
#include "Identifiable.h"
#include <fstream>
#include "GeneticAlgorithm.h"
#include "xmldocument.h"



using namespace std;

void forward_test(XmlNode& config);


void optimize_models(GeneticAlgorithm<SellBasedInDelta>& ga, 
                    const std::string& tick_file,
                    int number_of_ticks)
{
    TickEngine te;
    for (int i = 0; i< ga.size(); ++i)
        ga[i]->start_listening(&te);
    te.run(tick_file, number_of_ticks);
    for (int i = 0; i < ga.size(); ++i)
    {
        ga[i]->calc_fitness();
        ga[i]->stop_listening();
    }
    Order::clear_order_pool();
}


void run_optimizer(XmlNode& config)
{
    try
    {
            const std::string tick_file =  config["tick_file"].value();
            const int colony_size = config["colony_size"].value_to_int();
            const int number_of_ticks = config["number_of_ticks"].value_to_int();

            GeneticAlgorithm<SellBasedInDelta> ga(colony_size);
            int i = 0;
            for(;;)
            {
                cout << "generation: " << ++i << " " << cout << timestamp() <<endl;
                optimize_models(ga,tick_file,number_of_ticks);
                if (ga.evolve(true))
                    break;
                cout << "Winner so far" << endl;
                cout << ga[0]->get_full_description() << endl;
            }
            cout << "done" << endl;
    }
    catch (const char* psz)
    {
        std::cout << psz << std::endl;
    }
}

int main(int argc, char *argv[])
{

    if( argc < 2)
    {
        cout << "correct use: " << argv[0] << "[configuration.xml]" << endl;
        return -1;
    }
    
     cout << timestamp() << endl;
     srand ( time(NULL) );

     XmlDocument configuration(argv[1]);

     if(configuration.contains("forward_test"))
     {
         forward_test(configuration["forward_test"]);
     }
     else  if(configuration.contains("optimize"))
     {
         run_optimizer(configuration["optimize"]);
     }
     else
     {
        cout << "nothing to do.." << endl;
     }

    cout << timestamp() << endl;

    return 0;
}



void forward_test(XmlNode& config)
{    
    std::string filename = config["tick_file"].value();

    long max_number_of_ticks = -1;
    long start_after = 0;

    if(config.contains("max_number_of_ticks"))
    {
        max_number_of_ticks = config["max_number_of_ticks"].value_to_int();
    }

    if(config.contains("start_after"))
    {
        start_after = config["start_after"].value_to_int();
    }

    double minute_to_trade = config["minute_to_trade"].value_to_double();
    double delta = config["delta"].value_to_double();
    double stop_loss = config["stop_loss"].value_to_double();
    double take_profit = config["take_profit"].value_to_double();

    SellBasedInDelta model;
    model.set_values(minute_to_trade,delta,stop_loss,take_profit);
    //model.set_values(38.0,10.5,25.21,11.68);
    //model.set_values(41,11.33,66.19,12.71);
    //model.set_values(47, 11.01, 68.75, 15.49);
    //model.set_values(49, 9.48, 66.65, 10.6);
    TickEngine te;
    model.start_listening(&te);
    te.run(filename, max_number_of_ticks,start_after);
    model.calc_fitness(true);
    cout << model.to_string() << endl << endl;
    cout << model.get_full_description() << endl;
    Order::clear_order_pool();
}

/*
void dump_generation(int generation, SellBasedInDelta* models)
{
    char filename[1024];
    sprintf(filename, "generation_%i", generation);
    std::ofstream  f;
    f.open (filename);
    f << SellBasedInDelta::csv_header() << endl;
    for (int i = 0; i <NUMBER_OF_MODELS; ++i)
    {
        f << models[i].to_csv() << endl;
    }   
}
*/
