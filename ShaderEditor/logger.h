////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <fstream>
#include <memory>

/**
* Static class for logging application issues
*/
class Logger
{
public:

    /**
    * Destructor
    */
    ~Logger();

    /**
    * Initialises logging for the application
    */
    static void Initialise();

    /**
    * Logs info to a file and debug output window if windows
    * @param info The information to log
    */
    static void LogInfo(const std::string& info);

    /**
    * Logs an error to a file and debug output window if windows
    * @param error The error to log
    * @param popup Whether to show a popup windows dialog box with the error
    */
    static void LogError(const std::string& error, bool popup = false);

private:

    /**
    * Constructor
    */
    Logger();

    std::fstream m_logfile; ///< filestream for the logfile
    static std::unique_ptr<Logger> sm_logger;
};