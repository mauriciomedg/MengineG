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

	//const glm::vec3 positionsList[] = {
	//	glm::vec3(-0.5f, -0.5f, -0.5f),
	//	glm::vec3(-0.5f, 0.5f, -0.5f),
	//	glm::vec3(0.5f, 0.5f, -0.5f),
	//	glm::vec3(0.5f, -0.5f, -0.5f),
	//
	//	glm::vec3(0.5f, -0.5f, 0.5f),
	//	glm::vec3(0.5f, 0.5f, 0.5f),
	//	glm::vec3(-0.5f, 0.5f, 0.5f),
	//	glm::vec3(-0.5f, -0.5f, 0.5f)
	//};
	//
	//glm::vec2 texCoordsList[]
	//{
	//	glm::vec2(0, 0),
	//	glm::vec2(0, 1),
	//	glm::vec2(1, 0),
	//	glm::vec2(1, 1)
	//};
	//
	//std::vector<VertexMesh> verticesList =
	//{
	//	//front
	//	{ positionsList[0], texCoordsList[1] },
	//	{ positionsList[1], texCoordsList[0] },
	//	{ positionsList[2], texCoordsList[2] },
	//	{ positionsList[3], texCoordsList[3] },
	//
	//	//back
	//	{ positionsList[4], texCoordsList[1] },
	//	{ positionsList[5], texCoordsList[0] },
	//	{ positionsList[6], texCoordsList[2] },
	//	{ positionsList[7], texCoordsList[3] },
	//
	//	//top
	//	{ positionsList[1], texCoordsList[1] },
	//	{ positionsList[6], texCoordsList[0] },
	//	{ positionsList[5], texCoordsList[2] },
	//	{ positionsList[2], texCoordsList[3] },
	//	
	//	// bottom
	//	{ positionsList[7], texCoordsList[1] },
	//	{ positionsList[0], texCoordsList[0] },
	//	{ positionsList[3], texCoordsList[2] },
	//	{ positionsList[4], texCoordsList[3] },
	//
	//	// right
	//	{ positionsList[3], texCoordsList[1] },
	//	{ positionsList[2], texCoordsList[0] },
	//	{ positionsList[5], texCoordsList[2] },
	//	{ positionsList[4], texCoordsList[3] },
	//
	//	// left
	//	{ positionsList[7], texCoordsList[1] },
	//	{ positionsList[6], texCoordsList[0] },
	//	{ positionsList[1], texCoordsList[2] },
	//	{ positionsList[0], texCoordsList[3] }
	//};
	//
	//ui32 indicesList[] = {
	//	0, 1, 2,
	//	2, 3, 0,
	//
	//	4, 5, 6,
	//	6, 7, 4,
	//
	//	8, 9, 10,
	//	10, 11, 8,
	//
	//	12, 13, 14,
	//	14, 15, 12,
	//
	//	16, 17, 18,
	//	18, 19, 16,
	//
	//	20, 21, 22,
	//	22, 23, 20
	//};
	
	//MVertexAtrribute attributeList[] =
	//{
	//	sizeof(glm::vec3) / sizeof(f32), // pos
	//	sizeof(glm::vec2) / sizeof(f32) // text coord
	//};
	//
	//m_vertexArrayObject = resourceManager->getMGame()->getGraphicEngine()->getRenderSystem()->createVextexArrayObject(
	//	{(void*)(&verticesList[0]),
	//	sizeof(VertexMesh),
	//	(ui32)verticesList.size(),
	//
	//	attributeList,
	//	sizeof(attributeList) / (sizeof(MVertexAtrribute)),
	//	},
	//
	//	{
	//		(void*)indicesList,
	//		sizeof(indicesList)
	//	}
	//);
	
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
