/****************************************************************
* Kara Jensen (mail@karajensen.com)
* Static class for logging
*****************************************************************/
#pragma once

#include <fstream>
#include <boost/scoped_ptr.hpp>

class Logger
{
public:

    ~Logger();

    /**
    * Initialises logging for the application
    */
    static void Initialise();

    /**
    * Logs to a file and debug output window if windows
    * Info prepends INFO to message, Error prepends ERROR
    */
    static void LogInfo(const std::string& info);
    static void LogError(const std::string& error);

private:

    Logger();

    std::fstream m_logfile;
    static boost::scoped_ptr<Logger> sm_logger;
};