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

void PhysicsWorld::bodyToArray()
{
	for (int i = 0; i < mBodies.size(); i++)
		stateToArray(mBodies[i], &mY[i * STATE_SIZE]);
}

void PhysicsWorld::computeForceAndTorque(float t, RigidBody* body)
{

}

void PhysicsWorld::ddtStateToArray(RigidBody* rb, float* ydot)
{
	/* copy d
	dt x(t) = v(t) into ydot */
	*ydot++ = rb->mV[0];
	*ydot++ = rb->mV[1];
	*ydot++ = rb->mV[2];
	/* Compute R˙(t) = ω(t)∗R(t) */
	glm::mat3 Rdot;// = Star(rb->mW) * rb->mR;
	/* copy R˙(t) into array */
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			*ydot++ = Rdot[i][j];
	*ydot++ = rb->mForce[0]; /* d
	dt P(t) = F(t) */
	*ydot++ = rb->mForce[1];
	*ydot++ = rb->mForce[2];
	*ydot++ = rb->mTorque[0]; /* d
	dt L(t) = τ(t) */
	*ydot++ = rb->mTorque[1];
	*ydot++ = rb->mTorque[2];
}

void PhysicsWorld::dydt(float t, std::vector<float>& ydot)
{
	/* put data in y[ ] into Bodies[ ] */
	arrayToBody(&mY[0]);

	for (int i = 0; i < mBodies.size(); i++)
	{
		computeForceAndTorque(t, mBodies[i]);
		ddtStateToArray(mBodies[i], &ydot[i * STATE_SIZE]);
	}
}
