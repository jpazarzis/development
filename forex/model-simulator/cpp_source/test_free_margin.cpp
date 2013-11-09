#include "FreeMarginModel.h"
#include "toolkit.h"
#include "TickPool.h"
#include <iostream>

using namespace std;

void load_ticks() {
    TickPool& tp = TickPool::singleton();
    cout << timestamp() << endl;
    cout << "loading ticks..." << endl;
    auto from_date = make_date("");
    auto to_date = make_date("");
    tp.load("test_ticks.csv", from_date, to_date);
    cout << timestamp() << endl;
    cout << tp.size() << endl;
}

int main(){
    cout << "testing" << endl;
    load_ticks();
     FreeMarginModel fmm(100000,50,10,long_position_factory);
     fmm.start_processising();

     cout << fmm.to_string() << endl;
}
