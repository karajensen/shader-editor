////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "application.h"
#include "gui.h"
#include "cache.h"
#include <thread>

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
void qtmain(int argc, char *argv[], std::shared_ptr<Cache> cache)
{
    std::unique_ptr<Gui> gui(new Gui(cache));
    gui->Run(argc, argv);
}

/**
* Application entry point
*/
int main(int argc, char *argv[])
{
    HWND hWnd;
    HINSTANCE hInstance;
    InitializeWindow(&hInstance, &hWnd);

    std::shared_ptr<Cache> cache(new Cache());
    std::unique_ptr<Application> game(new Application(cache));

    if(game->Initialise(hWnd, hInstance))
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