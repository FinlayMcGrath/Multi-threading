// 3D Example Application
// Paul Robertson 2014


//Include
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "Input.h"
#include "Scene3D.h"
#include "timerclass.h"

// maximum mumber of lines the output console should have
static const WORD MAX_CONSOLE_LINES = 500;

//Globals
HWND hwnd; 
Input myInput;
Scene3D myScene;
TimerClass timer;
Mandelbrot mandelbrot;

//Prototypes
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
BOOL WaitFor(unsigned long delay);

//code
void RegisterMyWindow(HINSTANCE hInstance)
{
    WNDCLASSEX  wcex;									

    wcex.cbSize        = sizeof (wcex);				
    wcex.style         = CS_HREDRAW | CS_VREDRAW;		
    wcex.lpfnWndProc   = WndProc;						
    wcex.cbClsExtra    = 0;								
    wcex.cbWndExtra    = 0;								
    wcex.hInstance     = hInstance;						
    wcex.hIcon         = 0; 
    wcex.hCursor       = LoadCursor (NULL, IDC_ARROW);	
															
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
    wcex.lpszMenuName  = NULL;							
    wcex.lpszClassName = "FirstWindowClass";				
    wcex.hIconSm       = 0; 

	
	RegisterClassEx (&wcex);							
}


BOOL InitialiseMyWindow(HINSTANCE hInstance, int nCmdShow)
{
  
   hwnd = CreateWindow ("FirstWindowClass",					
						 "My first Triangle",		  	
						 WS_MAXIMIZE,
						 CW_USEDEFAULT,			
						 CW_USEDEFAULT,			
						 1440,			
						 900,			
						 NULL,					
						 NULL,					
						 hInstance,				
						 NULL);		
	if (!hwnd)
	{
		return FALSE;
	}

    ShowWindow (hwnd, nCmdShow);						
    UpdateWindow (hwnd);									
	return TRUE;

}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)			
{									
    MSG         msg;	

	//AllocConsole();
	//AttachConsole(GetCurrentProcessId());
	//freopen("CON", "w", stdout);

	RegisterMyWindow(hInstance);

   	if (!InitialiseMyWindow(hInstance, nCmdShow))
	return FALSE;

	myScene.Init(&hwnd, &myInput);
	timer.Initialize();
	
	while (TRUE)					
    {							
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
		    if (msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		}
		else
		{	
			timer.Frame();
			myScene.DrawScene(timer.GetTime());
			if(myInput.isKeyDown(VK_ESCAPE))
			{
				break;
			}
		}
    }

	return msg.wParam ;										
}

				
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)											
    {
		case WM_CREATE:											
			break;

		case WM_KEYDOWN:
			myInput.SetKeyDown(wParam); // Pass key value into input obj
			break;

		case WM_KEYUP:
			myInput.SetKeyUp(wParam); // Pass key value into input obj
			break;

		case WM_MOUSEMOVE:
			myInput.setMouseX(LOWORD (lParam)); // Pass X position
			myInput.setMouseY(HIWORD (lParam)); // Pass Y position
			break;

		case WM_LBUTTONDOWN:
			if (myInput.getLeftClicked())
			{
				myInput.setLeftClick(false);
			}
			else
			{
				myInput.setLeftClick(true);
			}
			break;
		case WM_LBUTTONUP:
			myInput.setLeftClicked(false);
			break;

		case WM_RBUTTONDOWN:			
			if (myInput.getRightClicked())
			{
				myInput.setRightClick(false);
			}
			else
			{
				myInput.setRightClick(true);
			}
			break;

		case WM_RBUTTONUP:
			myInput.setRightClicked(false);
			myInput.setRightClick(false);
			break;

		case WM_PAINT:
		    break;		

		case WM_DESTROY:	
			PostQuitMessage(0);	
			break;				
	}														

	return DefWindowProc (hwnd, message, wParam, lParam);																
}