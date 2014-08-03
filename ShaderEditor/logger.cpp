////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "logger.h"
#include "common.h"
#include <iostream>
#include <Windows.h>

std::mutex Logger::sm_logMutex;

void Logger::LogInfo(const std::string& info)
{
    #ifdef USE_CONSOLE
        std::lock_guard<std::mutex> lock(sm_logMutex);
        std::cout << "INFO: \t" << info << std::endl;
    #endif
}

void Logger::LogError(const std::string& error)
{
    #ifdef USE_CONSOLE
        std::lock_guard<std::mutex> lock(sm_logMutex);
        std::cout << "ERROR: \t" <<  error << std::endl;
    #endif
}