#include "InputSystem.h"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace MG;

std::map<std::string, int> InputSystem::m_conversion;
std::map<std::string, std::string> InputSystem::m_mouse_conversion;

void InputSystem::bindConversion()
{
	m_conversion["w"] = GLFW_KEY_W;
	m_conversion["a"] = GLFW_KEY_A;
	m_conversion["s"] = GLFW_KEY_S;
	m_conversion["d"] = GLFW_KEY_D;

	m_conversion["up"] = GLFW_KEY_UP;
	m_conversion["left"] = GLFW_KEY_LEFT;
	m_conversion["right"] = GLFW_KEY_RIGHT;
	m_conversion["down"] = GLFW_KEY_D;

	m_mouse_conversion["mX"] = "mX";
	m_mouse_conversion["mY"] = "mY";
}

InputSystem::InputSystem()
{
	m_mouseCoordScale["mX"] = std::make_tuple(0.0, 0.0);
	m_mouseCoordScale["mY"] = std::make_tuple(0.0, 0.0);
}

void isMouseMove(GLFWwindow* window, std::map<std::string, std::tuple<double, double>>& mouseCoordScale)
{
	std::string axis[] = { "mX", "mY" };
	double pos[2];
	glfwGetCursorPos(window, &pos[0], &pos[1]);

	for (int i = 0; i < 2; ++i)
	{
		if (pos[i] != std::get<0>(mouseCoordScale[axis[i]]))
		{
			std::get<1>(mouseCoordScale[axis[i]]) = pos[i] - std::get<0>(mouseCoordScale[axis[i]]);
			std::get<0>(mouseCoordScale[axis[i]]) = pos[i];
		}
	}
}

void InputSystem::update(GLFWwindow* window)
{
	//https://www.glfw.org/docs/3.3/input_guide.html

	std::map<int, std::tuple<std::string, float>>::iterator itt;
	for (itt = m_InputsMapped.begin(); itt != m_InputsMapped.end(); ++itt)
	{
		int state = glfwGetKey(window, itt->first);
		if (state == GLFW_PRESS)
		{
			auto& axisNameVal = itt->second;
			auto itt_binding = m_bindings.find(std::get<0>(axisNameVal));
			if (itt_binding != m_bindings.end())
			{
				itt_binding->second(std::get<1>(axisNameVal));
			}
		}
	}

	isMouseMove(window, m_mouseCoordScale);
	
	std::map<std::string, std::tuple<std::string, float>>::iterator ittMouse;
	for (ittMouse = m_MouseInputsMapped.begin(); ittMouse != m_MouseInputsMapped.end(); ++ittMouse)
	{
		auto ittCoord = m_mouseCoordScale.find(ittMouse->first);

		if (ittCoord != m_mouseCoordScale.end() && std::get<1>(ittCoord->second) != 0.0)
		{
			auto& axisNameVal = ittMouse->second;
			auto itt_binding = m_bindings.find(std::get<0>(axisNameVal));
			if (itt_binding != m_bindings.end())
			{
				itt_binding->second(std::get<1>(axisNameVal) * std::get<1>(ittCoord->second));
				std::get<1>(ittCoord->second) = 0.0;
			}
		}
	}
}