#pragma once
#include "../OGraphicsEngine/MPrerequisites.h"

namespace MG
{
	class MComponent
	{
	public:
		MComponent();
		virtual ~MComponent();
		void release();
	protected:
		size_t m_id = 0;
		MEntity* m_entity = nullptr;

		friend class MEntity;
	};
}