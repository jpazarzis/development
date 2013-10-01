#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

/////////////////////////////////////////////////////
// test-cloning.cpp
// 
// Author: John Pazarzis
//
// Date  : Sep. 30 213
//
// Summary: 
//      Implemets the functionality to create an optimizable structure
//      so it can be used by the genetic algorithm
//
// Notes:
//
// To make a structure usable by the optimization algorithm it needs to be
// declared like the following struct A.
//
// Every variable needs to be declared by using the DECLARE_OPTIMIZABLE...
// macro, proving the name of the variable and the minimum - maximum
// values. 
//
// To make the structure auto-randomizeable we need to overwrite the 
// default constructor adding the macro RANDOMIZE_VARIABLE for every
// variable.  By doing so, we assure that each object of this type 
// will be created it will automatically be initialized randomly.
//
// We also need to provide the equality test by overloading the == operator
// so we can later check if an object already exists in a collection
//
////////////////////////////////////////////////////////////////////////

// Declares an integer variable providing its min-max values. Used to
// easily declare optimize-able structures
#define DECLARE_OPTIMIZABLE_INT(variable, min, max)\
    int variable;\
    void randomize2_##variable() {\
        variable = (rand() % (max - min) ) + min;\
    }
   
// Called from the constructor of an optimiable struct
#define RANDOMIZE_VARIABLE(variable) randomize2_##variable();

// Called by the genetic algorithm to clone two chromosomes. It is very 
// fast since is using memset to combine two chromosomes creating two new
// chromosomes using a random breaking point based in the size of the
// chromosome in bytes. It is possible to return identical chromosomes if
// the breaking point appears to be selected in marginal points. In this
// case the client code should perform the necessary checking to handle
// the situation (most likely to ignore the breeding and continue)
#define CLONE(STRUCT,DEST1,DEST2,SOURCE1,SOURCE2) \
    int BREAK_POINT = (rand() % (sizeof(STRUCT) - 1) ) + 1;\
    memset((void*)&DEST1, 0, sizeof(STRUCT));\
    memset((void*)&DEST2, 0, sizeof(STRUCT));\
    memcpy ( (void*)&DEST1, (void*)&SOURCE1, BREAK_POINT );\
    memcpy ( (void*)&DEST1 +BREAK_POINT, ((void*)&SOURCE2) +BREAK_POINT, sizeof(STRUCT) - BREAK_POINT);\
    memcpy ( (void*)&DEST2, (void*)&SOURCE2, BREAK_POINT);\
    memcpy ( (void*)&DEST2 +BREAK_POINT, ((void*)&SOURCE1) +BREAK_POINT, sizeof(STRUCT) - BREAK_POINT);

// Used to overload the == operator for an optimiable structure.
#define BEGIN_EQUALITY_TEST(STRUCT)\
    static bool operator ==(const STRUCT & lhp, const STRUCT & rhp) { return

// Adds a variable to be used in the overload of the == operator
#define ADD_EQUALIRY_VARIABLE(variable) \
    lhp. variable == rhp. variable &&

// Ends the overload of the == operator
#define END_EQUALITY_TEST true ;}

// The following is a sample structure 
struct A
{
    DECLARE_OPTIMIZABLE_INT(i, 1, 59)
    DECLARE_OPTIMIZABLE_INT(j, 1, 20)
    DECLARE_OPTIMIZABLE_INT(k, 10, 20)
    DECLARE_OPTIMIZABLE_INT(m, 100, 200)
    
    A()
    {
        RANDOMIZE_VARIABLE(i)
        RANDOMIZE_VARIABLE(j)
        RANDOMIZE_VARIABLE(k)
        RANDOMIZE_VARIABLE(m)
    }   
};

BEGIN_EQUALITY_TEST(A)
        ADD_EQUALIRY_VARIABLE(i)
        ADD_EQUALIRY_VARIABLE(j)
        ADD_EQUALIRY_VARIABLE(k)
        ADD_EQUALIRY_VARIABLE(m)
END_EQUALITY_TEST


int main()
{
    srand ( time(NULL) );
    A a[2];
    for(int i = 0; i < 2; ++i)
    {
        cout << a[i].i << " " << a[i].j <<" " << a[i].k << " " << a[i].m << endl;
    }

    A b[2];
    cout << "=======================" << endl;
    for(int j = 0; j < 10; ++j)
    {
            CLONE(A,b[0],b[1],a[0],a[1])
            
            for(int i = 0; i < 2; ++i)
            {
                cout << b[i].i << " " << b[i].j <<" " << b[i].k << " " << b[i].m << endl;
            }

            if (b[0] == a[0] || b[1] == a[0])
            {
                cout << "are equal"<< endl;
            }
            cout << "=======================" << endl;
    }    
    return -1;
}
