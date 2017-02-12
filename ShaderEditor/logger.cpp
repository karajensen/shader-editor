////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "logger.h"
#include "common.h"
#include <iostream>
#include <Windows.h>
#include <array>

std::mutex Logger::sm_logMutex;

void Logger::LogInfo(const std::string& info)
{
    #ifdef USE_CONSOLE
        std::lock_guard<std::mutex> lock(sm_logMutex);
        std::cout << "INFO: \t" << info << std::endl;
    #endif
        OutputDebugStringA((info + "\n").c_str());
}

void Logger::LogError(const std::string& error)
{
    #ifdef USE_CONSOLE
        std::lock_guard<std::mutex> lock(sm_logMutex);
        std::cout << "ERROR: \t" <<  error << std::endl;
    #endif
        OutputDebugStringA((error + "\n").c_str());
}

void Logger::LogInfo(const char* info, ...)
{
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    va_list args;
    va_start(args, info);
    vsnprintf(buffer, sizeof(buffer), info, args);
    va_end(args);

    LogInfo(std::string(buffer, strlen(buffer)));
}

void Logger::LogError(const char* error, ...)
{
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));

    va_list args;
    va_start(args, error);
    vsnprintf(buffer, sizeof(buffer), error, args);
    va_end(args);

    LogError(std::string(buffer, strlen(buffer)));
}