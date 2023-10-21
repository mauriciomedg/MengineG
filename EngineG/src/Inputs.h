#pragma once

#include <thread>         // std::thread
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <map>

class GEntity;

typedef  void (GEntity::* ActionAxis)(float val);

class Inputs
{
public:

	static Inputs& get() {
		static Inputs instance;
		return instance;
	}

	~Inputs() {};

	void addBinding(std::string axisName, ActionAxis actionInput, GEntity* obj);

private:

	Inputs() {};
	Inputs(const Inputs&) {};
	
public:

	void updateIO();

	void init()
	{
		
	}

private:

	std::map<std::string, std::tuple<ActionAxis, GEntity*>> m_bindings;
private:

	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	
	VOID ErrorExit(LPCSTR);
	VOID KeyEventProc(KEY_EVENT_RECORD);
	VOID MouseEventProc(MOUSE_EVENT_RECORD);
	VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

};


