﻿#pragma once

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Contact.h"
#include "CollisionDetection.h"
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

int PhysicsWorld::instanciatePrimitive(const glm::mat4& transform, bool isSimulatingPhysics)
{
	return -1;
}

int PhysicsWorld::instanciatePrimitiveBox(const glm::mat4& transform, glm::vec3& halfSize, bool isSimulatingPhysics)
{
	CollisionBox* box = new CollisionBox;
	box->halfSize = halfSize;
	mPrimitive.push_back(box);
	mPrimitive.back()->body = (isSimulatingPhysics ? new RigidBody() : nullptr);
	if (isSimulatingPhysics) 
	{
		mPrimitive.back()->body->init(transform, halfSize);
		mPrimitive.back()->calculateInternals();
	}
	mPrimitive.back()->calculateInternals(transform);
	mPrimitive.back()->mSimulatePhysics = isSimulatingPhysics;
	return mPrimitive.size() - 1;
}

int PhysicsWorld::instanciatePrimitivePlane(const glm::mat4& transform, bool isSimulatingPhysics)
{
	CollisionPlane* plane = new CollisionPlane;
	plane->direction = transform[1];
	mPrimitive.push_back(plane);
	mPrimitive.back()->body = nullptr;
	mPrimitive.back()->calculateInternals(transform);
	mPrimitive.back()->mSimulatePhysics = isSimulatingPhysics;
	return mPrimitive.size() - 1;
}

void PhysicsWorld::generateContacts()
{
	mCollisionDetection->cData.reset(MAX_CONTACTS);
	for (int i = 0; i < mPrimitive.size(); ++i)
	{
		CollisionBox* box = dynamic_cast<CollisionBox*>(mPrimitive[i]);

		for (int j = i + 1; j < mPrimitive.size(); ++j)
		{
			if (box && !box->mIgnoreCollision)
			{
				CollisionBox* box2 = dynamic_cast<CollisionBox*>(mPrimitive[j]);

				if (box2 && !box2->mIgnoreCollision)
				{
					if (glm::length(box->getAxis(3) - box2->getAxis(3)) <= (glm::length(box->halfSize) + glm::length(box2->halfSize)))
					{
						mCollisionDetection->cData.friction = -1.0f;
						mCollisionDetection->cData.restitution = 0.0f;
						CollisionDetector::boxAndBox(*box, *box2, &mCollisionDetection->cData);
					}
				}

				CollisionPlane* plane = dynamic_cast<CollisionPlane*>(mPrimitive[j]);

				if (plane)
				{
					mCollisionDetection->cData.friction = 0.0f;
					mCollisionDetection->cData.restitution = 0.4f;
					CollisionDetector::boxAndHalfSpace(*box, *plane, &mCollisionDetection->cData);
				}
			}
		}
	}
}

void PhysicsWorld::addMovement(int id, glm::vec3& intensity, float scale)
{
	mPrimitive[id]->body->addMovement(intensity, scale);
}

void PhysicsWorld::move(int id, glm::vec3& velocity)
{
	if (mPrimitive[id]->body)
	{
		mPrimitive[id]->body->move(velocity * mDeltaT);
	}
	else
	{
		const glm::mat4& currentTransform = mPrimitive[id]->getTransform();
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(currentTransform[3]) + velocity * mDeltaT);
		mPrimitive[id]->calculateInternals(translation);
	}
}

void PhysicsWorld::setAffectedByGravity(int id, bool isAffetedByGravity)
{
	if (mPrimitive[id]->body)
	{
		mPrimitive[id]->body->mIsAffectedByGravity = isAffetedByGravity;
	}
}

void PhysicsWorld::setIgnoreCollision(int id, bool ignore)
{
	mPrimitive[id]->mIgnoreCollision = ignore;
}

void PhysicsWorld::runSimulation(float deltaT)
{
	int nbSuperSample = 3;

	deltaT = deltaT / nbSuperSample;
	mDeltaT = deltaT;

	for (int sample = 0; sample < nbSuperSample; ++sample)
	{
		int indexBody = 0;
		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->prepareSystem(&mY[0 + RigidBody::STATE_SIZE * indexBody],
					&mYdot[0 + RigidBody::STATE_SIZE * indexBody],
					deltaT,
					mGravity);
				++indexBody;
			}
		}

		if (mYdot.size() > 0)
		{
			ode(&mY[0], &mYdot[0], deltaT, mYdot.size());
		}
		
		indexBody = 0;
		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->update(&mY[0 + RigidBody::STATE_SIZE * indexBody]);
				mPrimitive[i]->calculateInternals();
				++indexBody;
			}
		}

		generateContacts();
		mCollisionResponse->update(&mCollisionDetection->cData, mPrimitive);
	}
}

void PhysicsWorld::init()
{
	mGravity = glm::vec3(0.0f, -98.0f, 0.0f);

	int nbSimulatedPhysicsPrimitives = 0;
	for (int i = 0; i < mPrimitive.size(); i++)
	{
		RigidBody* bodies = mPrimitive[i]->body;
		
		if (mPrimitive[i]->mSimulatePhysics)
		{
			++nbSimulatedPhysicsPrimitives;
		}
	}

	mY.resize(RigidBody::STATE_SIZE * nbSimulatedPhysicsPrimitives);
	mYdot.resize(RigidBody::STATE_SIZE * nbSimulatedPhysicsPrimitives);

	mContacts = new Contact[MAX_CONTACTS];
	mCollisionDetection = new CollisionDetection;
	mCollisionDetection->cData.contactArray = mContacts;
	mCollisionDetection->cData.contactsLeft = MAX_CONTACTS;
	mCollisionResponse = new CollisionResponse;
}

const glm::mat4& PhysicsWorld::getPrimitiveLocation(int id) const
{
	return mPrimitive[id]->getTransform();
}
