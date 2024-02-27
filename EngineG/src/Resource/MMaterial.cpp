#include "MMaterial.h"

#include "MResourceManager.h"

#include "../MGame/MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MRenderSystem.h"
#include "../OGraphicsEngine/RenderSystem/MShaderProgram.h"
#include "../OGraphicsEngine/RenderSystem/MUniformBuffer.h"
#include "../OGraphicsEngine/RenderSystem/MTexture2D.h"

#include "../Resource/MTexture.h"

using namespace MG;


MMaterial::MMaterial(const char* file_path1, const char* file_path2, MResourceManager* resourceManager)
	: MResource(file_path1, resourceManager)
{
	auto renderSystem = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();
	m_shader = renderSystem->createShaderProgram({ file_path1, file_path2 });
	addUniform("UniformData", sizeof(UniformData), 0);
}

MShaderProgSharedPtr& MG::MMaterial::getShaderProgram()
{
	return m_shader;
}

void MMaterial::addUniform(const std::string& name, ui32 size, ui32 slot)
{
	auto renderSystem = m_resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();

	auto uniform = renderSystem->createUniformBuffer(
		{
			size
		}
	);

	m_uniforms.insert(std::make_pair(name, uniform));
	setUniformSlot(name.c_str(), slot);
}

void MG::MMaterial::addTexture(const MTextureSharedPtr& texture)
{
	m_textures.push_back(texture);
}

void MMaterial::setUniformSlot(const char* name, ui32 slot)
{
	m_shader.get()->setUniformBufferSlot(name, slot);
}

void MMaterial::setUniformData(const std::string& name, void* data)
{
	m_uniforms[name].get()->setData(data);
}

const MUniformBufferSharedPtr& MMaterial::getUniform(const std::string& name)
{
	return m_uniforms[name];
}

const MTexture2DSharedPtr& MMaterial::getTexture() const
{
	return m_textures[0].get()->getTexture();
}

void MG::MMaterial::setCullType(const MCullType cullType)
{
	m_cullType = cullType;
}

void MG::MMaterial::setWindingOrder(const MWindingOrder windingOrder)
{
	m_windingOrder = windingOrder;
}

MCullType MG::MMaterial::getCullType() const
{
	return m_cullType;
}

MWindingOrder MG::MMaterial::getWindingOrder() const
{
	return m_windingOrder;
}
