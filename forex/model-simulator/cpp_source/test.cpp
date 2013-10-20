#include "std_include.h"
#include "Statistics.h"
#include <numeric>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include "TickProcessor.h"
using namespace std;


void foo(int year, int month, int day) {
  time_t rawtime;
  struct tm * timeinfo;
  int year, month ,day;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  timeinfo->tm_year = year - 1900;
  timeinfo->tm_mon = month - 1;
  timeinfo->tm_mday = day;
  timeinfo->tm_sec = second;   // seconds of minutes from 0 to 61
  timeinfo->tm_min = minute;   // minutes of hour from 0 to 59
  timeinfo->tm_hour = hour;  // hours of day from 0 to 24

  /* call mktime: timeinfo->tm_wday will be set */
  mktime ( timeinfo );

  printf ("That day is a %s.\n", weekday[timeinfo->tm_wday]);
}




int main()
{
    Tick tick;


    tick.day = 1;
    tick.month = 3;
    tick.year = 11;
    tick.hour = 12;
    tick.minute = 45;
    tick.second = 21;
    tick.bid = 12.0; 
    tick.ask = 12.0;

    cout << tick.timestamp() << endl;
 
}




