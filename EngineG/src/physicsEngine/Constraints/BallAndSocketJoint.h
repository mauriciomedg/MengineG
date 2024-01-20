#pragma once
#include "Constraint.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct RigidBody;

namespace MG
{
	class BallAndSocketJoint : public Constraint
	{
	public:
		BallAndSocketJoint();
		BallAndSocketJoint(RigidBody* b1, RigidBody* b2);

		virtual void prepare(std::vector<float>& J, std::vector<float>& C) {};
		virtual void execute(float dt);

	private:
		void prepareData(RigidBody* b, glm::vec3& C, std::vector<float>& skew_r);

	private:

		RigidBody* mBody[2];
	};
}