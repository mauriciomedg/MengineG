#pragma once

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include <iostream>

namespace
{
	void ScaleVector(float* dst, float DeltaT, int size)
	{
		for (int i = 0; i < size; ++i)
		{
			dst[i] *= DeltaT;
		}
	}

	void AddVectors(float* src, float* dst, int size)
	{
		for (int i = 0; i < size; ++i) {
			dst[i] += src[i];
		}
	}

	void ode(float* y, float* ydot, float deltaT, int size)
	{
		ScaleVector(ydot, deltaT, size);
		AddVectors(ydot, y, size); /* add -> temp2 */
	}
}

void PhysicsWorld::runSimulation(float deltaT)
{
	int nbSuperSample = 2;

	deltaT = deltaT / nbSuperSample;

	for (int sample = 0; sample < nbSuperSample; ++sample)
	{
		for (int i = 0; i < mBodies.size(); i++)
		{
			mBodies[i]->prepareSystem(&mY[0], &mYdot[0], deltaT, mGravity);
		}

		ode(&mY[0], &mYdot[0], deltaT, mYdot.size());

		for (int i = 0; i < mBodies.size(); i++)
		{
			mBodies[i]->update(&mY[0]);
		}
	}
}

void PhysicsWorld::init()
{
	mGravity = glm::vec3(0.0f, -50.0f, 0.0f);

	mY.resize(RigidBody::STATE_SIZE * mBodies.size());
	mYdot.resize(RigidBody::STATE_SIZE * mBodies.size());
}
