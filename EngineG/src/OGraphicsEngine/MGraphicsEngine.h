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
		bool shouldCloseWindow();
		void clear();
		void drawTriangles(const MTriangleType& triangleType, ui32 vertexCount, ui32 offset);
		void drawIndexedTriangles(const MTriangleType& triangleType, ui32 indexCount);
		void swapBuffer();

		MVAOSharedPtr createVextexArrayObject(const MVertexBufferDesc& vbDes);
		MVAOSharedPtr createVextexArrayObject(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes);
		MUniformBufferSharedPtr createUniformBuffer(const MUniformBufferDesc& data);
		MShaderProgSharedPtr createShaderProgram(const MShaderProgramDesc& data);

		void setVextexArrayObject(const MVAOSharedPtr& vao);
		void setShaderProgram(const MShaderProgSharedPtr& shader);
		void setUniformBuffer(const MUniformBufferSharedPtr& uniform, ui32 slot);

		void setFaceCulling(const MCullType& type);
		void setWindingOrder(const MWindingOrder& order);

	private:
		
		GLFWwindow* m_window;
	};
}