
// DateTime.h
//
// Author        : John Pazarzis
// Creation date : Wed 23 Oct 2013 10:45:53 PM EDT
//
// Summary
//      Summary goes here
//
// Notes
  //

#ifndef DATETIME_INCLUDED
#define DATETIME_INCLUDED

#include<string>

class DateTimeImpl;

class DateTime final
{
        const DateTimeImpl*  _pimpl;
    
    public:

        DateTime(   int year, 
                    int month, 
                    int day, 
                    int hour = 0,
                    int minute = 0,
                    int second = 0);


        ~DateTime();

        DateTime(const DateTime& other);

        DateTime& operator=(const DateTime& other);

        int year() const;

        int month() const;

        int day() const;

        int hour() const;

        int minute() const;

        int second() const;

        bool is_valid() const;
};


bool operator <(const DateTime& lhp, const DateTime& rhp);

bool operator >(const DateTime& lhp, const DateTime& rhp);

bool operator ==(const DateTime& lhp, const DateTime& rhp);

bool operator !=(const DateTime& lhp, const DateTime& rhp);

bool operator <=(const DateTime& lhp, const DateTime& rhp);

bool operator >=(const DateTime& lhp, const DateTime& rhp);


#define VALIDATE_DATE(d) if(!d.is_valid()) throw "invalid date";

#endif // DATETIME_INCLUDED
