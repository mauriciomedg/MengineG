#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RigidBody
{
	static const int STATE_SIZE;
public:

	RigidBody();
	void init(glm::vec3 pos, glm::vec3 v, glm::quat Q);

	void prepareSystem(float* y, float* ydot, float deltaT, const glm::vec3& gravity);


	void ddtStateToArray(float* ydot);
	void arrayToState(float* y);
	void stateToArray(float* y);

	void computeForceAndTorque(float deltaT, const glm::vec3& gravity);

	void update(float* y);

	// Constants
	float mMass;
	glm::mat3 mIbody;
	glm::mat3 mIbodyInv;

	// State variables
	glm::vec3 mX;
	glm::quat mQ;
	//glm::mat3 mR;
	glm::vec3 mP;
	glm::vec3 mL;

	//Derived quantites
	glm::vec3 mV;
	glm::mat3 mIinv;
	glm::vec3 mW; //w(t)

	// Computed quantities
	glm::vec3 mForce;
	glm::vec3 mTorque;

	/////////////////////////////////
	glm::vec3 mForceInput; /* force from inputs */
	glm::vec3 mLastForceInputConsume;

	void addMovement(glm::vec3& intensity, float scale);
	void ConsumeForceInput();
};