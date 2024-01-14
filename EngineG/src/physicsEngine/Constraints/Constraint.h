#pragma once

namespace MG
{
	class Constraint
	{
	public:
		virtual void execute(float dt) = 0;
	};
}