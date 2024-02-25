#include "MEntity.h"
#include "MEntitySystem.h"
#include "../Components/MComponent.h"
#include "../Components/MTransformComponent.h"

using namespace MG;

MEntity::MEntity()
{
	m_transformComponent = createComponent<MTransformComponent>();
}

MEntity::~MEntity()
{
}

void MEntity::release()
{
	m_entitySystem->removeEntity(this);
}

MEntitySystem* MG::MEntity::getEntitySystem()
{
	return m_entitySystem;
}

MTransformComponent* MG::MEntity::getTransform()
{
	return m_transformComponent;
}

void MEntity::createComponentInternal(MComponent* component, size_t id)
{
	auto ptr = std::unique_ptr<MComponent>(component);
	m_components.emplace(id, std::move(ptr));

	component->m_id = id;
	component->m_entity = this;

	component->onCreateInternal();
}

MComponent* MEntity::getComponentInternal(size_t id)
{
	auto it = m_components.find(id);
	if (it == m_components.end()) return nullptr;
	return it->second.get();
}

void MEntity::removeComponent(size_t id)
{
	m_components.erase(id);
}
