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

	bool contactPointDetermination(Contact* contact)
	{
		RigidBody* b = contact->body[0];
		glm::vec3& pWorld = contact->contactPoint;
		glm::vec3& N = contact->contactNormal;

		glm::vec3 v = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));

		float threshold = 1.0f;
		float vrel = glm::dot(v, N);

		if (vrel > threshold) // moving away
		{
			contact->type = Contact::MOVING_AWAY;
			return false;
		}
		if (vrel > -threshold) // resting contact
		{
			contact->type = Contact::RESTING;
			return false;
		}
		else
		{
			contact->type = Contact::COLLIDING;
			return true;
		}
	}

	bool collisionDetection(RigidBody* body, glm::vec4& pLocal, Contact* contact)
	{
		glm::vec3 P(0.0f, 20.0f, 0.0f);
		glm::vec3 N(0.0f, 1.0f, 0.0f);
		glm::vec3 pWorld = (body->mWorldMat * pLocal);

		float depth = glm::dot(pWorld - P, N);

		contact->contactNormal = N;
		contact->contactDepth = glm::abs(depth);
		contact->isStillInContact = depth < 0.0f;
		contact->contactPoint = pWorld;
		return contact->isStillInContact;
	}

	bool hasContact(RigidBody* b, glm::vec4& pLocal, float dt, Contact* contact)
	{
		if (collisionDetection(b, pLocal, contact))
		{
			glm::vec3 pWorld = (b->mWorldMat * pLocal);
			contact->body[0] = b;
			contact->contactPoint = pWorld;
			contact->relContactPoint = pWorld - b->mX;
			contact->contactVelocity = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));

			contactPointDetermination(contact);
			return true;
		}
		else
		{
			return false;
		}
	}

	void resolveContact(Contact* contact)
	{
		RigidBody* b = contact->body[0];
		glm::vec3& pWorld = contact->contactPoint;
		glm::vec3& N = contact->contactNormal;

		glm::vec3 v = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));

		float epsilon = 0.2f; // coefficent of restitution 0 <= epsilon <= 1
		float numerator = -(1 + epsilon) * glm::dot(v, N);

		pWorld += -(b->mX);

		float term1 = 1 / b->mMass;
		float term3 = glm::dot(N, glm::cross(b->mIinv * glm::cross(pWorld, N), pWorld));

		float j = numerator / (term1 + term3);
		glm::vec3 force = j * N;

		b->mP += force;
		b->mL += glm::cross(pWorld, force);

		b->mV = b->mP / b->mMass;
		b->mW = b->mIinv * b->mL;

		contact->contactVelocity = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));
	}
}

Contact::Contact()
{
	body[0] = nullptr;
	body[1] = nullptr;
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

void CollisionResponse::resolveInterpenetration(RigidBody* bodies, glm::vec4 pLocal, float* y, float* ydot, float dt0, float dt1, float dtm, dydt_func solver)
{
	glm::vec3 P(0.0f, 20.0f, 0.0f);
	glm::vec3 N(0.0f, 1.0f, 0.0f);

	float epsilon = 0.05f;
	glm::vec3 pWorld = (bodies->mWorldMat * pLocal);

	float depth = glm::dot(pWorld - P, N);
	int iter = 0;
	int MAX_ITER = 5;
	while (!(depth >= -epsilon && depth <= epsilon) && iter < MAX_ITER)
	{
		solver(&y[0 + RigidBody::STATE_SIZE * bodies->mIndex],
			&ydot[0 + RigidBody::STATE_SIZE * bodies->mIndex],
			-dtm,
			RigidBody::STATE_SIZE);
		bodies->update(&y[0 + RigidBody::STATE_SIZE * bodies->mIndex]);

		dtm = (dt0 + dt1) / 2.0f;

		solver(&y[0 + RigidBody::STATE_SIZE * bodies->mIndex],
			&ydot[0 + RigidBody::STATE_SIZE * bodies->mIndex],
			dtm,
			RigidBody::STATE_SIZE);

		bodies->update(&y[0 + RigidBody::STATE_SIZE * bodies->mIndex]);

		pWorld = (bodies->mWorldMat * pLocal);
		depth = glm::dot(pWorld - P, N);

		if (depth < -epsilon)
		{
			dt1 = dtm;
		}
		else if (depth > epsilon)
		{
			dt0 = dtm;
		}

		iter++;
	}
}

void CollisionResponse::update(std::vector<RigidBody*>& bodies, float* y, float* ydot, float dt, dydt_func solver)
{
	//collision detection
	generateContacts(bodies, dt);

	int distance = std::distance(mContacts, mContactItt);

	if (distance > 0)
	{
		for (int i = 0; i < distance; i++)
		{
			Contact* c = &(mContacts[i]);

			if (c->type == Contact::COLLIDING)
			{
				resolveInterpenetration(c->body[0], vertex[c->localContactId], y, ydot, 0, dt, dt, solver);

				//update contacts due to the resolving interpenetration
				for (int j = 0; j < distance; j++)
				{
					Contact* cc = &(mContacts[j]);
					collisionDetection(cc->body[0], vertex[cc->localContactId], cc);
				}
			}
		}
		
		for (int i = 0; i < distance; i++)
		{
			Contact* c = &(mContacts[i]);
			if (c->isStillInContact && c->type == Contact::COLLIDING)
			{
				resolveContact(c);
			}
		}
	}
}
