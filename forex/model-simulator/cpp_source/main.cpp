#include "TickEngine.h"
#include "Models.h"
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Order.h"
using namespace std;

class SampleProcessor: public TickProcessor
{
  public:
        PROCESSOR_RESULT process(const Tick& tick) 
        {
            double x = tick.bid;
            //cout << "bid: " << tick.bid << endl;
            return CONTINUE_PROCESSING;
        }
};


void test1()
{
    time_t current_time;
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;

    SampleProcessor sp[1800];

    TickEngine te;
    for(int i = 0; i <1800; ++i)
        te.register_processor(&sp[i]);
    
    te.start("../../historical-ticks/EUR_USD.csv");
    current_time = time(NULL);
    cout << ctime(&current_time) << endl;
}




int main()
{ 
    cout << "here" << endl;    
    Tick t;
    ORDER_REF order1 = Order::make(BUY, "test", 100.1, 100.2, 100.0);
    ORDER_REF order2 = Order::make(BUY, "test", 100.1, 100.2, 100.0);
    Tick t2;
    int i = order1.process(t2);
    cout << i << endl;

    
    
    return 0;
}
