#include "toolkit.h"
#include <stdlib.h>
#include <stdio.h>      
#include <fstream>

std::string sformat(double value, const std::string& format)
{
    char buffer[1024];
    sprintf(buffer, format.c_str(), value);
    return buffer;
}

std::string sformat(int value, const std::string& format)
{
    char buffer[1024];
    sprintf(buffer, format.c_str(), value);
    return buffer;
}

std::string sformat(long value, const std::string& format)
{
    char buffer[1024];
    sprintf(buffer, format.c_str(), value);
    return buffer;
}


std::string sformat(const std::string& strg, const std::string& format)
{
    char buffer[1024];
    sprintf(buffer, format.c_str(), strg.c_str());
    return buffer;
}

std::string timestamp()
{
    time_t rawtime;
    time (&rawtime);
    return ctime(&rawtime);
}

std::string format_time(const boost::posix_time::ptime& date_time)
{
   if(date_time.date().is_not_a_date()){
        return "invalid";
    }

    char buffer[32];
    sprintf(buffer, "%02i:%02i:%02i",
        (int)date_time.time_of_day().hours(),
        (int)date_time.time_of_day().minutes(),
        (int)date_time.time_of_day().seconds()
        );
    return buffer;
}

std::string format_datetime(const boost::posix_time::ptime& date_time)
{
    if(date_time.date().is_not_a_date()){
        return "invalid";
    }
    

    char buffer[32];
    sprintf(buffer, "%02i/%02i/%02i %02i:%02i:%02i",
        (int)date_time.date().month(),
        (int)date_time.date().day(),
        (int)date_time.date().year()-2000,
        (int)date_time.time_of_day().hours(),
        (int)date_time.time_of_day().minutes(),
        (int)date_time.time_of_day().seconds()
        );
    return buffer;
}


boost::gregorian::date make_date(std::string date_str){
    using namespace boost::gregorian;
    try {
            trim(date_str);
            int year = 0, month = 0, day = 0;
            if(date_str.length() == 8){ // YYYYMMDD format
                year = atoi(date_str.substr(0,4).c_str());
                month = atoi(date_str.substr(4,2).c_str());
                day = atoi(date_str.substr(6,2).c_str());
            }
            else if(date_str.length() == 10){ // MM/DD/YYYY format
                year = atoi(date_str.substr(6).c_str());
                day = atoi(date_str.substr(3,5).c_str());
                month = atoi(date_str.substr(0,2).c_str());
            }

            return date(year,month,day);

     }
     catch(...) {
        return date();
     }
}






double max_drawdown(const std::vector<double>& v)
{
    const int size = v.size();

    if(size<=1)
        return 0;

    double maxdrawdown = 0.0, max_so_far = v[0];
    
    for(register int i = 1; i < size; ++i)
    {
        if(v[i] > max_so_far)
        {
            max_so_far = v[i];
        }
        else
        {
            if(max_so_far > 0)
            {
                const double dd = (v[i] - max_so_far) / max_so_far;
                if(dd < maxdrawdown)
                    maxdrawdown =dd;
            }
            
        }
    }
    return fabs(maxdrawdown);
}
