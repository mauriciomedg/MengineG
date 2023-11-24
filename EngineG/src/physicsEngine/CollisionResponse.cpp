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
		glm::vec3& N = contact->contactNormal;
		
		glm::vec3 v = glm::cross(b->mW, glm::vec3(contact->contactPoint - b->mX));
		//v += b->mV;
	
		//glm::vec3 v = glm::cross(b->mPreviousW, glm::vec3(contact->contactPoint - b->mPreviousX));
		//v += b->mPreviousV;

		float threshold = 0.5f;
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

	bool hasContact(RigidBody* b, glm::vec4& pLocal, Contact* contact)
	{
		if (collisionDetection(b, pLocal, contact))
		{
			glm::vec3 pWorld = (b->mWorldMat * pLocal);
			contact->body[0] = b;
			contact->contactPoint = pWorld;
			contact->relContactPoint = pWorld - b->mX;

			contactPointDetermination(contact);
			return true;
		}
		else
		{
			return false;
		}
	}

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
		//float numerator = -glm::dot(v, N) - epsilon * (glm::dot(v, N) - glm::dot(b->mPreviousV, N));
		float term1 = 1 / b->mMass;
		float term3 = glm::dot(N, glm::cross(b->mIinv * glm::cross(localPoint, N), localPoint));

		float j = numerator / (term1 + term3);
		glm::vec3 force = j * N;

		return force;
	}

	glm::vec3 applyImpulse2(RigidBody* b, const glm::vec3& v, const glm::vec3& contactPoint, const glm::vec3& N, float epsilon)
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
			//std::cout << glm::length(velToKill) << std::endl;
			glm::vec3 Nplanar = -glm::normalize(velToKill);
			auto forcePlanar = applyImpulse(b, v, contact->contactPoint, Nplanar, epsilon);
		
			b->mP += forcePlanar;
			b->mL += glm::cross(contact->contactPoint - b->mX, forcePlanar);
		}
		
		b->mV = b->mP / b->mMass;
		b->mW = b->mIinv * b->mL;
	}

	void resolveFrictionlessContact(Contact* contact, float epsilon)
	{
		RigidBody* b = contact->body[0];
		glm::vec3& N = contact->contactNormal;
		glm::vec3 v = b->mV + glm::cross(b->mW, contact->contactPoint - b->mX);

		auto force = applyImpulse(b, v, contact->contactPoint, N, epsilon);

		b->mP += force;
		b->mL += glm::cross(contact->contactPoint - b->mX, force);
	
		b->mV = b->mP / b->mMass;
		b->mW = b->mIinv * b->mL;
	}
}

Contact::Contact()
{
	body[0] = nullptr;
	body[1] = nullptr;
}

void Contact::applyPositionChange(glm::vec3 linearChange[2],
	glm::vec3 angularChange[2],
	float penetration)
{
	const float angularLimit = 0.2f;
	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	// We need to work out the inertia of each object in the direction
	// of the contact normal, due to angular inertia only.
	for (unsigned i = 0; i < 1; i++) if (body[i])
	{
		glm::mat3 inverseInertiaTensor = body[i]->mIinv;

		// Use the same procedure as for calculating frictionless
		// velocity change to work out the angular inertia.
		float term3 = glm::dot(contactNormal, glm::cross(inverseInertiaTensor * glm::cross(relContactPoint, contactNormal), relContactPoint));
		angularInertia[i] = term3;

		// The linear component is simply the inverse mass
		float term1 = 1 / body[i]->mMass;
		linearInertia[i] = term1;

		// Keep track of the total inertia from all components
		totalInertia += linearInertia[i] + angularInertia[i];

		// We break the loop here so that the totalInertia value is
		// completely calculated (by both iterations) before
		// continuing.
	}

	// Loop through again calculating and applying the changes
	for (unsigned i = 0; i < 1; i++) if (body[i])
	{
		glm::mat3 inverseInertiaTensor = body[i]->mIinv;

		// The linear and angular movements required are in proportion to
		// the two inverse inertias.
		float sign = (i == 0) ? 1 : -1;
		angularMove[i] =
			sign * contactDepth * (angularInertia[i] / totalInertia);
		linearMove[i] =
			sign * contactDepth * (linearInertia[i] / totalInertia);

		// To avoid angular projections that are too great (when mass is large
		// but inertia tensor is small) limit the angular move.

		glm::vec3 projection = relContactPoint;
		projection += contactNormal * (glm::dot(-relContactPoint, contactNormal));

		// Use the small angle approximation for the sine of the angle (i.e.
		// the magnitude would be sine(angularLimit) * projection.magnitude
		// but we approximate sine(angularLimit) to angularLimit).
		float maxMagnitude = angularLimit * projection.length();

		if (angularMove[i] < -maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if (angularMove[i] > maxMagnitude)
		{
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		// We have the linear amount of movement required by turning
		// the rigid body (in angularMove[i]). We now need to
		// calculate the desired rotation to achieve that.
		if (angularMove[i] == 0)
		{
			// Easy case - no angular movement means no rotation.
			angularChange[i] = glm::vec3(0);
		}
		else
		{
			// Work out the direction we'd like to rotate in.
			glm::vec3 targetAngularDirection =
				glm::cross(relContactPoint, contactNormal);

			// Work out the direction we'd need to rotate to achieve that
			angularChange[i] =
				(inverseInertiaTensor * targetAngularDirection) *
				(angularMove[i] / angularInertia[i]);
		}

		// Velocity change is easier - it is just the linear movement
		// along the contact normal.
		linearChange[i] = contactNormal * linearMove[i];

		// Now we can start to apply the values we've calculated.
		// Apply the linear movement
		body[i]->mX += contactNormal * linearMove[i];

		// And the change in orientation
		glm::quat Wquad(0.0f, angularChange[i][0], angularChange[i][1], angularChange[i][2]);
		glm::quat qdot = 0.5f * (Wquad * body[i]->mQ);
		body[i]->mQ += qdot;


		// We need to calculate the derived data for any body that is
		// asleep, so that the changes are reflected in the object's
		// data. Otherwise the resolution will not change the position
		// of the object, and the next collision detection round will
		// have the same penetration.
		// 
		//if (!body[i]->getAwake()) body[i]->calculateDerivedData();
		body[i]->calculateInternalData();
	}
}

CollisionResponse::CollisionResponse()
{
	mContacts = new Contact[MaxContacts];
	mContactItt = mContacts;
}

void CollisionResponse::generateContacts(std::vector<RigidBody*>& bodies)
{
	mContactItt = mContacts;

	for (int i = 0; i < bodies.size(); i++)
	{
		RigidBody* b = bodies[i];
		for (int i = 0; i < 8; ++i)
		{
			Contact* contact = mContactItt;
			if (hasContact(b, vertex[i], contact))
			{
				contact->localContactId = i;
				mContactItt++;
				contact->nextContact = mContactItt;
			}
		}
	}
}

void CollisionResponse::update(std::vector<RigidBody*>& bodies)
{
	//collision detection
	generateContacts(bodies);

	int distance = std::distance(mContacts, mContactItt);

	if (distance > 0)
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			RigidBody* b = bodies[i];

			float max = 0.0f;
			int index = -1;

			for (int j = 0; j < distance; j++)
			{
				Contact* c = &(mContacts[j]);

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
				Contact* c = &(mContacts[index]);

				glm::vec3 linearChange[2], angularChange[2];
				(&mContacts[index])->applyPositionChange(linearChange,
					angularChange,
					max);

				//update contacts due to the resolving interpenetration
				for (int k = 0; k < distance; k++)
				{
					Contact* cc = &(mContacts[k]);
					if (cc->body[0] == b)
					{
						collisionDetection(cc->body[0], vertex[cc->localContactId], cc);
					}
				}
			}
			
			for (int velIteration = 0; velIteration < 5; ++velIteration)
			{
				float max = 0.0f;
				int index = -1;

				for (int j = 0; j < distance; j++)
				{
					Contact* c = &(mContacts[j]);

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
					Contact* c = &(mContacts[index]);
					//resolveFrictionlessContact(c, 0.4f);
					resolveFrictionContact(c, 0.0f);
				}
			}
		}
	}
}
