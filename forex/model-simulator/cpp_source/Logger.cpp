#include "Logger.h"


using namespace std;

Logger::Logger()
{
#ifndef DISABLE_LOGGING
    _log_file.open ("log_file");
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
    logger._log_file << std::endl;
#endif
    return logger;
}



Logger Logger::_logger_singleton;
