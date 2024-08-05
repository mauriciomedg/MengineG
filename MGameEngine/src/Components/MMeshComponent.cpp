#include "MMeshComponent.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"

using namespace MG;

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
		eventItt->second(dataSphape({ m_entity->getId(), m_entity->getId() }));
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
