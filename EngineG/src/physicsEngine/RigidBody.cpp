#include "RigidBody.h"

RigidBody::RigidBody()
{
}

/* Copy the state information into an array */
void RigidBody::stateToArray(RigidBody* rb, float* y)
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
void RigidBody::arrayToState(RigidBody* rb, float* y)
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
	rb->mV = rb->mP / mMass;
	/* I−1(t) = R(t)I−1 bodyR(t)T*/
	rb->mIinv = mR * mIbodyInv * glm::transpose(mR);
	/* ω(t) = I−1(t)L(t) */
	rb->mW = rb->mIinv * rb->mL;
}