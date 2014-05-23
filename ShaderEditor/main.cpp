////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "vld/include/vld.h"
#endif

#include "application.h"
#include "common.h"

void InitializeWindow(HINSTANCE);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    Logger::Initialise();
    InitializeWindow(hInstance);

    std::unique_ptr<Application> game(new Application());
    if(!game->Initialise(hWnd, hInstance))
    {
        Logger::LogError("Application failed to initialise");
        return EXIT_FAILURE;
    }

    ShowWindow(hWnd, cmdShow);

    if(!game->Run())
    {
        Logger::LogError("Application loop exited unexpectedly");
        return EXIT_FAILURE;
    }

    Logger::LogInfo("Application exited successfully");
    return EXIT_SUCCESS;
}

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

void InitializeWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX)); 
    wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance; 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "ShaderEditor";
    RegisterClassEx(&wc); 

    DWORD style = WS_EX_TOPMOST | WS_POPUP;
    hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
        "ShaderEditor", TEXT("Shader Editor"), 
        style, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, hInstance, nullptr);
}