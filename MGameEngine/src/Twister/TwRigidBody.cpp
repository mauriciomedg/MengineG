#include "TwRigidBody.h"
#include <math.h>

using namespace MG;

TwDynamicBox* TwDynamicBox::create(const TwVec3& halfSize, float mass)
{
	return new TwDynamicBox(halfSize, mass);
}

TwDynamicBox::TwDynamicBox(const TwVec3& halfSize, float mass)
{
	mMass = mass;

	mMassInv = 1 / mass;

	float x0 = 2.0f * halfSize.x;
	float y0 = 2.0f * halfSize.y;
	float z0 = 2.0f * halfSize.z;

	mIbody.setDiagonal(std::pow(y0, 2) + std::pow(z0, 2),
		std::pow(x0, 2) + std::pow(z0, 2),
		std::pow(x0, 2) + std::pow(y0, 2));

	mIbody *= mMass / 12;

	mIbodyInv = mIbody.inverse();
}

TwRigidBody::TwRigidBody(TwDynamicShape* shape, const TwMat4& transform, bool isStatic)
	: mShape(shape)
{
}

TwRigidBody* TwRigidBody::create(TwDynamicShape* shape, const TwMat4& transform, bool isStatic)
{
	return new TwRigidBody(shape, transform, isStatic);
}


