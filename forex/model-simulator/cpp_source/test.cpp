#include "std_include.h"
#include "Statistics.h"
#include <numeric>
using namespace std;




int main()
{
        vector<double> v;

        /*
        v.push_back(10.0);
        v.push_back(0.5);
        v.push_back(-100);
        v.push_back(-120);
        v.push_back(-180);
        v.push_back(-151.5);
        19
        */

        v.push_back(1000+10.0);
        v.push_back(1000+0.5);
        v.push_back(1000-100);
        v.push_back(1000-120);
        v.push_back(1000-180);
        v.push_back(1000-151.5);
        0.18



/*
        v.push_back(1000.00); 
        v.push_back(1020.45); 
        v.push_back(1107.19); 
        v.push_back(1157.57); 
        v.push_back(1192.25); 
        v.push_back(1152.39); 
        v.push_back(1112.90); 
        v.push_back(1046.51); 
        v.push_back(904.68); 
        v.push_back(812.73); 
        v.push_back(924.00); 
        v.push_back(911.29); 
        v.push_back(890.49); 
        v.push_back(981.40); 
        v.push_back(1033.45); 
        v.push_back(1051.42); 
        v.push_back(1038.79); 
        v.push_back(960.97); 
        v.push_back(1031.13); 
        v.push_back(1029.80); 
        v.push_back(1039.31); 
        v.push_back(1056.20); 
        v.push_back(1026.91); 
        v.push_back(1038.69); 
        v.push_back(1058.01); 
        v.push_back(0.5); */
 
        cout << max_drawdown(v) << endl;


}




