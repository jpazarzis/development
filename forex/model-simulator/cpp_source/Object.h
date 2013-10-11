// Object.h
//
// Author        : John Pazarzis
// Creation date : Tue 08 Oct 2013 10:53:17 AM EDT
//
// Summary
//      Serves as the base class for all the public classes
//      of the system providing common functionality
//
// Notes
//

#ifndef OBJECT_INCLUDED
#define OBJECT_INCLUDED

#include <string>


class Object
{
    public:

        virtual std::string to_string() const = 0;
};

#endif // OBJECT_INCLUDED
