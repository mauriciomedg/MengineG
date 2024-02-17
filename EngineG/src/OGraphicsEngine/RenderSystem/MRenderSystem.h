#pragma once
#include "../MPrerequisites.h"
namespace MG
{
	class MRenderSystem
	{
	public:
		MRenderSystem();
		~MRenderSystem();

		MVAOSharedPtr createVextexArrayObject(const MVertexBufferDesc& vbDes);
		MVAOSharedPtr createVextexArrayObject(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes);
		MUniformBufferSharedPtr createUniformBuffer(const MUniformBufferDesc& data);
		MShaderProgSharedPtr createShaderProgram(const MShaderProgramDesc& data);

		MDeviceContextSharedPtr getDeviceContext();
	private:
		MDeviceContextSharedPtr m_deviceContext;
	};
}