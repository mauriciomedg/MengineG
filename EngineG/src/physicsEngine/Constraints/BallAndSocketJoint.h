#pragma once
#include "Constraint.h"

struct RigidBody;

namespace MG
{
	class BallAndSocketJoint : public Constraint
	{
	public:
		BallAndSocketJoint();
		BallAndSocketJoint(RigidBody* b1, RigidBody* b2);

		virtual void execute(float dt);

	private:
		RigidBody* mBody[2];
	};
}