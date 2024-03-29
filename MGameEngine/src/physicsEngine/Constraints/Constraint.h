#pragma once

#include <vector>

namespace MG
{
	class Constraint
	{
	public:
		virtual void execute(float dt) = 0;
		virtual void update2(float dt) = 0;
	};
}