#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define DECLARE_OPTIMIZABLE_INT(variable, min, max)\
    int variable;\
    void randomize2_##variable() {\
        variable = (rand() % (max - min) ) + min;\
    }
    
   
#define RANDOMIZE_VARIABLE(variable) randomize2_##variable();


#define CLONE(STRUCT,DEST1,DEST2,SOURCE1,SOURCE2,BREAK_POINT) \
    memset((void*)&DEST1, 0, sizeof(STRUCT));\
    memset((void*)&DEST2, 0, sizeof(STRUCT));\
    memcpy ( (void*)&DEST1, (void*)&SOURCE1, BREAK_POINT );\
    memcpy ( (void*)&DEST1 +BREAK_POINT, ((void*)&SOURCE2) +BREAK_POINT, sizeof(STRUCT) - BREAK_POINT);\
    memcpy ( (void*)&DEST2, (void*)&SOURCE2, BREAK_POINT);\
    memcpy ( (void*)&DEST2 +BREAK_POINT, ((void*)&SOURCE1) +BREAK_POINT, sizeof(STRUCT) - BREAK_POINT);

struct A
{
    DECLARE_OPTIMIZABLE_INT(i, 1000000, 20)
    DECLARE_OPTIMIZABLE_INT(j, 100, 2000)
    
    A()
    {
        RANDOMIZE_VARIABLE(i)
        RANDOMIZE_VARIABLE(j)
    }    
};

int main()
{
    srand ( time(NULL) );
    A a[100];
    for(int i = 0; i < 100; ++i)
    {
        cout << a[i].i << " " << a[i].j << endl;
    }

    



    //cout << a1.i << " " << a1.j << endl;

    //A a2;
    //cout << a2.i << " " << a2.j << endl;

    /*    
    memcpy ( (void*)&x, (void*)&a1, index );
    memcpy ( (void*)&x +index, ((void*)&a2) +index, sizeof(A) - index);
    memcpy ( (void*)&y, (void*)&a2, index );
    memcpy ( (void*)&y +index, ((void*)&a1) +index, sizeof(A) - index);
    */
    return -1;
}
