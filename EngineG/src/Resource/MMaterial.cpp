#include "MMaterial.h"

#include "MResourceManager.h"

#include "../MGame/MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MRenderSystem.h"
#include "../OGraphicsEngine/RenderSystem/MShaderProgram.h"
#include "../OGraphicsEngine/RenderSystem/MUniformBuffer.h"

using namespace MG;


MMaterial::MMaterial(const char* file_path1, const char* file_path2, MResourceManager* resourceManager)
	: MResource(file_path1, resourceManager)
{
	auto renderSystem = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();
	m_shader = renderSystem->createShaderProgram({ file_path1, file_path2 });
}

MShaderProgSharedPtr& MG::MMaterial::getShaderProgram()
{
	return m_shader;
}

void MMaterial::addUniform(const MUniformBufferSharedPtr& uniform)
{
	m_uniform = uniform;
}

void MMaterial::setUniformSlot(const char* name, ui32 slot)
{
	m_shader.get()->setUniformBufferSlot("UniformData", 0);
}

void MMaterial::setUniformData(void* data)
{
	m_uniform.get()->setData(data);
}

const MUniformBufferSharedPtr& MMaterial::getUniform() const
{
	return m_uniform;
}
