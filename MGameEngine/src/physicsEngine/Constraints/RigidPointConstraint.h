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

		RigidPointConstraint(RigidBody* b, const glm::vec3& localPost, const glm::vec3& position, float distanceOffset);

		virtual void execute(float dt);
		virtual void update2(float dt) {};
		virtual void debugDraw() {};
	private:

		RigidBody* mBody;
		glm::vec3 mLocalPost;
		glm::vec3 mPosition;
		float mDistanceOffset;
		std::vector<float> mMinv;
		std::vector<float> mJ;
	};
}