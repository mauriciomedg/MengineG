#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

typedef void (*dydt_func)(float t, float* ydot);

class RigidBody;
class CollisionDetection;
class CollisionResponse;
class Contact;
class CollisionPrimitive;

class PhysicsWorld
{
private:
	/** Holds the maximum number of contacts. */
	const static unsigned MAX_CONTACTS = 256;

public:
	void init();
	int instanciatePrimitive(const glm::mat4& transform, bool isSimulatingPhysics = false);
	int instanciatePrimitiveBox(const glm::mat4& transform, glm::vec3& halfSize, bool isSimulatingPhysics = false);
	int instanciatePrimitivePlane(const glm::mat4& transform, bool isSimulatingPhysics = false);
	void addMovement(int id, glm::vec3& intensity, float scale);
	void move(int id, glm::vec3& velocity);
	void setAffectedByGravity(int id, bool isAffetedByGravity);
	void setIgnoreCollision(int id, bool ignore);
	const glm::mat4& getPrimitiveLocation(int id) const;
	void runSimulation(float deltaT);

private:

	float mDeltaT;
	void generateContacts();
	glm::vec3 mGravity;
	int m_numberOfNodes;
	std::vector<float> mY;
	std::vector<float> mYdot;
	std::vector<CollisionPrimitive*> mPrimitive;
	Contact* mContacts;
	CollisionDetection* mCollisionDetection;
	CollisionResponse* mCollisionResponse;
};