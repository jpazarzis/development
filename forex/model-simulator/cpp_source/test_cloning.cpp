#include "cloner.h"
#include <iostream>
#include <time.h>
using namespace std;


// The following is a sample structure 
struct A
{
    DECLARE_OPTIMIZABLE_INT(i, 1, 59)
    DECLARE_OPTIMIZABLE_INT(j, 1, 20)
    DECLARE_OPTIMIZABLE_INT(k, 10, 20)
    DECLARE_OPTIMIZABLE_INT(m, 100, 200)

    //DECLARE_OPTIMIZABLE_DOUBLE(s, 100.0, 110.0)
    
    A()
    {
        RANDOMIZE_VARIABLE(i)
        RANDOMIZE_VARIABLE(j)
        RANDOMIZE_VARIABLE(k)
        RANDOMIZE_VARIABLE(m)
      //  RANDOMIZE_VARIABLE(s)
    }   
};

BEGIN_EQUALITY_TEST(A)
        CHECK_EQUALITY_FOR_INT(i)
        CHECK_EQUALITY_FOR_INT(j)
        CHECK_EQUALITY_FOR_INT(k)
        CHECK_EQUALITY_FOR_INT(m)
        //CHECK_EQUALITY_FOR_DOUBLE(s)
END_EQUALITY_TEST

#define CHROMOSOME_COUNT 100
bool test2()
{
    A a1[CHROMOSOME_COUNT];
    A* pa1[CHROMOSOME_COUNT];

    for(int i = 0; i <CHROMOSOME_COUNT; ++i)
    {
       pa1[i] = & a1[i];
    }

    return create_next_generation<A,CHROMOSOME_COUNT,10>(pa1);


}

int main()
{
    srand ( time(NULL) );

    int success = 0, fail = 0;
    for(int i = 0; i < 10000; ++i)
    {
        if(test2())
            ++success;
        else
            ++fail;

    }

    cout << success << endl;
    cout << fail << endl;
      
    return -1;
}
