#pragma once

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "CollisionResponse.h"

#include <iostream>

namespace
{
	//void ScaleVector(float* dst, float DeltaT, int size)
	//{
	//	for (int i = 0; i < size; ++i)
	//	{
	//		dst[i] *= DeltaT;
	//	}
	//}

	void AddVectors(float* src, float* dst, float DeltaT, int size)
	{
		for (int i = 0; i < size; ++i) 
		{
			//float x = dst[i];
			//float v = src[i];
			//float d = v * DeltaT;
			//float nextp = x + d;
			dst[i] += src[i] * DeltaT;// nextp;
		}
	}

	void ode(float* y, float* ydot, float deltaT, int size)
	{
		//ScaleVector(ydot, deltaT, size);
		AddVectors(ydot, y, deltaT, size); /* add -> temp2 */
	}
}

void PhysicsWorld::runSimulation(float deltaT)
{
	int nbSuperSample = 3;

	deltaT = deltaT / nbSuperSample;

	for (int sample = 0; sample < nbSuperSample; ++sample)
	{
		for (int i = 0; i < mBodies.size(); i++)
		{
			mBodies[i]->prepareSystem(&mY[0 + RigidBody::STATE_SIZE * i],
				&mYdot[0 + RigidBody::STATE_SIZE * i],
				deltaT,
				mGravity);
		}

		if (mYdot.size() > 0)
		{
			ode(&mY[0], &mYdot[0], deltaT, mYdot.size());
		}
		
		for (int i = 0; i < mBodies.size(); i++)
		{
			mBodies[i]->update(&mY[0 + RigidBody::STATE_SIZE * i], deltaT);
		}

		mCollisionResponse->update(mBodies, &mY[0], &mYdot[0], deltaT, ode);
	}
}

void PhysicsWorld::init()
{
	mGravity = glm::vec3(0.0f, -980.0f, 0.0f);

	mY.resize(RigidBody::STATE_SIZE * mBodies.size());
	mYdot.resize(RigidBody::STATE_SIZE * mBodies.size());

	mCollisionResponse = new CollisionResponse;
}
