#pragma once

#include <map>
#include <memory>
#include <set>
#include "../OGraphicsEngine/MPrerequisites.h"

namespace MG
{
	class MEntity;
	class MEntitySystem
	{
	public:
		MEntitySystem(MGame* game);
		~MEntitySystem();

		template<typename T>
		T* createEntity()
		{
			static_assert(std::is_base_of<MEntity, T>::value, "T must be derived from MEntity");
			auto id = typeid(T).hash_code();
			auto e = new T();

			if (createEntityInternal(e, id))
				return e;
			return nullptr;
		}

		MGame* getGame();
	private:
		bool createEntityInternal(MEntity* entity, size_t id);
		void removeEntity(MEntity* entity);
		void update(f32 dt);

	private:
		std::map<size_t, std::map<MEntity*, std::unique_ptr<MEntity>>> m_entities;
		std::set<MEntity*> m_entitiesToDestroy;
		MGame* m_game = nullptr;

		friend class MEntity;
		friend class MGame;
	};
}