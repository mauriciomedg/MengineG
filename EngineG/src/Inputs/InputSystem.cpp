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
}

struct MousePosAxis
{
	std::string x;
	std::string y;
	double scaleX;
	double scaleY;
};

void isMouseMove(GLFWwindow* window, MousePosAxis& mpa, double& mouseX, double& mouseY)
{

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (xpos > mouseX)
	{
		mouseX = xpos;
		mpa.scaleX = 1.0;
		mpa.x = std::string("mX");
	}
	else if (xpos < mouseX)
	{
		mouseX = xpos;
		mpa.scaleX = -1.0;
		mpa.x = std::string("mX");
	}

	if (ypos > mouseY)
	{
		mouseY = ypos;
		mpa.scaleY = 1.0;
		mpa.y = std::string("mY");
	}
	else if (ypos < mouseY)
	{
		mouseY = ypos;
		mpa.scaleY = -1.0;
		mpa.y = std::string("mY");
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


	std::map<std::string, std::tuple<std::string, float>>::iterator ittMouse;
	
	MousePosAxis mpa = {};
	isMouseMove(window, mpa, m_mouseX, m_mouseY);//glfwGetKey(window, itt->first);


	for (ittMouse = m_MouseInputsMapped.begin(); ittMouse != m_MouseInputsMapped.end(); ++ittMouse)
	{
		if (mpa.x == ittMouse->first)
		{
			auto& axisNameVal = ittMouse->second;
			auto itt_binding = m_bindings.find(std::get<0>(axisNameVal));
			if (itt_binding != m_bindings.end())
			{
				itt_binding->second(std::get<1>(axisNameVal) * mpa.scaleX);
			}
		}

		if (mpa.y == ittMouse->first)
		{
			auto& axisNameVal = ittMouse->second;
			auto itt_binding = m_bindings.find(std::get<0>(axisNameVal));
			if (itt_binding != m_bindings.end())
			{
				itt_binding->second(std::get<1>(axisNameVal) * mpa.scaleY);
			}
		}
	}
}