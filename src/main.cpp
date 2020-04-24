////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "render_data.h"
#include "application.h"
#include "cache.h"

#include "qt/qt_gui.h"

#include <thread>
#include <iostream>

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

    const int x = (GetSystemMetrics(SM_CXSCREEN)/2)-(WINDOW_WIDTH/2); 
    const int y = (GetSystemMetrics(SM_CYSCREEN)/2)-(WINDOW_HEIGHT/2);

    DWORD style = WS_EX_TOPMOST | WS_POPUP;
    *hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
        "ShaderEditor", TEXT("Shader Editor"), 
        style, x, y, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, *hInstance, nullptr);
}

/**
* Runs Qt on a seperate thread to the main application
*/
void qtmain(int argc, char *argv[], std::shared_ptr<Cache> cache)
{
    std::unique_ptr<QtGui> gui(new QtGui(cache));
    gui->Run(argc, argv);
}

/**
* Application entry point
*/
int main(int argc, char *argv[])
{
    Random::Initialise();

    HWND hWnd;
    HINSTANCE hInstance;
    InitializeWindow(&hInstance, &hWnd);

    auto cache = std::make_shared<Cache>();
    auto game = std::make_unique<Application>();

    if(game->Initialise(hWnd, hInstance, cache))
    {
        std::thread thread(&qtmain, argc, argv, cache);
    
        ShowWindow(hWnd, SW_SHOWDEFAULT);
    
        game->Run();
    
        thread.join();
    
        return EXIT_SUCCESS;
    }
    
    #ifdef USE_CONSOLE
    std::cin.get(); // pause the console
    #endif

    return EXIT_FAILURE;
};