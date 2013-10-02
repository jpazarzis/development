////////////////////////////////////////////////////////////////////////
// cloner.h
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

#if !CLONER_DEFINED
#define CLONER_DEFINED



#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
using namespace std;


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

//////////////////////////////////////////////////////////////////////////////
// create_next_generation: 
// 
//      Receives as a single parameter an array containing pointers
//      to the objects to clone. 
//
//      The cloning is performed in place. 
//
//      We need to specify the SIZE template parameter which defines
//      the size of the array while FIT_CHROMOSOMES_COUNT defines
//      the number of the chromosomes that will be used as parents
//      starting from the begining of the array.
//
//      Before you call this function you should already calculate the
//      fitness of each chromosome and create a sorted array of pointers
//      to each of them which you will pass to this function.
//
//      Since I am using pointers to the clonable objects, this means that
//      they can be contained in any other object (most of the cases in a model)
//      so after the function returns your model will be already updated. 
//      By doing so I avoid temporary objects making the cloning very fast.
//
//      Returns true if a new generation is created successfully. 
//      It will return true if it cannot generate a new generation of different
//      chromosomes after 10 consequtive tries for any of them.  When this
//      happens, it usually means that the proportion of the parents to
//      descendants chromosomes is very small to the point to cover all 
//      possible combinations. In this case you need to either increase the
//      size of the parents or decrease the size of the descenands.
//////////////////////////////////////////////////////////////////////////////

template<class T, int SIZE, int FIT_CHROMOSOMES_COUNT> 
bool create_next_generation( T** a)
{
    const int fcc = FIT_CHROMOSOMES_COUNT % 2 == 0 ? FIT_CHROMOSOMES_COUNT : FIT_CHROMOSOMES_COUNT - 1;
    const int size = SIZE % 2 == 0 ? SIZE : SIZE - 1;

    int tries = 0;

    for(register int i = fcc; i < size;)
    {
            if(tries >= 1000)
            {
                return false;
            }

            const int index1 = rand() % (fcc);
            const int index2 = rand() % (fcc);

            if(index1 == index2)
            {
                continue;
            }

            CLONE(T,*a[i],*a[i+1],*a[index1],*a[index2])

            // If either one of the new chromosomes already exist 
            // ignore the cloning by not incrementing the index!

            bool ok = true;
            for(register int j = 0; j < i; ++j)
            {
                if(*a[i] == *a[j] || *a[i+1] == *a[j])
                {
                    ok = false;
                    tries += 1;
                    break;
                }               
            }
            
            if(ok)
            {
                i += 2;
                tries = 0;
            }

            
            
    }
    return true;
}

#endif
