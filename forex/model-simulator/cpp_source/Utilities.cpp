#include "Utilities.h"
#include <stdlib.h>
#include <stdio.h>      

std::string sformat(double value)
{
    char buffer[1024];
    sprintf(buffer, "%10.6f", value);
    return buffer;
}

std::string sformat(int value)
{
    char buffer[1024];
    sprintf(buffer, "%10d", value);
    return buffer;
}


std::string sformat(const std::string& strg)
{
    char buffer[1024];
    sprintf(buffer, "%10s", strg.c_str());
    return buffer;
}
