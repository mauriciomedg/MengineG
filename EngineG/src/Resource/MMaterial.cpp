#include "MMaterial.h"

#include "MResourceManager.h"

#include "../MGame/MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MRenderSystem.h"

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
