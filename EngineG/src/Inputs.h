#pragma once

#include <thread>         // std::thread
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <map>
#include <mutex>
#include <queue>
#include "Delegate.h"

class GEntity;
struct AxisParams;

typedef  void (GEntity::* ActionAxis)(float val);

class Inputs
{
public:

	static Inputs& get() {
		static Inputs instance;
		return instance;
	}

	~Inputs() {};

	//template <class T, typename s = void (T::*)(float)>
	//void addBinding(std::string axisName, s a, T* obj)
	//{
	////https://simoncoenen.com/blog/programming/CPP_Delegates
	//// https://medium.com/@kayraurf/delegates-in-c-polos-56211a7536ba
	////	obj->ActionAxis(3);
	//	//m_bindings[axisName] = std::make_tuple(ActionAxis, obj);
	//}

	void addBinding(std::string axisName, delegate d)
	{
		m_bindings[axisName] = d;
	}

	void mapInput(std::string axisName, char key, float value)
	{
		m_InputsMapped[key] = std::make_tuple(axisName, value);
	}

private:

	Inputs();
	Inputs(const Inputs&) {};
	
public:

	void updateIO();
	void init() {};
	void update();
private:

	std::map<std::string, delegate> m_bindings;
	std::map<char, std::tuple<std::string, float>> m_InputsMapped;
private:

	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	std::mutex mtx;
	std::queue<char> m_inputsAxisNames;
	VOID ErrorExit(LPCSTR);
	VOID KeyEventProc(KEY_EVENT_RECORD);
	VOID MouseEventProc(MOUSE_EVENT_RECORD);
	VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

};


