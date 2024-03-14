#include "MRenderSystem.h"
#include "MDeviceContext.h"
#include "MVertexArrayObject.h"
#include "MShaderProgram.h"
#include "MUniformBuffer.h"
#include "MTexture2D.h"

using namespace MG;

MRenderSystem::MRenderSystem()
{
	m_deviceContext = std::make_shared<MDeviceContext>(this);
}

MRenderSystem::~MRenderSystem()
{
}

MVAOSharedPtr MRenderSystem::createVextexArrayObject(const MVertexBufferDesc& vbDes)
{
	return std::make_shared<MVertexArrayObject>(vbDes);
}

MVAOSharedPtr MRenderSystem::createVextexArrayObject(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes)
{
	return std::make_shared<MVertexArrayObject>(vbDes, ibDes);
}

MUniformBufferSharedPtr MRenderSystem::createUniformBuffer(const MUniformBufferDesc& data)
{
	return std::make_shared<MUniformBuffer>(data);
}

MShaderProgSharedPtr MRenderSystem::createShaderProgram(const MShaderProgramDesc& data)
{
	return std::make_shared<MShaderProgram>(data);
}

MTexture2DSharedPtr MG::MRenderSystem::createTexture2D(const char* file_path)
{
	return std::make_shared<MTexture2D>(file_path);
}

MDeviceContextSharedPtr MRenderSystem::getDeviceContext()
{
	return m_deviceContext;
}
