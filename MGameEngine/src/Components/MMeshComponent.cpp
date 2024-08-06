#include "MMeshComponent.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"
#include "../OGraphicsEngine/RenderSystem/MVertexArrayObject.h"
#include "../MathLibrary/TwVec3.h"

#include <GL/glew.h>

using namespace MG;

namespace 
{
	struct BoundingBox {
		TwVec3 min;
		TwVec3 max;
	};

	struct BoxShape
	{
		static void calculateExtend(const MMeshSharedPtr& mesh, TwVec3& extend)
		{
			glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexArrayObject()->getVertexBufferObject());

			TwVec3* vertices = (TwVec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
			if (!vertices) {
				throw std::runtime_error("Failed to map VBO");
			}

			TwVec3 min = vertices[0];
			TwVec3 max = vertices[0];

			for (size_t i = 1; i < mesh->getVertexArrayObject()->getVertexBufferSize() / 3; ++i)
			{
				if (vertices[i].x < min.x) min.x = vertices[i].x;
				if (vertices[i].y < min.y) min.y = vertices[i].y;
				if (vertices[i].z < min.z) min.z = vertices[i].z;

				if (vertices[i].x > max.x) max.x = vertices[i].x;
				if (vertices[i].y > max.y) max.y = vertices[i].y;
				if (vertices[i].z > max.z) max.z = vertices[i].z;
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			extend.x = max.x - min.x;
			extend.y = max.y - min.y;
			extend.z = max.z - min.z;
		}
	};
}

MMeshComponent::MMeshComponent()
{

}

MMeshComponent::~MMeshComponent()
{
	m_entity->getEntitySystem()->getGame()->getGraphicEngine()->removeComponent(this);
}

void MMeshComponent::setMesh(const MMeshSharedPtr& mesh)
{
	m_mesh = mesh;

	auto& events = m_entity->getEntitySystem()->getGame()->getEventSystem().m_events;

	auto eventItt = events.find(m_entity->getId());
	if (eventItt != events.end())
	{
		TwVec3 extend;
		BoxShape::calculateExtend(mesh, extend);

		eventItt->second(dataSphape({ extend }));
	}
}

const MMeshSharedPtr& MMeshComponent::getMesh()
{
	return m_mesh;
}

void MMeshComponent::addMaterial(const MMaterialSharedPtr& material)
{
	m_materials.push_back(material);
}

void MMeshComponent::removeMaterial(ui32 index)
{
	if (index >= m_materials.size()) return;
	m_materials.erase(m_materials.begin() + index);
}

const std::vector<MMaterialSharedPtr>& MMeshComponent::getMaterials()
{
	return m_materials;
}

void MMeshComponent::onCreateInternal()
{
	m_entity->getEntitySystem()->getGame()->getGraphicEngine()->addComponent(this);
}
