#include "CollisionResponse.h"
#include <glm/gtc/type_ptr.hpp>
#include "RigidBody.h"
#include "Contact.h"
#include "CollisionDetection.h"
#include <iostream>

namespace
{
	//glm::vec4 vertex[] = { glm::vec4(1.0f),
	//		glm::vec4(-1.0f, -1.0f, -1.0f, 1.0),
	//		glm::vec4(-1.0f, 1.0f, -1.0f, 1.0),
	//		glm::vec4(1.0f, -1.0f, 1.0f, 1.0),
	//		glm::vec4(1.0f, -1.0f, -1.0f, 1.0),
	//		glm::vec4(-1.0f, -1.0f, 1.0f, 1.0),
	//		glm::vec4(-1.0f, 1.0f, 1.0f, 1.0),
	//		glm::vec4(1.0f, 1.0f, -1.0f, 1.0)
	//};
	//
	//bool contactPointDetermination(Contact* contact)
	//{
	//	RigidBody* b = contact->body[0];
	//	glm::vec3& N = contact->contactNormal;
	//	
	//	glm::vec3 v = glm::cross(b->mW, glm::vec3(contact->contactPoint - b->mX));
	//	//v += b->mV;
	//
	//	//glm::vec3 v = glm::cross(b->mPreviousW, glm::vec3(contact->contactPoint - b->mPreviousX));
	//	//v += b->mPreviousV;
	//
	//	float threshold = 0.5f;
	//	float vrel = glm::dot(v, N);
	//
	//	if (vrel > threshold) // moving away
	//	{
	//		contact->type = Contact::MOVING_AWAY;
	//		return false;
	//	}
	//	if (vrel > -threshold) // resting contact
	//	{
	//		contact->type = Contact::RESTING;
	//		return false;
	//	}
	//	else
	//	{
	//		contact->type = Contact::COLLIDING;
	//		return true;
	//	}
	//}

	//bool collisionDetection(RigidBody* body, glm::vec4& pLocal, Contact* contact)
	//{
	//	glm::vec3 P(0.0f, 20.0f, 0.0f);
	//	glm::vec3 N(0.0f, 1.0f, 0.0f);
	//	glm::vec3 pWorld = (body->mWorldMat * pLocal);
	//
	//	float depth = glm::dot(pWorld - P, N);
	//
	//	contact->contactNormal = N;
	//	contact->contactDepth = glm::abs(depth);
	//	contact->isStillInContact = depth < 0.0f;
	//	contact->contactPoint = pWorld;
	//	return contact->isStillInContact;
	//}

	//bool hasContact(RigidBody* b, glm::vec4& pLocal, Contact* contact)
	//{
	//	if (collisionDetection(b, pLocal, contact))
	//	{
	//		glm::vec3 pWorld = (b->mWorldMat * pLocal);
	//		contact->body[0] = b;
	//		contact->contactPoint = pWorld;
	//		contact->relContactPoint = pWorld - b->mX;
	//
	//		contactPointDetermination(contact);
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}

	float desiredVelocity(Contact* contact, float epsilon)
	{
		RigidBody* b = contact->body[0];
		glm::vec3& N = contact->contactNormal;
		glm::vec3 v = b->mV + glm::cross(b->mW, contact->contactPoint - b->mX);
		float numerator = -(1 + epsilon) * glm::dot(v, N);
		return numerator;
	}

	glm::vec3 applyImpulse(RigidBody* b, const glm::vec3& v, const glm::vec3& contactPoint, const glm::vec3& N, float epsilon)
	{
		glm::vec3 localPoint = contactPoint - b->mX;
		float numerator = -(1 + epsilon) * glm::dot(v, N);
		float term1 = 1 / b->mMass;
		float term3 = glm::dot(N, glm::cross(b->mIinv * glm::cross(localPoint, N), localPoint));

		float j = numerator / (term1 + term3);
		glm::vec3 force = j * N;

		return force;
	}

	void resolveFrictionContact(Contact* contact, float epsilon)
	{
		RigidBody* b = contact->body[0];
		glm::vec3& N = contact->contactNormal;
		glm::vec3 v = b->mV + glm::cross(b->mW, contact->contactPoint - b->mX);
		
		auto forceNormal = applyImpulse(b, v, contact->contactPoint, N, epsilon);
		
		b->mP += forceNormal;
		b->mL += glm::cross(contact->contactPoint - b->mX, forceNormal);
		b->mV = b->mP / b->mMass;
		b->mW = b->mIinv * b->mL;

		v = b->mV + glm::cross(b->mW, contact->contactPoint - b->mX);
		
		epsilon = 0.0f; // coefficent of restitution 0 <= epsilon <= 1
		glm::vec3 velToKill = (v - glm::dot(v, N) * N);
		
		if (glm::length(velToKill) > 0)
		{
			glm::vec3 Nplanar = -glm::normalize(velToKill);
			auto forcePlanar = applyImpulse(b, v, contact->contactPoint, Nplanar, epsilon);
		
			b->mP += forcePlanar;
			b->mL += glm::cross(contact->contactPoint - b->mX, forcePlanar);
		}
		
		b->mV = b->mP / b->mMass;
		b->mW = b->mIinv * b->mL;
	}

	void velocity(RigidBody* b, const glm::vec3& P, glm::vec3& v)
	{
		if (b)
		{
			v = b->mV + glm::cross(b->mW, P - b->mX);
		}
	}

	glm::vec3 applyImpulse(Contact* contact, float epsilon)
	{
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		glm::vec3 v1(0.0f);
		glm::vec3 v2(0.0f);
		velocity(b1, contact->contactPoint, v1);
		velocity(b2, contact->contactPoint, v2);

		glm::vec3 v = v1 - v2;
		glm::vec3& N = contact->contactNormal;
		glm::vec3 localPoint1 = b1 ? contact->contactPoint - b1->mX : glm::vec3(0.0f);
		glm::vec3 localPoint2 = b2 ? contact->contactPoint - b2->mX : glm::vec3(0.0f);

		float numerator = -(1 + epsilon) * glm::dot(v, N);
		float term1 = b1 ? 1 / b1->mMass : 0.0f;
		float term2 = b2 ? 1 / b2->mMass : 0.0f;
		float term3 = b1 ? glm::dot(N, glm::cross(b1->mIinv * glm::cross(localPoint1, N), localPoint1)) : 0.0f;
		float term4 = b2 ? glm::dot(N, glm::cross(b2->mIinv * glm::cross(localPoint2, N), localPoint2)) : 0.0f;

		float term = term1 + term2 + term3 + term4;
		float j = term > 0.0f ? numerator / (term1 + term3) : 0.0f;
		glm::vec3 force = j * N;

		return force;
	}

	void resolveFrictionlessContact(Contact* contact, float epsilon)
	{
		auto force = applyImpulse(contact, epsilon);

		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		if (b1)
		{
			b1->mP += force;
			b1->mL += glm::cross(contact->contactPoint - b1->mX, force);

			b1->mV = b1->mP / b1->mMass;
			b1->mW = b1->mIinv * b1->mL;
		}
		
		if (b2)
		{
			b2->mP -= force;
			b2->mL -= glm::cross(contact->contactPoint - b2->mX, force);

			b2->mV = b2->mP / b2->mMass;
			b2->mW = b2->mIinv * b2->mL;
		}
	}
}

CollisionResponse::CollisionResponse()
{
	//mContacts = new Contact[MaxContacts];
	//mContactItt = mContacts;
}

//void CollisionResponse::generateContacts(std::vector<RigidBody*>& bodies)
//{
//	mContactItt = mContacts;
//
//	for (int i = 0; i < bodies.size(); i++)
//	{
//		RigidBody* b = bodies[i];
//		for (int i = 0; i < 8; ++i)
//		{
//			Contact* contact = mContactItt;
//			if (hasContact(b, vertex[i], contact))
//			{
//				contact->localContactId = i;
//				mContactItt++;
//				contact->nextContact = mContactItt;
//			}
//		}
//	}
//}

void CollisionResponse::update(CollisionData* cData, std::vector<CollisionPrimitive*>& primitives)
{
	//collision detection
	//generateContacts(bodies);
	if (cData->contactCount > 0)
	{
		for (int i = 0; i < primitives.size(); i++)
		{
			RigidBody* b = primitives[i]->body;

			float max = 0.0f;
			int index = -1;

			for (int j = 0; j < cData->contactCount; j++)
			{
				Contact* c = &(cData->contactArray[j]);

				if (c->body[0] == b)
				{
					if (c->contactDepth > max)
					{
						max = c->contactDepth;
						index = j;
					}
				}
			}

			if (index != -1)
			{
				Contact* c_index = &(cData->contactArray[index]);
			
				glm::vec3 linearChange[2], angularChange[2];
				c_index->applyPositionChange(linearChange,
											 angularChange,
											 max);
			
				//update contacts due to the resolving interpenetration
				for (int k = 0; k < cData->contactCount; k++)
				{
					Contact* c_k = &(cData->contactArray[k]);

					// Check each body in the contact
					for (unsigned b = 0; b < 2; b++) if (c_k->body[b])
					{
						// Check for a match with each body in the newly
						// resolved contact
						for (unsigned d = 0; d < 2; d++)
						{
							if (c_k->body[b] == c_index->body[d])
							{
								glm::vec3 deltaPosition = linearChange[d] +
									glm::cross(angularChange[d], c_k->relContactPoint[b]);

								// The sign of the change is positive if we're
								// dealing with the second body in a contact
								// and negative otherwise (because we're
								// subtracting the resolution)..
								c_k->contactDepth +=
									glm::dot(deltaPosition, c_k->contactNormal)
									* (b ? 1 : -1);
							}
						}
					}

					//Contact* cc = &(cData->contactArray[k]);
					//if (cc->body[0] == b)
					//{
						//collisionDetection(cc->body[0], vertex[cc->localContactId], cc);
					//}
				}
			}
			
			for (int velIteration = 0; velIteration < 5; ++velIteration)
			{
				float max = 0.0f;
				int index = -1;

				for (int j = 0; j < cData->contactCount; j++)
				{
					Contact* c = &(cData->contactArray[j]);

					if (c->body[0] == b)
					{
						float desiredVel = desiredVelocity(c, 0.4f);
						if (desiredVel > max)
						{
							max = desiredVel;
							index = j;
						}
					}
				}

				if (index != -1)
				{
					Contact* c = &(cData->contactArray[index]);
					resolveFrictionlessContact(c, 0.2f);
					//resolveFrictionContact(c, 0.0f);
				}
			}
		}
	}
}
