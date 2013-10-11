#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;


class A
{
public:
    const vector<double> v;
    A(const vector<double>& v1)   : v(v1)
    {
        
    }

    void test()
    {
    }


};



int main()
{
    
    
            vector<double> v;
            v.push_back(10);
            v.push_back(12.1);
            v.push_back(13.1);
      
            A a(v);
            v.clear();
    




    cout << a.v.size() << endl;
    for(int i = 0; i<a.v.size(); ++i)
        cout << a.v[i] << endl;

     
}
