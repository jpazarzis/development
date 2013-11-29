// nntrading.cpp
//
// John Pazarzis
// Mon Nov 18 09:13:20 EST 2013
// uses the signal of a nn to trade..

#include "TickEngine.h"
#include "FreeMarginModel.h"
#include "toolkit.h"
#include "TickPool.h"
#include <iostream>
#include "BuyBasedInNN.h"

using namespace std;

void load_ticks(const string& from_date, const string& to_date) {
    TickPool& tp = TickPool::singleton();
    cout << timestamp() << endl;
    cout << "loading ticks..." << endl;
    tp.load("/home/john/projects/forex/historical-ticks/EUR_USD.csv", make_date(from_date), make_date(to_date));
    cout << timestamp() << endl;
    cout << tp.size() << endl;
}

int main(){


    string from_date ( "20120920");
    //string to_date ( "20130830");
    //string to_date ( "20130130");
    string to_date ( "20130721");
    cout << "period: " << from_date << " - " << to_date << endl;
    load_ticks(from_date, to_date);
    BuyBasedInNN nn;
    nn.calculate_fitness();
}
