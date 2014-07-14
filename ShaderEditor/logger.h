////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <mutex>

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
    */
    static void LogError(const std::string& error);

private:

    static std::mutex sm_logMutex; ///< For getting sole access to the console
};