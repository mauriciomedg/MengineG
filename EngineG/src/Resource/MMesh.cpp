#include "MMesh.h"
#include "MResource.h"
#include "MResourceManager.h"
#include "../Utils.h"

#include "../MGame/MGame.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MRenderSystem.h"

using namespace MG;

MMesh::MMesh(const char* file_path, MResourceManager* resourceManager)
	: MResource(file_path, resourceManager)
{
    ImportedModel myModel(file_path);

    std::vector<glm::vec3> vert = myModel.getVertices();
    std::vector<glm::vec2> tex = myModel.getTextureCoords();
    std::vector<glm::vec3> norm = myModel.getNormals();
    int numObjVertices = myModel.getNumVertices();

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors
    
    std::vector<VertexMesh> verticesList;
    for (int i = 0; i < numObjVertices; i++)
    {
        verticesList.push_back({vert[i], tex[i]});
    }

    MVertexAtrribute attributeList[] =
    {
        sizeof(glm::vec3) / sizeof(f32), // pos
        sizeof(glm::vec2) / sizeof(f32) // text coord
    };

    auto renderSystem = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();
    m_vertexArrayObject = renderSystem->createVextexArrayObject(
        {
            (void*)(&verticesList[0]),
            sizeof(VertexMesh),
            (ui32)verticesList.size(),

            attributeList,
            sizeof(attributeList) / (sizeof(MVertexAtrribute)),
        });
}

MMesh::MMesh(const MVertexBufferDesc& vbDes, const MIndexBufferDesc& ibDes, MResourceManager* resourceManager)
	: MResource("", resourceManager)
{
	auto renderSystem = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem();
	renderSystem->createVextexArrayObject(vbDes, ibDes);
}

MMesh::~MMesh()
{
}

MVAOSharedPtr& MG::MMesh::getVertexArrayObject()
{
    return m_vertexArrayObject;
}
