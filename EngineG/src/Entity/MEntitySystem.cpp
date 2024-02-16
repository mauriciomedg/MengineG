#include "MEntitySystem.h"
#include "MEntity.h"

using namespace MG;

MEntitySystem::MEntitySystem()
{
	
}

MEntitySystem::~MEntitySystem()
{

}

bool MEntitySystem::createEntityInternal(MEntity* entity, size_t id)
{
	auto ptr = std::unique_ptr<MEntity>(entity);
	m_entities[id].emplace(entity, std::move(ptr));
	
	entity->m_id = id;
	entity->m_entitySystem = this;
	entity->onCreate();

	return true;
}

void MG::MEntitySystem::removeEntity(MEntity* entity)
{
	m_entitiesToDestroy.emplace(entity);
}

void MEntitySystem::update(f32 dt)
{
	for (auto e : m_entitiesToDestroy)
	{
		m_entities[e->m_id].erase(e);
	}
	m_entitiesToDestroy.clear();
}
