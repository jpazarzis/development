#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

#include <fstream>

class EndOfLine {};

class Logger
{
        static Logger _logger_singleton;

#ifndef DISABLE_LOGGING
        std::ofstream  _log_file;
#endif
        
        Logger();

    public:

        static Logger& get_logger();
            
        template<class T> friend Logger& operator << (Logger& logger, const T& t);

        friend Logger& operator << (Logger& logger, const EndOfLine& t);

        ~Logger ();

};

template<class T> Logger& operator << (Logger &logger, const T& t) 
{
#ifndef DISABLE_LOGGING
    logger._log_file << t ;
#endif
    return logger;
}



#define LOG Logger::get_logger()
#define EOL EndOfLine()




#endif
