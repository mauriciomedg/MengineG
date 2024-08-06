#pragma once

#include <map>
#include <string>
#include "Delegate.h"
#include "MathLibrary/TwVec3.h"

namespace MG
{
	struct dataSphape
	{
		TwVec3 extend;
	};

	class EventSystem
	{
	public:
		std::map<size_t, delegate<dataSphape>> m_events;
	};
}
