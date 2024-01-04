#include "Contact.h"
#include "RigidBody.h"

Contact::Contact()
{
	body[0] = nullptr;
	body[1] = nullptr;
}

void Contact::setBodyData(RigidBody* one, RigidBody* two,
	float frictionCoef, float restitutionCoef)
{
	body[0] = one;
	body[1] = two;
	friction = frictionCoef;
	restitution = restitutionCoef;
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
	for (unsigned i = 0; i < 2; i++) if (body[i])
	{
		glm::mat3 inverseInertiaTensor = body[i]->mIinv;

		// Use the same procedure as for calculating frictionless
		// velocity change to work out the angular inertia.
		float term3 = glm::dot(contactNormal, glm::cross(inverseInertiaTensor * glm::cross(relContactPoint[i], contactNormal), relContactPoint[i]));
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
	for (unsigned i = 0; i < 2; i++) if (body[i])
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

		glm::vec3 projection = relContactPoint[i];
		projection += contactNormal * (glm::dot(-relContactPoint[i], contactNormal));

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
				glm::cross(relContactPoint[i], contactNormal);

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