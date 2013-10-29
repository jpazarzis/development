//toolkit.h
#ifndef TOOLKIT_INCLUDED
#define TOOLKIT_INCLUDED

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>
#include <vector>
#include <numeric>

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

boost::gregorian::date make_date(std::string date_str);

std::string sformat(long value, const std::string& format = "%10d");
std::string sformat(int value, const std::string& format = "%10d");;
std::string sformat(double value, const std::string& format = "%10.4d");
std::string sformat(const std::string& strg, const std::string& format = "%10s");
std::string timestamp();


template <typename T> double mean(const std::vector<T>& v)
{
    return std::accumulate(v.begin(), v.end(), 0.0) / (v.size() * 1.0);
}

template <typename T> double variance(const std::vector<T>& v)
{
    const double m = mean(v);
    const int size = v.size();
    double temp = 0;
    for(int i = 0; i < size; i++)
        temp += pow(v[i] - m,2 );
   return temp / (size-1);
}

template <typename T> double stdev(const std::vector<T>& v)
{
    return sqrt(variance(v));
}

template <typename T> T median(std::vector<T> v)
{
    const int size = v.size();
    sort(v.begin(), v.end());
    if (size  % 2 == 0)
    {
          return (v[size / 2 - 1] + v[size / 2]) / 2;
    }
    else 
    {
          return v[size / 2];
    }
}

double max_drawdown(const std::vector<double>& v);

#endif
