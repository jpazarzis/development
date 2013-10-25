#include <iostream>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
 
int main() {
    using namespace boost::posix_time; /* put that in functions where you work with time (namespace) */
    using namespace boost::gregorian;
 
    ptime now = microsec_clock::local_time(); // current *LOCAL TIMEZONE* time/date

    ptime t = ptime(date(2013,12,1), hours(5)+minutes(4)+seconds(2));

    cout <<  t.date().year()  << endl;
    cout <<  t.date().month()  << endl;
    cout <<  t.date().day()  << endl;
    cout <<  "hours: "<< t.time_of_day().hours()  << endl;
    cout <<  "minutes: "<<  t.time_of_day().minutes()  << endl;
    cout <<  "seconds: "<<  t.time_of_day().seconds()  << endl;

    ptime t0 = ptime(date(2013,12,1), hours(8)+minutes(4)+seconds(2));

    time_duration tod = t0 - t;

    cout << tod.hours() << ':'
        << tod.minutes() << ':'
        << tod.seconds()
        << std::endl;

   time_duration td1(hours(10));

   cout << td1.hours() << ':'
        << td1.minutes() << ':'
        << td1.seconds()
        << std::endl;


    if(tod >= td1)
    {
        cout << "larger" << endl;
    }
    else
    {
        cout << "smaller" << endl;   
    }

 

}
