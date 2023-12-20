#include "Inputs.h"
#include "GEntity.h"
#include <GLFW/glfw3.h>

std::map<std::string, int> Inputs::m_conversion;
void Inputs::bindConversion()
{
	m_conversion["w"] = GLFW_KEY_W;
	m_conversion["a"] = GLFW_KEY_A;
	m_conversion["s"] = GLFW_KEY_S;
	m_conversion["d"] = GLFW_KEY_D;

	m_conversion["up"] = GLFW_KEY_UP;
	m_conversion["left"] = GLFW_KEY_LEFT;
	m_conversion["right"] = GLFW_KEY_RIGHT;
	m_conversion["down"] = GLFW_KEY_D;
}

Inputs::Inputs()
{
}

void Inputs::update(GLFWwindow* window)
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
}