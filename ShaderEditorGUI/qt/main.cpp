////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <boost/thread.hpp>
#include "shaderEditorGUI.h"
#include <QtWidgets/qapplication.h>

bool quitApplication;

void RunQt(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShaderEditorGUI gui;
    gui.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    gui.show();
    app.exec();
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

int main(int argc, char *argv[])
{ 
    quitApplication = false;

    boost::thread qtThread(&RunQt, argc, argv);
    
    WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProc,
                            0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
                            LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW+1),
                            0, LPCWSTR("MyClass"), LoadIcon(0,IDI_APPLICATION) } ;

    RegisterClassEx(&wndclass);

    HWND window = CreateWindowEx(0, PCWSTR("MyClass"), TEXT("title"),
                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);

    ShowWindow(window, SW_SHOWDEFAULT);

    MSG msg;
    while(GetMessage(&msg, 0, 0, 0)) 
    {
        DispatchMessage(&msg);
    }

    qtThread.join();

    return 0;    
}