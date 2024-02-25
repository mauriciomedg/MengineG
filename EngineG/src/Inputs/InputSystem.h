#pragma once

#include <string>
#include <map>
#include "../Delegate.h"

struct GLFWwindow;

namespace MG
{

	class InputSystem
	{
	public:

		static InputSystem& get() {
			static InputSystem instance;
			bindConversion();
			return instance;
		}

		~InputSystem() {};

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

		void mapMouseInput(std::string axisName, std::string key, float value)
		{
			m_MouseInputsMapped[m_mouse_conversion[key]] = std::make_tuple(axisName, value);
		}

	private:

		InputSystem();
		InputSystem(const InputSystem&) {};

	public:
		void init() {};
		void update(GLFWwindow* window);
	private:
		static std::map<std::string, int> m_conversion;
		static std::map<std::string, std::string> m_mouse_conversion;
		static void bindConversion();
		std::map<std::string, delegate> m_bindings;
		std::map<int, std::tuple<std::string, float>> m_InputsMapped;
		std::map<std::string, std::tuple<std::string, float>> m_MouseInputsMapped;

		double m_mouseX = 0.0; 
		double m_mouseY = 0.0;
	};
}

