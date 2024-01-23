#pragma once
#include "Constraint.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct RigidBody;
class Contact;

namespace MG
{
	class ContactConstraint : public Constraint
	{
	public:
		ContactConstraint(Contact* c);

		virtual void execute(float dt);

	private:

		RigidBody* mBody[2];
		glm::vec3 mLocalContactPoint[2];
		//glm::vec3 mDr[2];
		glm::vec3 mN;
		float mDepth;
	};
}