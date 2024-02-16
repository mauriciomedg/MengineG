#pragma once

#include <map>
#include <memory>
#include "../OGraphicsEngine/MPrerequisites.h"

namespace MG
{
	class MEntity;
	class MEntitySystem
	{
	public:
		MEntitySystem();
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

	private:
		bool createEntityInternal(MEntity* entity, size_t id);

	private:
		std::map<size_t, std::map<MEntity*, std::unique_ptr<MEntity>>> m_entities;
	};
}