#include "Utilities.h"
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


void save_curve( std::vector<double> account_balance_curve, const std::string& filename)
{
  std::ofstream f;
  f.open (filename.c_str());
  for(int i = 0; i < account_balance_curve.size(); ++i)
  {
        f << account_balance_curve[i] << std::endl;
  }
 f.close();
}
