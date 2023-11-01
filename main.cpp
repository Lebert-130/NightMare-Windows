#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#define _WIN32_WINNT 0x0500
#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>

#include "resource.h"

HINSTANCE hInst;
HBITMAP hBitmap = NULL;

UINT_PTR ID_TIMER;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int height;
int width;

void CALLBACK show(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
    PlaySound(PCHAR("BOO"), NULL, SND_RESOURCE | SND_ASYNC);
    ShowWindow(hwnd,SW_SHOW);
    SetTimer(hwnd,0,800,(TIMERPROC)&hide);
}

void CALLBACK hide(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
    ShowWindow(hwnd,SW_HIDE);
    SetTimer(hwnd,0,300000,(TIMERPROC)&show);
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    Sleep(300000);

    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    HBRUSH black = CreateSolidBrush(RGB(0,0,0));

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = black;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    PlaySound(PCHAR("BOO"), NULL, SND_RESOURCE | SND_ASYNC);

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindow(wincl.lpszClassName, "BOO",
    //window with title (overlapping window)
    WS_POPUP | WS_VISIBLE | WS_EX_TOPMOST,
    //window without title
    //WS_VISIBLE | WS_POPUP | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hThisInstance, NULL);

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    SetTimer(hwnd,0,800,(TIMERPROC)&hide);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            hBitmap = (HBITMAP)LoadImage(GetModuleHandle(0),MAKEINTRESOURCE(103),IMAGE_BITMAP,0,0,0);
            if(!hBitmap) MessageBox(NULL, _T("Error while loading skull"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
            break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc;
            BITMAP bitmap;
            HDC hdcMem;
            HGDIOBJ oldBitmap;

            hdc = BeginPaint(hwnd, &ps);

            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, hBitmap);

            GetObject(hBitmap, sizeof(bitmap), &bitmap);
            BitBlt(hdc,(width/2)-(bitmap.bmWidth/2),(height/2)-(bitmap.bmHeight/2),bitmap.bmWidth,bitmap.bmHeight,hdcMem,0,0,SRCCOPY);

            SelectObject(hdcMem,oldBitmap);
            DeleteDC(hdcMem);

            EndPaint(hwnd,&ps);
            break;
        case WM_DESTROY:
            DeleteObject(hBitmap);
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
