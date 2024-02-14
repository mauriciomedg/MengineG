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
		bool update(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders, const std::vector<ui32>& uniforms);

		ui32 createVextexArrayObject(const MVertexBufferDesc& vbDes);
		ui32 createVextexArrayObject(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes);
		ui32 createUniformBuffer(const MUniformBufferDesc& data);
		ui32 createShaderProgram(const MShaderProgramDesc& data);

		void setVextexArrayObject(const ui32 id);
		void setShaderProgram(const ui32 id);
		void setShaderUniformBufferSlot(const ui32 shaderId, const ui32 uniformId, const char* name, ui32 slot);
		void setUniformData(const ui32 uniformId, void* data);

	private:
		void display(const std::vector<ui32>& modelsToRender, const std::vector<ui32>& shaders, const std::vector<ui32>& uniforms);
		void drawTriangles(const MTriangleType& triangleType, ui32 vertexCount, ui32 offset);
		void drawIndexedTriangles(const MTriangleType& triangleType, ui32 indecesCount);

		GLFWwindow* m_window;
		std::vector<MVAOSharedPtr> m_VAOlist;
		std::map<ui32, MUniformBufferSharedPtr> m_uniformBuffers;
		std::map<ui32, ui32> m_uniformBufferSlots;
		std::map<ui32, MShaderProgSharedPtr> m_shadersProgram;
		std::vector<ui32> m_modelsToRender;

	};
}