#include <iostream>
#include <deque>
#include <thread>         
#include <mutex>          
#include <condition_variable>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <time.h>
using namespace std;
using namespace std::placeholders;


class Calculators {
    public:
        void exotic(int x, int y) {
            cout << x << " + 2*" << y << " = " << x+ 2* y << endl;
        }

        void adder(int x, int y){
            cout << x << " + " << y << " = " << x+y << endl;
        }

        void multiplier(int x, int y){
            cout << x << " * " << y << " = " << x*y << endl;
        }
};


int junk(double x, std::string y) {
    cout << y << " " << x << endl;
}

int main(){

    using calc = function<void (Calculators&, int, int)>;

    Calculators c;
    vector<calc> foos = {&Calculators::exotic, &Calculators::adder, &Calculators::multiplier};

    using deffered = function<void(void)>;
    vector<deffered> d;
    
    for(auto f: foos)
    {
        f(c, 8,3);
        d.push_back(bind(f, c,5.0, 3));
    }
   

    auto f = bind(&junk, 5.0, "john");
    f();

    deffered f2 = bind(&Calculators::exotic, c,5.0, 3);
    f2();


    for(auto f: d)
    {
        f();
    }


}

