#include "MEntitySystem.h"
#include "MEntity.h"
#include "../MGame/MGame.h"

using namespace MG;

MEntitySystem::MEntitySystem(MGame* game)
	: m_game(game)
{
	
}

MEntitySystem::~MEntitySystem()
{

}

MGame* MEntitySystem::getGame()
{
	return m_game;
}

bool MEntitySystem::createEntityInternal(MEntity* entity, size_t id)
{
	auto ptr = std::unique_ptr<MEntity>(entity);
	m_entities[id].emplace(entity, std::move(ptr));
	
	entity->m_id = id;
	entity->m_entitySystem = this;
	entity->create();

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

	//for (auto&& [id, entities] : m_entities) // std:c++17
	//{
	//	for (auto&& [ptr, entity] : entities)
	//	{
	//		//entity->
	//	}
	//}

	for (auto& v : m_entities) // std:c++17
	{
		for (auto& entity : v.second)
		{
			entity.second->update(dt);
		}
	}
}
