#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
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
		T* createEntity(const std::string& name)
		{
			static_assert(std::is_base_of<MEntity, T>::value, "T must be derived from MEntity");
			//auto id = typeid(T).hash_code();
			size_t id = mEntityCounter++;
			auto e = new T();

			if (createEntityInternal(e, id, name))
				return e;
			return nullptr;
		}

		MGame* getGame();

		void removeEntity(const std::string& name);
	private:
		bool createEntityInternal(MEntity* entity, size_t id, const std::string& name);
		void removeEntity(MEntity* entity);
		void update(f32 dt);

	private:
		std::map<size_t, std::map<MEntity*, std::unique_ptr<MEntity>>> m_entities;
		std::set<MEntity*> m_entitiesToDestroy;
		std::map<std::string, size_t> m_names_to_id;
		MGame* m_game = nullptr;

		size_t mEntityCounter = 0;

		friend class MEntity;
		friend class MGame;
	};
}