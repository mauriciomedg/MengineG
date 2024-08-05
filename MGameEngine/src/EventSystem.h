#pragma once

#include <map>
#include <string>
#include "Delegate.h"

namespace MG
{
	struct dataSphape
	{
		size_t a = 1;
		size_t b = 2;
	};

	class EventSystem
	{
	public:
		std::map<size_t, delegate<dataSphape>> m_events;
	};
}
