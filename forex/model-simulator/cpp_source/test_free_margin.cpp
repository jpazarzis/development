#include "TickEngine.h"
#include "FreeMarginModel.h"
#include "toolkit.h"
#include "TickPool.h"
#include <iostream>

using namespace std;

void load_ticks() {
    TickPool& tp = TickPool::singleton();
    cout << timestamp() << endl;
    cout << "loading ticks..." << endl;
    auto from_date = make_date("20120401");
    auto to_date = make_date("20120630");
    tp.load("/home/john/projects/forex/historical-ticks/EUR_USD.csv", from_date, to_date);
    cout << timestamp() << endl;
    cout << tp.size() << endl;
}

int main(){
    cout << "testing" << endl;
    load_ticks();
     //FreeMarginExecutor fme(100000,50,10,long_position_factory);
     //fme.start_processising();
     FreeMarginModel fmm;
     fmm.calculate_fitness();

     cout << fmm.to_string() << endl;
}
