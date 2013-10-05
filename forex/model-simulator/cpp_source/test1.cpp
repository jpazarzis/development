#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;



int main()
{
    char buffer[1024];
    int id = 19283;
    double price = 9838.223;

    sprintf(buffer, "[order] id: %d %10.6f", id, price);

    cout << buffer << endl;



     
}
