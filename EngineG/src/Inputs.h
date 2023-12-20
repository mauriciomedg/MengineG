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
struct GLFWwindow;

typedef  void (GEntity::* ActionAxis)(float val);

class Inputs
{
public:

	static Inputs& get() {
		static Inputs instance;
		bindConversion();
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

	void mapInput(std::string axisName, std::string key, float value)
	{
		m_InputsMapped[m_conversion[key]] = std::make_tuple(axisName, value);
	}

private:

	Inputs();
	Inputs(const Inputs&) {};
	
public:
	void init() {};
	void update(GLFWwindow* window);
private:
	static std::map<std::string, int> m_conversion;
	static void bindConversion();
	std::map<std::string, delegate> m_bindings;
	std::map<int, std::tuple<std::string, float>> m_InputsMapped;
};


