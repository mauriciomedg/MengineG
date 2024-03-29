#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Constraints/Constraint.h"
#include "Constraints/ContactConstraint.h"

struct RigidBody;
class CollisionDetection;
class CollisionResponse;
class Contact;
class CollisionPrimitive;

class PhysicsWorld
{
private:
	/** Holds the maximum number of contacts. */
	const static unsigned MAX_CONTACTS = 1000;

public:
	void init();
	void addRigidPointRigidPointConstraint(int id1, const glm::vec3& localPost1, int id2, const glm::vec3& localPost2, float distanceOffset);
	void addRigidPointConstraint(int id, const glm::vec3& localPost, const glm::vec3& position, float distanceOffset);
	int instanciatePrimitive(const glm::mat4& transform, bool isSimulatingPhysics);
	int instanciatePrimitiveBox(const glm::mat4& transform, glm::vec3& halfSize, float mass, bool isSimulatingPhysics);
	int instanciatePrimitivePlane(const glm::mat4& transform, float mass, bool isSimulatingPhysics);
	void addMovement(int id, glm::vec3& intensity, float scale);
	void move(int id, glm::vec3& velocity);
	void setAffectedByGravity(int id, bool isAffetedByGravity);
	void setIgnoreCollision(int id, bool ignore);
	const glm::mat4& getPrimitiveLocation(int id) const;

	void simulating(float deltaT);

private:

	float mDeltaT;
	bool generateContacts(std::vector<MG::ContactConstraint>& contactConstraintList);

	glm::vec3 mGravity;

	std::vector<CollisionPrimitive*> mPrimitive;
	Contact* mContacts;
	CollisionDetection* mCollisionDetection;
	CollisionResponse* mCollisionResponse;
	std::vector<MG::Constraint*> mConstraints;
};