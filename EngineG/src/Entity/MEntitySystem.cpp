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

	entity->onCreate();

	return true;
}
