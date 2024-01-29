#pragma once

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Contact.h"
#include "CollisionDetection.h"
#include "CollisionResponse.h"
#include "MathUtils.h"
#include "Constraints/RigidPointRigidPointConstraint.h"
#include "Constraints/RigidPointConstraint.h"
#include "Constraints/ContactConstraint.h"
#include <iostream>
#include <algorithm>

using namespace MG;

int PhysicsWorld::instanciatePrimitive(const glm::mat4& transform, bool isSimulatingPhysics)
{
	return -1;
}

int PhysicsWorld::instanciatePrimitiveBox(const glm::mat4& transform, glm::vec3& halfSize, float mass, bool isSimulatingPhysics)
{
	CollisionBox* box = new CollisionBox;
	box->halfSize = halfSize;
	mPrimitive.push_back(box);
	mPrimitive.back()->body = (isSimulatingPhysics ? new RigidBody(DynamicShape::createBlock(halfSize, mass)) : nullptr);
	if (isSimulatingPhysics) 
	{
		mPrimitive.back()->body->init(transform);
		mPrimitive.back()->calculateInternals();
	}
	mPrimitive.back()->calculateInternals(transform);
	mPrimitive.back()->mSimulatePhysics = isSimulatingPhysics;
	return mPrimitive.size() - 1;
}

int PhysicsWorld::instanciatePrimitivePlane(const glm::mat4& transform, float mass, bool isSimulatingPhysics)
{
	CollisionPlane* plane = new CollisionPlane;
	plane->direction = transform[1];
	mPrimitive.push_back(plane);

	glm::vec3 halfSize(0.0f);
	mPrimitive.back()->body = new RigidBody(DynamicShape::createBlock(halfSize, 0.0f));
	mPrimitive.back()->body->init(transform);
	mPrimitive.back()->body->mIsSimulatingPhysics = false;
	
	mPrimitive.back()->calculateInternals(transform);
	mPrimitive.back()->mSimulatePhysics = isSimulatingPhysics;
	return mPrimitive.size() - 1;
}

bool PhysicsWorld::generateContacts(std::vector<MG::ContactConstraint>& contactConstraintList)
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
					mCollisionDetection->cData.restitution = 0.0f;
					CollisionDetector::boxAndHalfSpace(*box, *plane, &mCollisionDetection->cData);
				}
			}
		}
	}

	//
	CollisionData* cData = &mCollisionDetection->cData;
	for (int j = 0; j < cData->contactCount; j++)
	{
		contactConstraintList.push_back(MG::ContactConstraint(&(cData->contactArray[j])));
	}
	//

	return false;
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


void PhysicsWorld::simulating(float deltaT)
{
	int numSubsteps = 2;

	deltaT = deltaT / numSubsteps;
	mDeltaT = deltaT;

	std::vector<MG::ContactConstraint> contactConstraintList;
	generateContacts(contactConstraintList);

	for (int sample = 0; sample < numSubsteps; ++sample)
	{
		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->computeForceAndTorque(deltaT, mGravity);
				bodies->update1(deltaT);
			}
		}

		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->computeForceAndTorque(deltaT, mGravity);
				bodies->update2(deltaT);
				mPrimitive[i]->calculateInternals();
			}
		}

		int numPosIters = 5;
		for (int posItt = 0; posItt < numPosIters; ++posItt)
		{
			
			for (MG::Constraint* c : mConstraints)
			{
				c->execute(mDeltaT);
			}

			for (MG::ContactConstraint& c : contactConstraintList)
			{
				c.execute(mDeltaT);
			}

			for (int i = 0; i < mPrimitive.size(); i++)
			{
				if (mPrimitive[i]->mSimulatePhysics)
				{
					RigidBody* bodies = mPrimitive[i]->body;
					bodies->calculateInternalData();
					mPrimitive[i]->calculateInternals();
				}
			}
		}

		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->postUpdate(deltaT);
				mPrimitive[i]->calculateInternals();
			}
		}

		for (MG::ContactConstraint& c : contactConstraintList)
		{
			c.update2(mDeltaT);
		}

		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->calculateInternalData();
				mPrimitive[i]->calculateInternals();
			}
		}
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

void PhysicsWorld::addRigidPointRigidPointConstraint(int id1, const glm::vec3& localPost1, int id2, const glm::vec3& localPost2, float distanceOffset)
{
	mConstraints.push_back(new RigidPointRigidPointConstraint(id1 >= 0 ? mPrimitive[id1]->body : nullptr, localPost1, id2 >= 0 ? mPrimitive[id2]->body : nullptr, localPost2, distanceOffset));
}

void PhysicsWorld::addRigidPointConstraint(int id, const glm::vec3& localPost, const glm::vec3& position, float distanceOffset)
{
	mConstraints.push_back(new RigidPointConstraint(id >= 0 ? mPrimitive[id]->body : nullptr, localPost, position, distanceOffset));
}
