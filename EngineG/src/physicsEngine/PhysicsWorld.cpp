#pragma once

#include "PhysicsWorld.h"
#include "RigidBody.h"

const int PhysicsWorld::STATE_SIZE = 18;

/* Copy the state information into an array */
void PhysicsWorld::stateToArray(RigidBody* rb, float* y)
{
	*y++ = rb->mX[0]; /* x component of position */
	*y++ = rb->mX[1]; /* etc. */
	*y++ = rb->mX[2];
	for (int i = 0; i < 3; i++) /* copy rotation matrix */
		for (int j = 0; j < 3; j++)
			*y++ = rb->mR[i][j];

	*y++ = rb->mP[0];
	*y++ = rb->mP[1];
	*y++ = rb->mP[2];
	*y++ = rb->mL[0];
	*y++ = rb->mL[1];
	*y++ = rb->mL[2];
}

/* Copy information from an array into the state variables */
void PhysicsWorld::arrayToState(RigidBody* rb, float* y)
{
	rb->mX[0] = *y++;
	rb->mX[1] = *y++;
	rb->mX[2] = *y++;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rb->mR[i][j] = *y++;
	rb->mP[0] = *y++;
	rb->mP[1] = *y++;
	rb->mP[2] = *y++;
	rb->mL[0] = *y++;
	rb->mL[1] = *y++;
	rb->mL[2] = *y++;
	/* Compute auxiliary variables... */
	/* v(t) = P(t) * M */
	rb->mV = rb->mP / rb->mMass;
	/* I−1(t) = R(t)I−1 bodyR(t)T*/
	rb->mIinv = rb->mR * rb->mIbodyInv * glm::transpose(rb->mR);
	/* ω(t) = I−1(t)L(t) */
	rb->mW = rb->mIinv * rb->mL;
}

void PhysicsWorld::arrayToBody(float* y)
{
	for (int i = 0; i < mBodies.size(); i++)
		arrayToState(mBodies[i], &y[i * STATE_SIZE]);
}

void PhysicsWorld::bodiesToArray()
{
	for (int i = 0; i < mBodies.size(); i++)
		stateToArray(mBodies[i], &mYfinal[i * STATE_SIZE]);
}

void PhysicsWorld::computeForceAndTorque(float t, RigidBody* body)
{
	// gravity and drag
	float k_drag = 1.0f;

	body->mForce = glm::vec3(0.0f, 0.0f, 0.0f);
	body->mTorque = glm::vec3(0.0f, 0.0f, 0.0f);

	body->mForce += body->mMass * mGravity - k_drag * body->mV;
}

namespace
{
	glm::mat3 Star(glm::vec3& a)
	{
		glm::mat3 skew(0.0f);

		skew[0][1] = -a[2];
		skew[0][2] = -a[1];
		skew[1][2] = -a[0];

		glm::mat3 result(skew + glm::transpose(skew));
		return result;
	}
}
void PhysicsWorld::ddtStateToArray(RigidBody* rb, float* ydot)
{
	/* copy d
	dt x(t) = v(t) into ydot */
	*ydot++ = rb->mV[0];
	*ydot++ = rb->mV[1];
	*ydot++ = rb->mV[2];
	/* Compute R˙(t) = ω(t)∗R(t) */
	glm::mat3 Rdot = Star(rb->mW) * rb->mR;
	/* copy R˙(t) into array */
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			*ydot++ = Rdot[i][j];
	*ydot++ = rb->mForce[0]; /* dt P(t) = F(t) */
	*ydot++ = rb->mForce[1];
	*ydot++ = rb->mForce[2];
	*ydot++ = rb->mTorque[0]; /* dt L(t) = τ(t) */
	*ydot++ = rb->mTorque[1];
	*ydot++ = rb->mTorque[2];
}

void PhysicsWorld::dydt(float deltaT, float* ydot)
{
	/* put data in y[ ] into Bodies[ ] */
	arrayToBody(&mYfinal[0]);
	
	for (int i = 0; i < mBodies.size(); i++)
	{
		computeForceAndTorque(deltaT, mBodies[i]);
		ddtStateToArray(mBodies[i], &ydot[i * STATE_SIZE]);
	}
}

namespace
{
	void ScaleVector(std::vector<float>& dst, float DeltaT)
	{
		for (int i = 0; i < dst.size(); ++i)
		{
			dst[i] *= DeltaT;
		}
	}

	void AddVectors(std::vector<float>& src, std::vector<float>& dst)
	{
		for (int i = 0; i < src.size(); ++i) {
			dst[i] += src[i];
		}
	}
}

void PhysicsWorld::ode(float* y0, float* yEnd, float deltaT)
{
	dydt(deltaT, &mYdot[0]);
	ScaleVector(mYdot, deltaT);
	AddVectors(mYdot, mYfinal); /* add -> temp2 */
}


void PhysicsWorld::runSimulation(float deltaT)
{
	/* copy yfinal back to y0 */
	for (int i = 0; i < STATE_SIZE * mBodies.size(); i++)
		mY[i] = mYfinal[i];
	ode(&mY[0], &mYfinal[0], deltaT);
	/* copy d
	dtY(t + 1
	30 ) into state variables */
	arrayToBody(&mYfinal[0]);
}

void PhysicsWorld::init()
{
	mGravity = glm::vec3(0.0f, -98.0f, 0.0f);

	mY.resize(STATE_SIZE * mBodies.size());
	mYfinal.resize(STATE_SIZE * mBodies.size());
	mYdot.resize(STATE_SIZE * mBodies.size());

	bodiesToArray();
}
