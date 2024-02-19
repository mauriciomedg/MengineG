#pragma once
#include "../MPrerequisites.h"

struct GLFWwindow;
namespace MG
{
	class MDeviceContext
	{
	public:
		MDeviceContext(MRenderSystem* renderSystem);
		~MDeviceContext();

		bool shouldCloseWindow();
		void clear();
		void drawTriangles(const MTriangleType& triangleType, ui32 vertexCount, ui32 offset);
		void drawIndexedTriangles(const MTriangleType& triangleType, ui32 indexCount);
		void swapBuffer();

		void setVextexArrayObject(const MVAOSharedPtr& vao);
		void setShaderProgram(const MShaderProgSharedPtr& shader);
		void setUniformBuffer(const MUniformBufferSharedPtr& uniform, ui32 slot);
		void setTexture2D(const MTexture2DSharedPtr& texture);

		void setFaceCulling(const MCullType& type);
		void setWindingOrder(const MWindingOrder& order);

	private:
		MRenderSystem* m_renderSystem;
		GLFWwindow* m_window;
	};
}