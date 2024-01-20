#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class DynamicShape
{
public:
	DynamicShape();
	
	float getMass() const;
	float getMassInv() const;
	const glm::mat3& getIbody() const;
	const glm::mat3& getIbodyInv() const;

	static DynamicShape* createBlock(glm::vec3& halfSize, float mass);

protected:
	float mMass;
	float mMassInv;
	glm::mat3 mIbody;
	glm::mat3 mIbodyInv;
};

class DynamicBlock : public DynamicShape
{
public:
	DynamicBlock(glm::vec3& halfSize, float mass);
};

struct RigidBody
{
	static const int STATE_SIZE;
	static const int POSITION_BASE_STATE_SIZE;
public:

	RigidBody(DynamicShape* shape);
	void init(const glm::mat4& mMat);
	void computeForceAndTorque(float deltaT, const glm::vec3& gravity);
	void move(const glm::vec3& distance);
	void update(float dt);
	void update2(float dt);

	int mIndex = 0;

	// Constants
	//float mMass;
	//glm::mat3 mIbody;
	//glm::mat3 mIbodyInv;
	DynamicShape* mShape;

	// State variables
	glm::vec3 mX;
	glm::quat mQ;
	glm::vec3 mXprev;
	glm::quat mQprev;
	//glm::mat3 mR;
	glm::vec3 mP;
	glm::vec3 mL;

	//Derived quantites
	glm::vec3 mV;
	glm::mat3 mIinv;
	std::vector<float> mInvInertiaArray;
std::vector<float> mIinvArray;
	glm::mat3 mI;
	glm::vec3 mW; //w(t)
	glm::mat4 mWorldMat;

	// Computed quantities
	glm::vec3 mForce;
	glm::vec3 mTorque;

	/////////////////////////////////
	glm::vec3 mForceAdded;
	bool mIsAffectedByGravity = true;

	void addMovement(glm::vec3& intensity, float scale);
	void calculateInternalData();
};