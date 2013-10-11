#include "Logger.h"
using namespace std;


std::string Logger::_filename;



void Logger::set_filename(const std::string& filename)
{
    _filename = filename;
}

Logger::Logger()
{
 
}

void Logger::open_output()
{
#ifndef DISABLE_LOGGING
       if(_filename.length() == 0)
       {
            _filename = "log_file";
       }
       _log_file.open (_filename.c_str());
#endif
}

Logger& Logger::get_logger()
{
    return _logger_singleton;
}

Logger::~Logger () 
{
#ifndef DISABLE_LOGGING
        if (_log_file.is_open()) 
        {
            _log_file.close();
        }
#endif
}


Logger& operator << (Logger& logger, const EndOfLine& t)
{
#ifndef DISABLE_LOGGING
    if (!logger._log_file.is_open()) 
    {
        logger.open_output();
    }        
        

    logger._log_file << std::endl;
#endif
    return logger;
}



Logger Logger::_logger_singleton;
