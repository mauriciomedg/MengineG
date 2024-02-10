#pragma once
#include "MPrerequisites.h"
#include <vector>

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
		bool update(const std::vector<ui32>& modelsToRender);

		ui32 createVextexArrayObject(const MVertexArrayBufferData& data);
		void setVextexArrayObject(const ui32 id);
	private:
		void display(const std::vector<ui32>& modelsToRender);
		void drawTriangles(ui32 vertexCount, ui32 offset);

		GLFWwindow* m_window;
		std::vector<MVAOSharedPtr> m_VAOlist;
		std::vector<ui32> m_modelsToRender;
	};
}