#include "RigidBody.h"

const int RigidBody::STATE_SIZE = 13;

namespace
{
	bool applyConnection(RigidBody* b, glm::vec4 pLocal, float dt)
	{
		float ks = 50.0f;
		float kd = 10.1f;
		glm::vec3 P(0.0f, 35.0f, 0.0f);
		glm::vec3 N(0.0f, 1.0f, 0.0f);

		glm::vec3 pWorld = (b->mWorldMat * pLocal);
		
		glm::vec3 v = b->mV + glm::cross(b->mW, glm::vec3(pWorld - b->mX));

		float depth = glm::dot(pWorld + v * dt - P, N);

		if (depth < 0.0f && glm::dot(v, N) < 0)
		{
			float epsilon = 0.2f; // coefficent of restitution 0 <= epsilon <= 1
			float numerator = -(1 + epsilon) * glm::dot(v, N);
			
			pWorld += -(b->mX);
			
			float term1 = 1 / b->mMass;
			float term3 = glm::dot(N, glm::cross(b->mIbodyInv * glm::cross(pWorld, N), pWorld));
			
			float j = numerator / (term1 + term3);
			glm::vec3 force = j * N;

			b->mP += force;
			b->mL += glm::cross(pWorld, force);

			b->mV = b->mP / b->mMass;
			b->mW = b->mIbodyInv * b->mL;

			return true;
		}

		return false;
	}

	bool Surface_Connection(RigidBody* b, float dt)
	{
		glm::vec4 pLocal = glm::vec4(1.0f);
		return applyConnection(b, pLocal, dt);

		//pLocal = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0);
		//applyConnection(b, pLocal, dt);
		//
		//pLocal = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0);
		//applyConnection(b, pLocal, dt);
		//
		//pLocal = glm::vec4(1.0f, -1.0f, 1.0f, 1.0);
		//applyConnection(b, pLocal, dt);
		//
		//pLocal = glm::vec4(1.0f, -1.0f, -1.0f, 1.0);
		//applyConnection(b, pLocal, dt);
		//
		//pLocal = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0);
		//applyConnection(b, pLocal, dt);
		//
		//pLocal = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0);
		//applyConnection(b, pLocal, dt);
		//
		//pLocal = glm::vec4(1.0f, 1.0f, -1.0f, 1.0);
		//applyConnection(b, pLocal, dt);

	}
}

RigidBody::RigidBody()
	: mMass(1.0f), mL(0.0f), mForce(0.0f), mV(0.0f)
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
	if (Surface_Connection(this, deltaT))
	{
		//arrayToState(y);
		stateToArray(y);
	}
	else
	{
		ddtStateToArray(ydot);
	}
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
	float k_drag = 0.8f;

	// Clean
	mForce = glm::vec3(0.0f, 0.0f, 0.0f);
	mTorque = glm::vec3(0.0f, 0.0f, 0.0f);

	// Compute force and torque from external force
	glm::vec3& force = mLastForceInputConsume;

	glm::vec3 pos = (mWorldMat * glm::vec4(-0.5f, 0.0f, 0.0f, 1.0));
	pos = pos - mX;
	
	glm::vec3 torque = glm::cross(pos, force);
	mTorque += torque;
	mForce += mMass * gravity - k_drag * mV + force;
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

	mWorldMat = glm::translate(glm::mat4(1.0f), mX) * glm::mat4(mQ);
}

void RigidBody::update(float* y)
{
	arrayToState(y);
}