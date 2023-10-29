#include "RigidBody.h"

const int RigidBody::STATE_SIZE = 13;

RigidBody::RigidBody()
	: mMass(0.1f), mL(0.0f), mForce(0.0f)
{
}

void RigidBody::init(glm::vec3 pos, glm::vec3 v, glm::quat Q)
{
	mX = pos; 
	mP = mMass * v;
	mQ = Q;

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
	//computeConstraints()
	ddtStateToArray(ydot);
}

void RigidBody::addMovement(glm::vec3& intensity, float scale)
{
	mForceInput += intensity * scale;
}

void RigidBody::ConsumeForceInput()
{
	mLastForceInputConsume = mForceInput;
	mForceInput = glm::vec3(0.0f);
}

void RigidBody::computeForceAndTorque(float deltaT, const glm::vec3& gravity)
{
	// gravity and drag
	float k_drag = 0.1f;

	// Clean
	mForce = glm::vec3(0.0f, 0.0f, 0.0f);
	mTorque = glm::vec3(0.0f, 0.0f, 0.0f);

	// Compute force and torque from external force
	glm::vec3& force = mLastForceInputConsume;

	glm::vec3 localPos = glm::vec3(-0.5f, 0.0f, 0.0f);
	//glm::vec3 forceAppLocation = body->mX + localPos;
	localPos = glm::normalize(localPos);
	glm::vec3 forceResult = glm::dot(localPos, force) * localPos;

	glm::vec3 torque = glm::cross(localPos, force);
	mTorque += torque;
	mForce += mMass * gravity - k_drag * mV + forceResult;
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
void RigidBody::arrayToState(float* y)
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

	mQ = glm::normalize(mQ);
	glm::mat3 R(mQ);
	/* Compute auxiliary variables... */
	/* v(t) = P(t) * M */
	mV = mP / mMass;
	/* I−1(t) = R(t)I−1 bodyR(t)T*/
	mIinv = R * mIbodyInv * glm::transpose(R);
	/* ω(t) = I−1(t)L(t) */
	mW = mIinv * mL;
}

void RigidBody::update(float* y)
{
	arrayToState(y);
}