#pragma once
#include "../OGraphicsEngine/MPrerequisites.h"

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

	protected:
		size_t m_id = 0;
		MEntitySystem* m_entitySystem = nullptr;

		friend class MEntitySystem;
	};
}