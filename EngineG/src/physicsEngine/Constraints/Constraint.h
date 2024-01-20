#pragma once

#include <vector>

namespace MG
{
	class Constraint
	{
	public:
		virtual void execute(float dt) = 0;
		virtual void prepare(std::vector<float>& J, std::vector<float>& C) = 0;
	};
}