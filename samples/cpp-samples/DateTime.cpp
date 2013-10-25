#include "DateTime.h"

static const char days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

enum
{
    SECONDS_IN_MINUTE = 60,
    SECONDS_IN_HOUR = 60 * SECONDS_IN_MINUTE,
    SECONDS_IN_DAY = 24 * SECONDS_IN_HOUR,
};

// return 1 if year is a leap year, else 0
static bool isleap(int y)
{
        if (y % 4)
                return false;
        if (y % 100)
                return true;
        if (y % 400)
                return false;
        return true;
}

class DateTimeImpl
{
public:
    const int _year;
    const int _month;
    const int _day;
    const int _hour;
    const int _minute;
    const int _second;

    DateTimeImpl(int year, int month, int day, int hour, int minute, int second):
        _year(year), _month(month), _day(day), _hour(hour), _minute(minute), _second(second)
    {
    }


};

DateTime::DateTime( int year, int month, int day, int hour, int minute, int second):
           _pimpl( new DateTimeImpl(year, month, day, hour, minute, second))
{
    
}

DateTime::~DateTime()
{
    delete _pimpl;
    _pimpl = NULL;
}

DateTime::DateTime(const DateTime& other):
    _pimpl(new DateTimeImpl(other.year(), other.month(), other.day(), 
                            other.hour(), other.minute(), other.second()) )
{
}

DateTime& DateTime::operator=(const DateTime& other)
{
    if(this != &other)
    {
        delete _pimpl;
        _pimpl = new DateTimeImpl(other.year(), other.month(), other.day(), 
                            other.hour(), other.minute(), other.second());
    }
    return *this;
}


int DateTime::year() const
{   
    return _pimpl->_year;
}

int DateTime::month() const
{
    return _pimpl->_month;
}

int DateTime::day() const
{
    return _pimpl->_day;
}

int DateTime::hour() const
{
    return _pimpl->_hour;
}

int DateTime::minute() const
{
    return _pimpl->_minute;
}

int DateTime::second() const
{
    return _pimpl->_second;
}

bool DateTime::is_valid() const
{
   return month() >= 1 && month() <=12 &&
          year() >= 1800 && year() <= 2099 &&  
          day() >= 1 &&
          (day() <= days_in_month[month()-1] || (month() ==2 && day() == 29 && isleap(year()))) &&
          hour() >= 0 && hour() <=23 &&
          minute() >= 0 && hour() <=59 &&
          second() >= 0 && second() <=59;
}

bool operator <(const DateTime& lhp, const DateTime& rhp)
{
    if(lhp.year() != rhp.year())
        return lhp.year() < rhp.year();

    if(lhp.month() != rhp.month())
        return lhp.month() < rhp.month();

    if(lhp.day() != rhp.day())
        return lhp.day() < rhp.day();

    if(lhp.hour() != rhp.hour())
        return lhp.hour() < rhp.hour();

    if(lhp.minute() != rhp.minute())
        return lhp.minute() < rhp.minute();

    if(lhp.second() != rhp.second())
        return lhp.second() < rhp.second();
    
    return false; // equal
}

bool operator >(const DateTime& lhp, const DateTime& rhp)
{
    return rhp < lhp;
}

bool operator ==(const DateTime& lhp, const DateTime& rhp)
{
    return ((rhp < lhp) == false && (rhp > lhp) == false);
}

bool operator !=(const DateTime& lhp, const DateTime& rhp)
{
    return (lhp == rhp) == false;
}


bool operator <=(const DateTime& lhp, const DateTime& rhp)
{
    return (lhp > rhp) == false;
}

bool operator >=(const DateTime& lhp, const DateTime& rhp)
{
    return (lhp < rhp) == false;
}

