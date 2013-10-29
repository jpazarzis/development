#ifndef ID_GENERATOR_INCLUDED
#define ID_GENERATOR_INCLUDED

class Identifiable
{
        static long _seed;
    public:
        const long _id;
        virtual long get_id() const;
        Identifiable();
        virtual ~Identifiable();
};

#endif

