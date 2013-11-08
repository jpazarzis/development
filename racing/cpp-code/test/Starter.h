// Starter.h
#ifndef STARTER_INCLUDED
#define STARTER_INCLUDED
#include <stdio.h>

class Starter {
    public:

        Starter();

        virtual ~Starter();

        void ResetBuffer();

        bool IsValid() const;

        char* c_str() ;

        static void Initialize();

        static Starter Make(FILE* file);

    private:

        char* _buffer;
        int* _index;
};

#endif // STARTER_INCLUDED
