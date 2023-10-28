#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct RigidBody
{

public:
	RigidBody();
	void init(glm::vec3 pos, glm::vec3 v, glm::mat3 R);

	// Constants
	float mMass;
	glm::mat3 mIbody;
	glm::mat3 mIbodyInv;

	// State variables
	glm::vec3 mX;
	glm::mat3 mR;
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
	glm::vec3& ConsumeForceInput();
};