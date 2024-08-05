#pragma once
#include "../OGraphicsEngine/MPrerequisites.h"
#include <map>
#include <memory>

namespace MG
{
	class MEntitySystem;
	class MEntity
	{
	public:
		MEntity();
		virtual ~MEntity();

		virtual void create() {}
		virtual void update(f32 dt) {}

		void release();

		MEntitySystem* getEntitySystem();
		MTransformComponent* getTransform();

		template<typename T>
		T* createComponent()
		{
			static_assert(std::is_base_of<MComponent, T>::value, "T must be derived from MComponent");
			auto c = getComponent<T>();
			if (!c)
			{
				auto id = typeid(T).hash_code();
				auto c = new T();
				createComponentInternal(c, id);
				return c;
			}
			return nullptr;
		}

		size_t getId() const { return m_id; }

		template<typename T>
		T* getComponent()
		{
			static_assert(std::is_base_of<MComponent, T>::value, "T must be derived from MComponent");
			auto id = typeid(T).hash_code();
			return (T*)getComponentInternal(id);
		}
	private:
		void createComponentInternal(MComponent* entity, size_t id);
		MComponent* getComponentInternal(size_t id);
		void removeComponent(size_t id);
	protected:
		size_t m_id = 0;
		MEntitySystem* m_entitySystem = nullptr;

		MTransformComponent* m_transformComponent = nullptr;

		std::map<size_t, std::unique_ptr<MComponent>> m_components;
		friend class MEntitySystem;
		friend class MComponent;
	};
}