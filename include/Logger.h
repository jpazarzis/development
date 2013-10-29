#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

#include <fstream>
#include <string>

class EndOfLine {};

class Logger
{
        static std::string _filename;
        static Logger _logger_singleton;

#ifndef DISABLE_LOGGING
        std::ofstream  _log_file;
#endif
        
        Logger();

        void open_output();

    public:

        static void set_filename(const std::string& filename);

        static Logger& get_logger();
            
        template<class T> friend Logger& operator << (Logger& logger, const T& t);

        friend Logger& operator << (Logger& logger, const EndOfLine& t);

        ~Logger ();

};

template<class T> Logger& operator << (Logger &logger, const T& t) 
{
#ifndef DISABLE_LOGGING
    if (!logger._log_file.is_open()) 
    {
        logger.open_output();
    }        
    logger._log_file << t ;
#endif
    return logger;
}



#define LOG Logger::get_logger()
#define EOL EndOfLine()




#endif
