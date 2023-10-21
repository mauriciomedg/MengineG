#pragma once

#include <thread>         // std::thread
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

class Inputs
{
public:

	static Inputs& get() {
		static Inputs instance;
		return instance;
	}

	~Inputs() {};

private:

	Inputs() {};
	Inputs(const Inputs&) {};
	
public:

	void updateIO()
	{
		////// <IO>
		DWORD cNumRead, fdwMode, i;
		INPUT_RECORD irInBuf[128];
		int counter = 0;

		// Get the standard input handle. 

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			ErrorExit("GetStdHandle");

		// Save the current input mode, to be restored on exit. 

		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			ErrorExit("GetConsoleMode");

		// Enable the window and mouse input events. 

		fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS;
		if (!SetConsoleMode(hStdin, fdwMode))
			ErrorExit("SetConsoleMode");
		/// <returns></returns>

		while (true)//counter++ <= 100)
		{
			if (!ReadConsoleInput(
				hStdin,      // input buffer handle 
				irInBuf,     // buffer to read into 
				128,         // size of read buffer 
				&cNumRead)) // number of records read 
				ErrorExit("ReadConsoleInput");

			// Dispatch the events to the appropriate handler. 

			for (i = 0; i < cNumRead; i++)
			{
				switch (irInBuf[i].EventType)
				{
				case KEY_EVENT: // keyboard input 
					KeyEventProc(irInBuf[i].Event.KeyEvent);
					break;

				case MOUSE_EVENT: // mouse input 
					MouseEventProc(irInBuf[i].Event.MouseEvent);
					break;

				case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
					ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
					break;

				case FOCUS_EVENT:  // disregard focus events 

				case MENU_EVENT:   // disregard menu events 
					break;

				default:
					ErrorExit("Unknown event type");
					break;
				}
			}
		}
	}

	void init()
	{
		
	}

private:

	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	
	VOID ErrorExit(LPCSTR);
	VOID KeyEventProc(KEY_EVENT_RECORD);
	VOID MouseEventProc(MOUSE_EVENT_RECORD);
	VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

};

VOID Inputs::ErrorExit(LPCSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID Inputs::KeyEventProc(KEY_EVENT_RECORD ker)
{
	printf("Key event: ");

	if (ker.bKeyDown)
		printf("key pressed\n");
	else printf("key released\n");
}

VOID Inputs::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press \n");
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved\n");
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
}

VOID Inputs::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	printf("Resize event\n");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}
