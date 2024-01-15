#include "RigidBody.h"
#include <iostream>

const int RigidBody::STATE_SIZE = 13;
const int RigidBody::POSITION_BASE_STATE_SIZE = 7;

RigidBody::RigidBody()
	: mMass(1.0f), mL(0.0f), mForce(0.0f), mV(0.0f, 0.0f, 0.0f), mForceAdded(0.0f)
{
	mIinvArray.resize(3 * 3);
}

void RigidBody::init(const glm::mat4& mMat, glm::vec3& halfSize)
{
	mX = mMat[3];
	mP = mMass * mV;
	mQ = mMat;

	mWorldMat = mMat;

	float x0 = 2.0f * halfSize[0];
	float y0 = 2.0f * halfSize[0];
	float z0 = 2.0f * halfSize[0];

	mIbody[0][0] = glm::pow(y0, 2) + glm::pow(z0, 2);
	mIbody[1][1] = glm::pow(x0, 2) + glm::pow(z0, 2);
	mIbody[2][2] = glm::pow(x0, 2) + glm::pow(y0, 2);

	mIbody *= mMass / 12;

	mIbodyInv = glm::inverse(mIbody);
}

void RigidBody::addMovement(glm::vec3& intensity, float scale)
{
	mForceAdded += intensity * scale;
}

void RigidBody::move(const glm::vec3& distance)
{
	mX += distance;
	calculateInternalData();
}

void RigidBody::computeForceAndTorque(float deltaT, const glm::vec3& gravity)
{
	// gravity and drag
	float k_drag = 0.0f;

	glm::vec3 gravityAcc = mIsAffectedByGravity ? gravity : glm::vec3(0.0f);
	///////////////
	// Clean
	mForce = glm::vec3(0.0f, 0.0f, 0.0f);
	mTorque = glm::vec3(0.0f, 0.0f, 0.0f);

	// Compute force and torque from external force
	glm::vec3& force = mForceAdded;
	glm::vec3 pos = (mWorldMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0));
	pos = pos - mX;
	
	glm::vec3 torque = glm::cross(pos, force);
	mTorque += torque - 0.8f * mW;
	mForce += mMass * gravityAcc - k_drag * mV + force;

	mForceAdded = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::calculateInternalData()
{
	mQ = glm::normalize(mQ);
	glm::mat3 R(mQ);
	/* Compute auxiliary variables... */
	/* v(t) = P(t) * M */
	//mV = mP / mMass;
	/* I−1(t) = R(t)I−1 bodyR(t)T*/
	mI = R * mIbody * glm::transpose(R);
	mIinv = R * mIbodyInv * glm::transpose(R);
	/* ω(t) = I−1(t)L(t) */
	//mW = mIinv * mL;

	mWorldMat = glm::translate(glm::mat4(1.0f), mX) * glm::mat4(mQ);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mIinvArray[j + i * 3] = mIinv[i][j];
		}
	}
}

void RigidBody::update(float dt)
{
	mXprev = mX;
	mV += dt * mForce / mMass;
	mX += dt * mV;

	mQprev = mQ;
	mW += dt * mIinv * (mTorque - glm::cross(mW, glm::cross(mW, mI * mW)));
	
	glm::quat Wquad(0.0f, mW[0], mW[1], mW[2]);
	glm::quat qdot = 0.5f * (Wquad * mQ);

	mQ += dt * qdot;

	calculateInternalData();
}

void RigidBody::update2(float dt)
{
	mV = (mX - mXprev) / dt;
	auto dQ = mQ * glm::inverse(mQprev);
	mW = 2.0f * glm::vec3(dQ[0], dQ[1], dQ[2]) / dt;
	mW = dQ[3] >= 0.0f ? mW : -mW;
}
