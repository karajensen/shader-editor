////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>

/**
* Static class for logging application issues
*/
class Logger
{
public:

    /**
    * Logs info to the outputstream
    * @param info The information to log
    */
    static void LogInfo(const std::string& info);

    /**
    * Logs error to the outputstream
    * @param error The error to log
    * @param popup Whether to show a popup windows dialog box with the error
    */
    static void LogError(const std::string& error, bool popup = false);
};