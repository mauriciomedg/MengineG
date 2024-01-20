#pragma once
#include "Constraint.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct RigidBody;

namespace MG
{
	class RigidPointRigidPointConstraint : public Constraint
	{
	public:
		RigidPointRigidPointConstraint(RigidBody* b1, const glm::vec3& localPost1, RigidBody* b2, const glm::vec3& localPost2);

		virtual void execute(float dt);

	private:

		RigidBody* mBody[2];
		glm::vec3 mLocalPost[2];
	};
}