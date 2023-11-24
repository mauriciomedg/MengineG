#include "RigidBody.h"
#include <iostream>

const int RigidBody::STATE_SIZE = 13;

RigidBody::RigidBody()
	: mMass(1.0f), mL(0.0f), mForce(0.0f), mV(0.0f, 0.0f, 0.0f), mForceAdded(0.0f),
	mPreviousAcc(0.0f), mPreviousV(0.0f), mPreviousW(0.0f), mPreviousX(0.0f)
{
}

void RigidBody::init(glm::mat4& mMat)
{
	mX = mMat[3];
	mP = mMass * mV;
	mQ = mMat;

	mWorldMat = mMat;

	float x0 = 2.0f;
	float y0 = 2.0f;
	float z0 = 2.0f;

	mIbody[0][0] = glm::pow(y0, 2) + glm::pow(z0, 2);
	mIbody[1][1] = glm::pow(x0, 2) + glm::pow(z0, 2);
	mIbody[2][2] = glm::pow(x0, 2) + glm::pow(y0, 2);

	mIbody *= mMass / 12;

	mIbodyInv = glm::inverse(mIbody);
}

void RigidBody::prepareSystem(float* y, float* ydot, float deltaT, const glm::vec3& gravity)
{
	stateToArray(y);
	computeForceAndTorque(deltaT, gravity);
	ddtStateToArray(ydot);
}

void RigidBody::addMovement(glm::vec3& intensity, float scale)
{
	mForceAdded += intensity * scale;
}

void RigidBody::computeForceAndTorque(float deltaT, const glm::vec3& gravity)
{
	// gravity and drag
	float k_drag = 0.0f;

	///////////////
	mPreviousAcc = mForce / mMass;
	mPreviousV = mPreviousAcc * deltaT;
	mPreviousW = mIinv * mL;
	mPreviousX = mX;
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
	mForce += mMass * gravity - k_drag * mV + force;

	mForceAdded = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::ddtStateToArray(float* ydot)
{
	*ydot++ = mV[0];
	*ydot++ = mV[1];
	*ydot++ = mV[2];

	glm::quat Wquad(0.0f, mW[0], mW[1], mW[2]);
	glm::quat qdot = 0.5f * (Wquad * mQ);

	*ydot++ = qdot[0];
	*ydot++ = qdot[1];
	*ydot++ = qdot[2];
	*ydot++ = qdot[3];

	*ydot++ = mForce[0]; /* dt P(t) = F(t) */
	*ydot++ = mForce[1];
	*ydot++ = mForce[2];
	*ydot++ = mTorque[0]; /* dt L(t) = τ(t) */
	*ydot++ = mTorque[1];
	*ydot++ = mTorque[2];
}

/* Copy the state information into an array */
void RigidBody::stateToArray(float* y)
{
	*y++ = mX[0]; /* x component of position */
	*y++ = mX[1]; /* etc. */
	*y++ = mX[2];
	
	*y++ = mQ[0];
	*y++ = mQ[1];
	*y++ = mQ[2];
	*y++ = mQ[3];

	*y++ = mP[0];
	*y++ = mP[1];
	*y++ = mP[2];
	*y++ = mL[0];
	*y++ = mL[1];
	*y++ = mL[2];
}

/* Copy information from an array into the state variables */
void RigidBody::arrayToState(float* y, float dt)
{
	mX[0] = *y++;
	mX[1] = *y++;
	mX[2] = *y++;

	mQ[0] = *y++;
	mQ[1] = *y++;
	mQ[2] = *y++;
	mQ[3] = *y++;
	
	mP[0] = *y++;
	mP[1] = *y++;
	mP[2] = *y++;
	mL[0] = *y++;
	mL[1] = *y++;
	mL[2] = *y++;

	calculateInternalData(dt);
}

void RigidBody::calculateInternalData(float deltaT)
{
	mQ = glm::normalize(mQ);
	glm::mat3 R(mQ);
	/* Compute auxiliary variables... */
	/* v(t) = P(t) * M */
	mV = mP / mMass;
	/* I−1(t) = R(t)I−1 bodyR(t)T*/
	mIinv = R * mIbodyInv * glm::transpose(R);
	/* ω(t) = I−1(t)L(t) */
	mW = mIinv * mL;

	mWorldMat = glm::translate(glm::mat4(1.0f), mX) * glm::mat4(mQ);

}

void RigidBody::update(float* y, float dt)
{
	arrayToState(y, dt);
}