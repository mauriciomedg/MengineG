#pragma once

#include "MathLibrary/TwVec3.h"
#include "MathLibrary/TwQuat.h"
#include "MathLibrary/TwMat3.h"
#include "MathLibrary/TwMat4.h"

namespace Twister
{
	class TwDynamicShape
	{
	public:
		
		float getMass() const { return mMass; };
		float getMassInv() const { return mMassInv; };
		const TwMat3& getIbody() const { return mIbody; };
		const TwMat3& getIbodyInv() const { return mIbodyInv; };

		static TwDynamicShape* createBox(TwVec3& halfSize, float mass);

		enum class Shape { Box = 0, Sphere = 1 };

	protected:
		float mMass;
		float mMassInv;
		TwMat3 mIbody;
		TwMat3 mIbodyInv;
	};

	class TwRigidBody
	{

	public:

		TwRigidBody(TwDynamicShape* shape);

		TwVec3 mX;
		TwQuat mQ;

		TwDynamicShape* mShape;

		//Derived quantites
		TwVec3 mV;
		TwVec3 mW; //w(t)
		TwMat3 mIinv;
		TwMat3 mI;
		
		bool mIsStatic = false;

		static TwRigidBody* Create(TwDynamicShape* shape, const TwMat4& transform, bool isStatic);
	};
}