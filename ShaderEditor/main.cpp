////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "vld/include/vld.h"
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "application.h"
#include <thread>
#include <QtWidgets/qapplication.h>
#include "qt/gui.h"

static bool sm_runQt = true;
static std::mutex sm_qtMutex;

/**
* Allows the main application to send an exit call to qt
*/
void ExitQt()
{
    std::lock_guard<std::mutex> lock(sm_qtMutex);
    sm_runQt = false;
}

/**
* Determines whether qt is required to be running
*/
bool RunQt()
{
    std::lock_guard<std::mutex> lock(sm_qtMutex);
    return sm_runQt;
}

/**
* Main application window message handler
*/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{  
    switch(message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
* Initialises the main window
*/
void InitializeWindow(HINSTANCE* hInstance, HWND* hWnd)
{
    *hInstance = GetModuleHandle(0);

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX)); 
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = *hInstance; 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "ShaderEditor";
    RegisterClassEx(&wc); 

    DWORD style = WS_EX_TOPMOST | WS_POPUP;
    *hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
        "ShaderEditor", TEXT("Shader Editor"), 
        style, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, *hInstance, nullptr);
}

/**
* Runs Qt on a seperate thread to the main application
*/
void qtmain(int argc, char *argv[])
{
    Logger::LogInfo("Initialising Qt");
    QApplication app(argc, argv);

    Gui gui;
    gui.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    gui.show();
    
    while(RunQt())
    {
        app.processEvents();
    }

    app.exit();
    Logger::LogInfo("Exiting Qt");
}

/**
* Application entry point
*/
int main(int argc, char *argv[])
{
    HWND hWnd;
    HINSTANCE hInstance;
    InitializeWindow(&hInstance, &hWnd);

    std::unique_ptr<Application> game(new Application());
    if(game->Initialise(hWnd, hInstance))
    {
        std::thread thread(&qtmain, argc, argv);

        ShowWindow(hWnd, SW_SHOWDEFAULT);
    
        game->Run();

        ExitQt();

        thread.join();

        return EXIT_SUCCESS;
    }

    #ifdef _DEBUG
    std::cin.get(); // pause the console
    #endif

    return EXIT_FAILURE;
};