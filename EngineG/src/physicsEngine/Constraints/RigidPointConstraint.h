#pragma once
#include "Constraint.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct RigidBody;

namespace MG
{
	class RigidPointConstraint : public Constraint
	{
	public:
		RigidPointConstraint();
		RigidPointConstraint(RigidBody* b);

		virtual void prepare(std::vector<float>& J, std::vector<float>& C);
		virtual void execute(float dt);

	private:
		void prepareData(RigidBody* b, glm::vec3& C, std::vector<float>& skew_r);

	private:

		RigidBody* mBody;
		std::vector<float> mMinv;
		std::vector<float> mJ;
	};
}