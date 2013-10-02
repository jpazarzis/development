#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
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

#define DECLARE_OPTIMIZABLE_DOUBLE(variable, min, max)\
    double variable;\
    void randomize2_##variable() {\
        variable = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;\
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
#define CHECK_EQUALITY_FOR_INT(variable) \
    lhp. variable == rhp. variable &&

#define CHECK_EQUALITY_FOR_DOUBLE(variable) \
    fabs(lhp. variable - rhp. variable) <= 0.0000001 &&

// Ends the overload of the == operator
#define END_EQUALITY_TEST true ;}

// The following is a sample structure 
struct A
{
    DECLARE_OPTIMIZABLE_INT(i, 1, 59)
    DECLARE_OPTIMIZABLE_INT(j, 1, 20)
    DECLARE_OPTIMIZABLE_INT(k, 10, 20)
    DECLARE_OPTIMIZABLE_INT(m, 100, 200)
    DECLARE_OPTIMIZABLE_INT(m1, 100, 200)
    DECLARE_OPTIMIZABLE_INT(m2, 100, 200)
    DECLARE_OPTIMIZABLE_INT(m3, 100, 200)    
    DECLARE_OPTIMIZABLE_INT(m4, 100, 200)
    DECLARE_OPTIMIZABLE_INT(m5, 100, 200)

    //DECLARE_OPTIMIZABLE_DOUBLE(s, 100.0, 110.0)
    
    A()
    {
        RANDOMIZE_VARIABLE(i)
        RANDOMIZE_VARIABLE(j)
        RANDOMIZE_VARIABLE(k)
        RANDOMIZE_VARIABLE(m)
        RANDOMIZE_VARIABLE(m1)
        RANDOMIZE_VARIABLE(m2)
        RANDOMIZE_VARIABLE(m3)
        RANDOMIZE_VARIABLE(m4)
        RANDOMIZE_VARIABLE(m5)
      //  RANDOMIZE_VARIABLE(s)
    }   
};

BEGIN_EQUALITY_TEST(A)
        CHECK_EQUALITY_FOR_INT(i)
        CHECK_EQUALITY_FOR_INT(j)
        CHECK_EQUALITY_FOR_INT(k)
        CHECK_EQUALITY_FOR_INT(m)
        CHECK_EQUALITY_FOR_INT(m1)
        CHECK_EQUALITY_FOR_INT(m2)
        CHECK_EQUALITY_FOR_INT(m3)
        CHECK_EQUALITY_FOR_INT(m4)
        CHECK_EQUALITY_FOR_INT(m5)
        //CHECK_EQUALITY_FOR_DOUBLE(s)
END_EQUALITY_TEST



template<class T, int SIZE, int PARENT_SIZE> 
void create_next_generation( T* a, T* b)
{
    for(register int i = 0; i < SIZE;)
    {
            const int index1 = rand() % (PARENT_SIZE);
            const int index2 = rand() % (PARENT_SIZE);

            if(index1 == index2)
            {
                continue;
            }

            CLONE(T,b[i],b[i+1],a[index1],a[index2])
            i += 2;
    }
}

template<class T, int SIZE, int PARENT_SIZE> 
void create_next_generation1( T** a, T** b)
{
    for(register int i = 0; i < SIZE;)
    {
            const int index1 = rand() % (PARENT_SIZE);
            const int index2 = rand() % (PARENT_SIZE);

            if(index1 == index2)
            {
                continue;
            }

            CLONE(T,*b[i],*b[i+1],*a[index1],*a[index2])
            i += 2;
    }
}


void test2()
{
    A a1[10], a2[10];

    A* pa1[10];
    A* pa2[10];

    for(int i = 0; i <10; ++i)
    {
       pa1[i] = & a1[i];
       pa2[i] = & a2[i];
    }

    for(int i = 0; i < 10; ++i)
    {
        memset((void*)&a2[i], 0, sizeof(A));    
    }

    cout << "before.." << endl;

    for(int i = 0; i < 10; ++i)
    {
        cout << a2[i].i << " " << a2[i].j <<" " << a2[i].k << " " << a2[i].m << " " <<endl;
    }


    create_next_generation1<A,10,10>(pa1,  pa2);

    cout << "after.." << endl;

    for(int i = 0; i < 10; ++i)
    {
        cout << a2[i].i << " " << a2[i].j <<" " << a2[i].k << " " << a2[i].m << " " <<endl;
    }

    

}


void test()
{
   A a[12];
    for(int i = 0; i < 12; ++i)
    {
        //cout << a[i].i << " " << a[i].j <<" " << a[i].k << " " << a[i].m << " " <<endl;
    }

    A b[100];
    int j = 0;
    while(j < 98)
    {
            int index1 = (rand() % (11 - 0) ) + 0;
            int index2 = (rand() % (11 - 0) ) + 0;

            if(index1 == index2)
            {
                continue;
            }
            

            CLONE(A,b[j],b[j+1],a[index1],a[index2])


            bool already_exists = false;
            for(int i = 0; i < j - 1; ++ i)
            {
                if(b[j] == b[i])
                {
                    //cout << "already exists: " << j << " " << i << endl;
                    already_exists = true;     
                    break;
                }
            }

            if(!already_exists)
            {
                j += 2;
            }

            
    }

    //cout << "=======================" << endl;
    for(int i = 0; i < 100; ++i)
    {
        //cout << i <<" => "<< b[i].i << " " << b[i].j <<" " << b[i].k << " " << b[i].m << " " << endl;
    }       
}


int main()
{
    srand ( time(NULL) );

    test2();
      
    return -1;
}
