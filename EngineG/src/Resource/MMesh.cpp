#include "MMesh.h"
#include "MResource.h"
#include "MResourceManager.h"

#include "../MGame/MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MRenderSystem.h"

using namespace MG;

MMesh::MMesh(const wchar_t* file_path, MResourceManager* resourceManager)
	: MResource(file_path, resourceManager)
{
}

MMesh::MMesh(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes, MResourceManager* resourceManager)
	: MResource(L"", resourceManager)
{
	auto renderSystem = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();
	renderSystem->createVextexArrayObject(vbDes, ibDes);
}

MMesh::~MMesh()
{
}
