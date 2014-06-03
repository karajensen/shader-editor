////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - logger.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "logger.h"
#include <iostream>
#include <Windows.h>

void Logger::LogInfo(const std::string& info)
{
    std::cout << "INFO: \t" << info << std::endl;
}

void Logger::LogError(const std::string& error, bool popup)
{
    std::cout << "ERROR: \t" <<  error << std::endl;

    if(popup)
    {
        #ifndef _DEBUG
        MessageBox(nullptr, error.c_str(), TEXT("ERROR"), MB_OK);
        #endif
    }
}