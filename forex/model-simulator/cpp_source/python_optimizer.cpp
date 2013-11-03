////////////////////////////////////////////////////////////////////////////////
//
// python_optimizer
//
// Author        : John Pazarzis
// Creation date : Sat Nov  2 09:19:54 EDT 2013 
//
//  Used to optimize and back test a strategy that is defined in python
//
//  You need to provide a configuration file containing the required parameters
//
////////////////////////////////////////////////////////////////////////////////  

#include "toolkit.h"
#include "TickEngine.h"
#include "PythonDrivenModel.h"
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
#include "TickPool.h"
#include "ParallelProcessor.h"
#include <sys/resource.h>

using namespace std;

#define NUMBER_OF_THREADS 4

void run_optimizer(XmlNode& config)
{
    try
    {
        const std::string tick_file =  config["tick_file"].value();
        const int colony_size = config["colony_size"].value_to_int();
        assert(config.contains("from_date"));
        auto from_date = make_date(config["from_date"].value());
        assert(config.contains("to_date"));
        auto to_date = make_date(config["to_date"].value());

        if(!from_date.is_not_a_date() && !to_date.is_not_a_date())
        {
            assert(from_date < to_date);
        }

        TickPool& tp = TickPool::singleton();
        cout << timestamp() << endl;
        cout << "loading ticks..." << endl;
        tp.load(tick_file, from_date, to_date);
        cout << timestamp() << endl;
        cout << tp.size() << endl;
        GeneticAlgorithm<PythonDrivenModel> ga(colony_size);
        int i = 0;

        for(int i = 0; i < ga.size(); ++i)
        {
                ga[i]->set_python_module("module_name_goes_here");
        }


        for(;;)
        {
            cout << "generation: " << ++i << " " << cout << timestamp() << endl;

            for(int i = 0; i < ga.size(); ++i)
            {
                ga[i]->calculate_fitness();
            }

            if(ga.evolve(true))
            {
                break;
            }

            if(!ga.was_rolled_back())
            {
                cout << "Winner so far" << endl;
                cout << ga[0]->get_full_description() << endl;
            }
        }

        cout << "done" << endl;
    }
    catch(const char* psz)
    {
        std::cout << psz << std::endl;
    }
}

void forward_test(XmlNode& config)
{


    std::string filename = config["tick_file"].value();
    double minute_to_trade = config["minute_to_trade"].value_to_double();
    double delta = config["delta"].value_to_double();
    double stop_loss = config["stop_loss"].value_to_double();
    double take_profit = config["take_profit"].value_to_double();
    double expriration_minutes = config["expriration_minutes"].value_to_double();
    assert(config.contains("from_date"));
    auto from_date = make_date(config["from_date"].value());
    assert(config.contains("to_date"));
    auto to_date = make_date(config["to_date"].value());

    if(!from_date.is_not_a_date() && !to_date.is_not_a_date())
    {
        assert(from_date < to_date);
    }

    TickPool& tp = TickPool::singleton();
    cout << timestamp() << endl;
    cout << "loading ticks..." << endl;
    tp.load(filename, from_date, to_date);
    cout << timestamp() << endl;
    cout << tp.size() << endl;
    PythonDrivenModel model;
    model.set_python_module("test_model");

    std::vector<double> values = {minute_to_trade, delta};

    model.set_values(stop_loss, take_profit, expriration_minutes, values);
    model.calculate_fitness();
    cout << "Testing period: from " << from_date << " to " << to_date << endl;
    cout << model.get_full_description() << endl;
    
}




int main(int argc, char *argv[])
{
    
    if(argc < 2)
    {
        cout << "correct use: " << argv[0] << "[configuration.xml]" << endl;
        return -1;
    }

    Py_Initialize();
    cout << timestamp() << endl;
    srand(time(NULL));
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
    Py_Finalize();
    return 0;
}
