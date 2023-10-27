#include "RigidBody.h"

RigidBody::RigidBody()
	: mMass(0.1f), mL(0.0f), mForce(0.0f)
{
}

void RigidBody::init(glm::vec3 pos, glm::vec3 v, glm::mat3 R)
{
	mX = pos; 
	mP = mMass * v;
	mR = R;

	float x0 = 2.0f;
	float y0 = 2.0f;
	float z0 = 2.0f;

	mIbody[0][0] = glm::pow(y0, 2) + glm::pow(z0, 2);
	mIbody[1][1] = glm::pow(x0, 2) + glm::pow(z0, 2);
	mIbody[2][2] = glm::pow(x0, 2) + glm::pow(y0, 2);

	mIbody *= mMass / 12;

	mIbodyInv = glm::inverse(mIbody);
}