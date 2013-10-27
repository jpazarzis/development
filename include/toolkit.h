//toolkit.h
#ifndef TOOLKIT_INCLUDED
#define TOOLKIT_INCLUDED

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "boost/date_time/gregorian/gregorian.hpp"


// trim from start
inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

// trim from start
inline std::string ltrim(const std::string &s) {
        std::string strg(s);
        return ltrim(strg);
}

// trim from end
inline std::string rtrim(const std::string &s) {
    std::string strg(s);
    return ltrim(strg);
}

// trim from both ends
inline std::string trim(const std::string &s) {
    std::string strg(s);
    return trim(strg);
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


#endif
