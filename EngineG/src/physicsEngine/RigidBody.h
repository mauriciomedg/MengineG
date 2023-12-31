#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct RigidBody
{
	static const int STATE_SIZE;
public:

	RigidBody();
	void init(const glm::mat4& mMat, glm::vec3& halfSize);

	void prepareSystem(float* y, float* ydot, float deltaT, const glm::vec3& gravity);

	void ddtStateToArray(float* ydot);
	void arrayToState(float* y);
	void stateToArray(float* y);
	void computeForceAndTorque(float deltaT, const glm::vec3& gravity);
	void move(const glm::vec3& distance);
	void update(float* y);

	int mIndex = 0;

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
	glm::mat4 mWorldMat;
	glm::vec3 mPreviousAcc;
	glm::vec3 mPreviousV;
	glm::vec3 mPreviousW;
	glm::vec3 mPreviousX;

	// Computed quantities
	glm::vec3 mForce;
	glm::vec3 mTorque;

	/////////////////////////////////
	glm::vec3 mForceAdded;
	bool mIsAffectedByGravity = true;

	void addMovement(glm::vec3& intensity, float scale);
	void calculateInternalData();
};