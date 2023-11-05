#include "CollisionResponse.h"
#include <glm/gtc/type_ptr.hpp>
#include "RigidBody.h"
#include <iostream>

namespace
{
	glm::vec4 vertex[] = { glm::vec4(1.0f),
			glm::vec4(-1.0f, -1.0f, -1.0f, 1.0),
			glm::vec4(-1.0f, 1.0f, -1.0f, 1.0),
			glm::vec4(1.0f, -1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, -1.0f, -1.0f, 1.0),
			glm::vec4(-1.0f, -1.0f, 1.0f, 1.0),
			glm::vec4(-1.0f, 1.0f, 1.0f, 1.0),
			glm::vec4(1.0f, 1.0f, -1.0f, 1.0)
	};

	bool hasContact(RigidBody* b, glm::vec4 pLocal, float dt, Contact* contact)
	{
		float ks = 50.0f;
		float kd = 10.1f;
		glm::vec3 P(0.0f, 20.0f, 0.0f);
		glm::vec3 N(0.0f, 1.0f, 0.0f);

		glm::vec3 pWorld = (b->mWorldMat * pLocal);

		glm::vec3 v = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));

		float depth = glm::dot(pWorld + v * dt - P, N);

		if (depth < 0.0f)// && glm::dot(v, N) < 0)
		{
			contact->body[0] = b;
			contact->contactNormal = N;
			contact->contactPoint = pWorld;
			contact->contactDepth = depth;

			return true;
		}

		return false;
	}

	void resolveContact(Contact* contact)
	{
		RigidBody* b = contact->body[0];
		glm::vec3& pWorld = contact->contactPoint;
		glm::vec3& N = contact->contactNormal;

		glm::vec3 v = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));

		float epsilon = 0.3f; // coefficent of restitution 0 <= epsilon <= 1
		float numerator = -(1 + epsilon) * glm::dot(v, N);

		pWorld += -(b->mX);

		float term1 = 1 / b->mMass;
		float term3 = glm::dot(N, glm::cross(b->mIbodyInv * glm::cross(pWorld, N), pWorld));

		float j = numerator / (term1 + term3);
		glm::vec3 force = j * N;

		b->mP += force;
		b->mL += glm::cross(pWorld, force);

		b->mV = b->mP / b->mMass;
		b->mW = b->mIbodyInv * b->mL;
	}
}

CollisionResponse::CollisionResponse()
{
	mContacts = new Contact[MaxContacts];
	mContactItt = mContacts;
}

void CollisionResponse::generateContacts(std::vector<RigidBody*>& bodies, float dt)
{
	mContactItt = mContacts;

	for (int i = 0; i < bodies.size(); i++)
	{
		RigidBody* b = bodies[i];
		for (int i = 0; i < 8; ++i)
		{
			Contact* contact = mContactItt;
			if (hasContact(b, vertex[i], dt, contact))
			{
				contact->localContactId = i;
				mContactItt++;
				contact->nextContact = mContactItt;
			}
		}
	}
}

void CollisionResponse::resolverContacts(std::vector<RigidBody*>& bodies, float* y, float* ydot, float dt, dydt_func solver)
{	
	for (int k = 0; k < 5; ++k)
	{
		int distance = std::distance(mContacts, mContactItt);

		if (distance > 0)
		{
			float max = 0.0f;
			int index = 0;

			for (int i = 0; i < distance; i++)
			{
				Contact* c = &(mContacts[i]);
				resolveContact(c);

				//RigidBody* b = c->body[0];

				//b->stateToArray(&y[0 + RigidBody::STATE_SIZE * b->mIndex]);
				//b->ddtStateToArray(&ydot[0 + RigidBody::STATE_SIZE * b->mIndex]);
				
				//b->prepareSystem(&y[0 + RigidBody::STATE_SIZE * b->mIndex],
				//	&ydot[0 + RigidBody::STATE_SIZE * b->mIndex],
				//	dt,
				//	glm::vec3(0.0f, -50.0f, 0.0f));



				//solver(&y[0 + RigidBody::STATE_SIZE * b->mIndex],
				//	&ydot[0 + RigidBody::STATE_SIZE * b->mIndex],
				//	dt,
				//	RigidBody::STATE_SIZE);

				//b->update(&y[0 + RigidBody::STATE_SIZE * b->mIndex]);
			}

			generateContacts(bodies, dt);
			
		}
	}
}