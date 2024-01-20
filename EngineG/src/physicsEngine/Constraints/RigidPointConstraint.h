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

		RigidPointConstraint(RigidBody* b, const glm::vec3& localPost);

		virtual void execute(float dt);

	private:

		RigidBody* mBody;
		glm::vec3 mLocalPost;
		std::vector<float> mMinv;
		std::vector<float> mJ;
	};
}