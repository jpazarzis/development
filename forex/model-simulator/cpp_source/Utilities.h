// Utilities.h
//
// Author        : John Pazarzis
// Creation date : Oct.10 2013
//
// Summary
//      Summary goes here
//
// Notes
//

#ifndef UTILITIES_INCLUDED
#define UTILITIES_INCLUDED

#include <string>


std::string sformat(long value, const std::string& format = "%10d");
std::string sformat(int value, const std::string& format = "%10d");;
std::string sformat(double value, const std::string& format = "%10.4d");
std::string sformat(const std::string& strg, const std::string& format = "%10s");

#endif // UTILITIES_INCLUDED
