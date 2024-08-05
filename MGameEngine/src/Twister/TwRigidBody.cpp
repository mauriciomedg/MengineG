#include "TwRigidBody.h"
#include <math.h>

namespace MG
{
	class TwDynamicBox : public TwDynamicShape
	{
	public:
		TwDynamicBox(TwVec3& halfSize, float mass);
	};

	TwDynamicShape* TwDynamicShape::createBox(TwVec3& halfSize, float mass)
	{
		return new TwDynamicBox(halfSize, mass);
	}

	TwDynamicBox::TwDynamicBox(TwVec3& halfSize, float mass)
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

	TwRigidBody::TwRigidBody(TwDynamicShape* shape)
		: mShape(shape)
	{
	}
}

