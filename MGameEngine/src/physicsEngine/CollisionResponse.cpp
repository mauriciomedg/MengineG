#include "CollisionResponse.h"
#include <glm/gtc/type_ptr.hpp>
#include "RigidBody.h"
#include "Contact.h"
#include "CollisionDetection.h"
#include <iostream>

namespace
{

	void velocity(RigidBody* b, const glm::vec3& P, glm::vec3& v)
	{
		if (b)
		{
			v = b->mV + glm::cross(b->mW, P - b->mX);
		}
	}

	glm::vec3 desiredVelocity(Contact* contact)
	{
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		glm::vec3 v1(0.0f);
		glm::vec3 v2(0.0f);
		velocity(b1, contact->contactPoint, v1);
		velocity(b2, contact->contactPoint, v2);

		glm::vec3 v = v1 - v2;
		return v;
	}

	glm::vec3 applyImpulse(Contact* contact, glm::vec3& N, float epsilon)
	{
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		glm::vec3 localPoint1 = b1 ? contact->contactPoint - b1->mX : glm::vec3(0.0f);
		glm::vec3 localPoint2 = b2 ? contact->contactPoint - b2->mX : glm::vec3(0.0f);

		float numerator = -(1 + epsilon) * glm::dot(desiredVelocity(contact), N);
		float term1 = b1 ? b1->mShape->getMassInv() : 0.0f;
		float term2 = b2 ? b2->mShape->getMassInv() : 0.0f;
		float term3 = b1 ? glm::dot(N, glm::cross(b1->mIinv * glm::cross(localPoint1, N), localPoint1)) : 0.0f;
		float term4 = b2 ? glm::dot(N, glm::cross(b2->mIinv * glm::cross(localPoint2, N), localPoint2)) : 0.0f;

		float term = term1 + term2 + term3 + term4;
		float j = term > 0.0f ? numerator / term : 0.0f;
		glm::vec3 force = j * N;

		return force;
	}

	void resolveFrictionContact(Contact* contact)
	{
		glm::vec3& N = contact->contactNormal;
		auto force = applyImpulse(contact, N, contact->restitution);
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		if (b1)
		{
			b1->mP += force;
			b1->mL += glm::cross(contact->contactPoint - b1->mX, force);

			b1->mV = b1->mP * b1->mShape->getMassInv();
			b1->mW = b1->mIinv * b1->mL;
			b1->calculateInternalData();
		}

		if (b2)
		{
			b2->mP -= force;
			b2->mL -= glm::cross(contact->contactPoint - b2->mX, force);

			b2->mV = b2->mP * b2->mShape->getMassInv();
			b2->mW = b2->mIinv * b2->mL;
			b2->calculateInternalData();
		}

		//if (contact->friction >= 0.0f)
		//{
		//	glm::vec3 v = desiredVelocity(contact);
		//	glm::vec3 velToKill = (v - glm::dot(v, N) * N);
		//
		//	if (glm::length(velToKill) > 0)
		//	{
		//		glm::vec3 Nplanar = -glm::normalize(velToKill);
		//		glm::vec3 forcePlanar = applyImpulse(contact, Nplanar, contact->friction);
		//		
		//		if (b1)
		//		{
		//			b1->mP += forcePlanar;
		//			b1->mL += glm::cross(contact->contactPoint - b1->mX, forcePlanar);
		//
		//			b1->mV = b1->mP / b1->mMass;
		//			b1->mW = b1->mIinv * b1->mL;
		//		}
		//
		//		if (b2)
		//		{
		//			b2->mP -= forcePlanar;
		//			b2->mL -= glm::cross(contact->contactPoint - b2->mX, forcePlanar);
		//
		//			b2->mV = b2->mP / b2->mMass;
		//			b2->mW = b2->mIinv * b2->mL;
		//		}
		//	}
		//}
	}

	void resolvePosition(Contact* contact)
	{
		glm::vec3& N = contact->contactNormal;

		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];


	}

	void resolveContact(Contact* contact, float dt)
	{
		glm::vec3& N = contact->contactNormal;

		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		float term1 = 0.0f;
		float term2 = 0.0f;
		float term3 = 0.0f;
		float term4 = 0.0f;

		if (b1)
		{
			glm::vec3 localPoint1 = contact->contactPoint - b1->mX;
			term1 = b1->mShape->getMassInv();
			term3 = glm::dot(N, glm::cross(b1->mIinv * glm::cross(localPoint1, N), localPoint1));
		}

		if (b2)
		{
			glm::vec3 localPoint2 = contact->contactPoint - b2->mX;
			term2 = b2->mShape->getMassInv();
			term4 = glm::dot(N, glm::cross(b2->mIinv * glm::cross(localPoint2, N), localPoint2));
		}
		
		float epsilon = 0.2f;
		float velRel = glm::dot(desiredVelocity(contact), N);
		velRel = glm::min(velRel, 0.0f);
		float numerator = -(1 + epsilon) * velRel;
		float lambda = glm::max(contact->contactDepth, 0.0f);//numerator / (term1 + term2 + term3 + term4);
				
		glm::vec3 p = lambda * N;

		
		if (b1)
		{
			b1->mX += term1 * p * dt;
			contact->contactDepth -= glm::length(term1 * p * dt);

			auto L = glm::cross(contact->contactPoint - b1->mX, p);
			auto w = b1->mIinv * L;

			glm::quat Wquad(0.0f, w[0], w[1], w[2]);
			glm::quat qdot = 0.5f * (Wquad * b1->mQ);
			b1->mQ += qdot * dt;
			b1->calculateInternalData();
		}

		if (b2)
		{
			b2->mX -= term2 * p * dt;
			contact->contactDepth -= glm::length(term2 * p * dt);

			auto L = glm::cross(contact->contactPoint - b2->mX, p);
			auto w = b2->mIinv * L;

			glm::quat Wquad(0.0f, w[0], w[1], w[2]);
			glm::quat qdot = 0.5f * (Wquad * b2->mQ);
			b2->mQ -= qdot * dt;
			b2->calculateInternalData();
		}
	}
}

CollisionResponse::CollisionResponse()
{
	//mContacts = new Contact[MaxContacts];
	//mContactItt = mContacts;
}

void CollisionResponse::solvePositions(CollisionData* cData, std::vector<CollisionPrimitive*>& primitives)
{
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
		}
	}
}

void CollisionResponse::solvePositions(CollisionData* cData, float dt)
{
	for (int j = 0; j < cData->contactCount; j++)
	{
		Contact* c = &(cData->contactArray[j]);
		resolveContact(c, dt);
	}
}


void CollisionResponse::update(CollisionData* cData, std::vector<CollisionPrimitive*>& primitives)
{
	//collision detection
	//generateContacts(bodies);
	if (cData->contactCount > 0)
	{
		for (int i = 0; i < primitives.size(); i++)
		{
			RigidBody* b = primitives[i]->body;
			for (int velIteration = 0; velIteration < 5; ++velIteration)
			{
				float max = 0.0f;
				int index = -1;

				for (int j = 0; j < cData->contactCount; j++)
				{
					Contact* c = &(cData->contactArray[j]);

					if (c->body[0] == b)
					{
						glm::vec3& N = c->contactNormal;
						float desiredVel = -(1 + c->restitution) * glm::dot(desiredVelocity(c), N);
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
					resolveFrictionContact(c);
				}
			}
		}
	}
}
