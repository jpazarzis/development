//toolkit.h
#ifndef TOOLKIT_INCLUDED
#define TOOLKIT_INCLUDED

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


// trim from start
static inline std::string ltrim(const std::string &s) {
        std::string strg(s);
        return ltrim(strg);
}

// trim from end
static inline std::string rtrim(const std::string &s) {
    std::string strg(s);
    return ltrim(strg);
}

// trim from both ends
static inline std::string trim(const std::string &s) {
    std::string strg(s);
    return trim(strg);
}


#endif
