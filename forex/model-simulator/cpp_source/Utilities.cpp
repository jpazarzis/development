#include "Utilities.h"
#include <stdlib.h>
#include <stdio.h>      

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
