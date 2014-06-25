#include <Python.h>
#include <iostream>
#include <vector>
#include "PythonSignalCaller.h"
using namespace std;

void test(int count = 1)
{
    double bid = 10.1, ask = 10.1, triggering_delta = 10.1, minute_to_trade = 1, expiration_minute = 1;
    PythonSignalCaller pm;

    //pm.set_module("process_tick_sample");
    pm.set_module("test_model");

    for(int i = 0; i < count; ++i)
    {
        vector<double> optimazable_parameters;
        int year = 1, month, day = 1, hour = 1,  minute = 1, second = 1;
        double bid = 100.1, ask = 100.2;

        for(int j = 0; j < pm.count_optimizable_parameters(); ++j)
        {
            optimazable_parameters.push_back(1012.1);
        }

        int s = pm.get_signal(year, month, day, hour, minute, second, bid, ask, optimazable_parameters) ;
    }
}

int main()
{
    int year = 1, month, day = 1, hour = 1,  minute = 1, second = 1;
    double bid = 10.1, ask = 10.1, triggering_delta = 10.1, minute_to_trade = 1, expiration_minute = 1;
    Py_Initialize();
    cout << "start" << endl;

    for(int i = 0; i < 200; ++i)
    {
        test(100);
        cout << i << endl;
    }

    cout << "finish" << endl;
    Py_Finalize();
    return 0;
}

