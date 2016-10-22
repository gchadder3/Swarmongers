// ===================
//     Swarmongers
// ===================
//
// George Chadderdon and Magic Lantern Playware,  C 2000
//
// Revised in 2009 so it can compile and run under Visual C++ 
// 2008 Express Edition!!  Note also that the windowed mode 
// works better, though you need to switch your Windows display 
// mode to 1024x768 in 8 bit mode for things to (maybe) look right.
// (I haven't had a chance to try it yet on a machine that lets 
// me set Windows to a 1024x768 8-bit mode which is what this 
// code uses.)
//
// This game was developed as a demo/ramp-up exercise when I 
// began work at Magic Lantern Playware.  I used the DirectX 
// code from Andre LaMothe's "Windows Game Programming for 
// Dummies" and developed a simple vector engine off that and 
// the game logic.  Special thanks go to Duncan McPherson for 
// the sounds and music.
//
// This game is incomplete, but feel free to modify, improve, 
// and explore...
//
// ===================
//
// main.cpp 
//
// Contains the WinMain() and WindProc() code for the game.
// Most of this code is taken from LaMothe with some changes
// in formatting style.
//
// Make sure to include ddraw.lib, dinput8.lib, dsound.lib, 
// and winmm.lib in the link.  The March 2009 version of the 
// DirectX SDK was used in the revision.


// INCLUDES

// GPDUMB includes.
#include "GPDUMB_includes.h"

// Swarmongers includes
#include "Game.h"
#include "GameScreen.h"
#include "SoundManager.h"
#include "ActionEventManager.h"
#include "DebugLog.h"


// DEFINES 

// Defines for windows 
const int WINDOW_WIDTH = 1024;   // size of window
const int WINDOW_HEIGHT = 768;

#define WINDOW_CLASS_NAME "WINXCLASS"  // class name

// GLOBALS 
HWND main_window_handle           = NULL; // save the window handle
HINSTANCE main_instance           = NULL; // save the instance
BOOL full_screen = TRUE;  // is the application a full-screen application?
Game *theGame;  // a pointer to the game object
GameScreen *theScreen;  // a pointer to the game screen object
StarField *theStars;  // a pointer to the star field object
SoundManager *sndManager; // a pointer to the game's sound manager
ActionEventManager *actionManager; // a pointer to the game's action events manager
DebugLog *dbgLog; // a pointer to a simple debug log object


// FUNCTIONS 

LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	// This is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   // handle to a device context

	// What is the message?
	switch(msg)
	{	
	case WM_CREATE: 
		// Do initialization stuff here...
		return(0);
		break;
    case WM_PAINT:
		// Start painting
        hdc = BeginPaint(hwnd,&ps);

        // End painting
        EndPaint(hwnd,&ps);
        return(0);
		break;
	case WM_DESTROY: 
		// Kill the application			
		PostQuitMessage(0);
		return(0);
		break;
	default:
		break;
	} // end switch

	// Process any messages that we didn't take care of. 
	return (DefWindowProc(hwnd, msg, wparam, lparam));
} // end WinProc


// WINMAIN 

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{
	WNDCLASS winclass;	// this will hold the class we create
	HWND	 hwnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hdc;       // generic dc
	PAINTSTRUCT ps;     // generic paintstruct

	WNDCLASSEX wc;
	RECT rc;

	// Fill in the window class structure.
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
		                      CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= _T(WINDOW_CLASS_NAME);

	// Register the window class
	if (!RegisterClass(&winclass))
		return(0);

	// If we're in full screen mode, create a plain popup window...
	if (full_screen)
	{
		// Create the window, note the use of WS_POPUP
		if (!(hwnd = CreateWindow(_T(WINDOW_CLASS_NAME),     // class
								  _T("WinX Game Console"),	 // title
								  WS_POPUP | WS_VISIBLE,
								  0,0,	   // x,y
								  WINDOW_WIDTH,  // width
								  WINDOW_HEIGHT, // height
								  NULL,	   // handle to parent 
								  NULL,	   // handle to menu
								  hinstance,// instance
								  NULL)))	// creation parms
			return(0);
	}
	// Otherwise, create a full window with title-bar, etc...
	else
	{
		if (!(hwnd = CreateWindow(_T(WINDOW_CLASS_NAME),     // class
								  _T("Swarmongers"),	     // title
								  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								  0,0,	   // x,y
								  WINDOW_WIDTH,  // width
								  WINDOW_HEIGHT, // height
								  NULL,	   // handle to parent 
								  NULL,	   // handle to menu
								  hinstance,// instance
								  NULL)))	// creation parms
			return(0);
	}

	// Save the window handle and instance in a global
	main_window_handle = hwnd;
	main_instance      = hinstance;

	// Perform all game console specific initialization
	theGame = new(Game);

	// Enter main event loop
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// Test if this is a quit
			if (msg.message == WM_QUIT)
				break;
	
			// Translate any accelerator keys
			TranslateMessage(&msg);

			// Send the message to the window proc
			DispatchMessage(&msg);
		} // end if
    
		// Main game processing goes here
		theGame->MainLoop();

	} // end while

	// Shutdown game and release all resources
	delete theGame;

	// Return to Windows like this
	return(msg.wParam);
} // end WinMain

