#include "ComparableValue.h"
#include "Scalar.h"
#include "Aggregate.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>






int main()
{
    using namespace std;
    srand(time(NULL));
    auto bid = Scalar::make("bid");
    *bid = 1.253;
    cout << bid->value() << endl;
}
