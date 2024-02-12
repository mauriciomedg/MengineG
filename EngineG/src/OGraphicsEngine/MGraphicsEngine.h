#pragma once
#include "MPrerequisites.h"
#include <vector>
#include <map>

struct GLFWwindow;

namespace MG
{
	class MGraphicsEngine
	{
	public:
		MGraphicsEngine();
		~MGraphicsEngine();
		
	public:
		void init();
		bool update(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders);

		ui32 createVextexArrayObject(const MVertexBufferDesc& data);
		ui32 createShaderProgram(const MShaderProgramDesc& data);

		void setVextexArrayObject(const ui32 id);
		void setShaderProgram(const ui32 id);
	private:
		void display(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders);
		void drawTriangles(ui32 vertexCount, ui32 offset);

		GLFWwindow* m_window;
		std::vector<MVAOSharedPtr> m_VAOlist;
		std::map<ui32, MShaderProgSharedPtr> m_shadersProgram;
		std::vector<ui32> m_modelsToRender;

	};
}