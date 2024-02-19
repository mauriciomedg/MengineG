#include "MTexture.h"

#include "MResourceManager.h"

#include "../MGame/MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MRenderSystem.h"

using namespace MG;

MTexture::MTexture(const char* file_path, MResourceManager* resourceManager)
	: MResource(file_path, resourceManager)
{
	auto renderSystem = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();
	m_texture = renderSystem->createTexture2D(file_path);
}
