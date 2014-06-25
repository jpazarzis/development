#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>
using namespace std;


vector<double> test(vector<double>& v1){
    vector<double> v;
    for(auto x: v1){
        v.push_back(x+1);
    }

    return v;
}

void print_num(int i)
{
    std::cout << i << '\n';
}

int main(){

    vector<double> v = {1,2,3};

    std::function<void(int)> f_display = print_num;

    function<vector<double>(vector<double>&)> f ;
    string s;
    s.c_

    f = &test;
    
    for(auto x :test(v)){
        cout << x << " " ;
    }

    cout << endl;



}

