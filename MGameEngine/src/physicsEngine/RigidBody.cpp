﻿#include "RigidBody.h"
#include <iostream>
#include "MathUtils.h"

const int RigidBody::STATE_SIZE = 13;
const int RigidBody::POSITION_BASE_STATE_SIZE = 7;

DynamicShape::DynamicShape()
{}

float DynamicShape::getMass() const { return mMass; }
float DynamicShape::getMassInv() const { return mMassInv; }
const glm::mat3& DynamicShape::getIbody() const { return mIbody; }
const glm::mat3& DynamicShape::getIbodyInv() const { return mIbodyInv; }

DynamicShape* DynamicShape::createBlock(glm::vec3& halfSize, float mass)
{
	return new DynamicBlock(halfSize, mass);
}

DynamicBlock::DynamicBlock(glm::vec3& halfSize, float mass)
{
	mMass = mass;

	if (mass > 0)
	{
		mMassInv = 1 / mass;

		float x0 = 2.0f * halfSize[0];
		float y0 = 2.0f * halfSize[0];
		float z0 = 2.0f * halfSize[0];

		mIbody[0][0] = glm::pow(y0, 2) + glm::pow(z0, 2);
		mIbody[1][1] = glm::pow(x0, 2) + glm::pow(z0, 2);
		mIbody[2][2] = glm::pow(x0, 2) + glm::pow(y0, 2);

		mIbody *= mMass / 12;

		mIbodyInv = glm::inverse(mIbody);
	}
	else
	{
		mIbody *= 0.0f;
		mIbodyInv *= 0.0f;
		mMassInv = 0.0f;
	}
}

RigidBody::RigidBody(DynamicShape* shape)
	: mShape(shape), mL(0.0f), mForce(0.0f), mV(0.0f, 0.0f, 0.0f), mForceAdded(0.0f)
{
	mIinv *= 0.0f;
	mInvInertiaArray.resize(3 * 3, 0.0f);
	mIinvArray.resize(3 * 3, 0.0f);
}

void RigidBody::init(const glm::mat4& mMat)
{
	//mMass = mass;
	mX = mMat[3];
	mP = mShape->getMass() * mV;
	mQ = mMat;

	mWorldMat = mMat;

	for (int j = 0; j < 3; ++j)
		mInvInertiaArray[j + j * 3] = mShape->getMassInv();
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
	if (!mIsSimulatingPhysics) return;

	// gravity and drag
	float k_drag = 0.5f;

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
	mTorque += torque;
	mForce += mShape->getMass() * gravityAcc - k_drag * mV + force;

	mForceAdded = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RigidBody::calculateInternalData()
{
	if (!mIsSimulatingPhysics) return;

	mQ = glm::normalize(mQ);
	glm::mat3 R(mQ);
	/* Compute auxiliary variables... */
	/* v(t) = P(t) * M */
	//mV = mP / mMass;
	/* I−1(t) = R(t)I−1 bodyR(t)T*/
	mI = R * mShape->getIbody() * glm::transpose(R);
	mIinv = R * mShape->getIbodyInv() * glm::transpose(R);
	/* ω(t) = I−1(t)L(t) */
	//mW = mIinv * mL;

	mWorldMat = glm::translate(glm::mat4(1.0f), mX) * glm::mat4(mQ);

	mInvInertiaArray.clear();
	mInvInertiaArray.resize(3 * 3);
	for (int j = 0; j < 3; ++j)
		mInvInertiaArray[j + j * 3] = mShape->getMassInv();

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mIinvArray[j + i * 3] = mIinv[i][j];
		}
	}
	MG::addBlockDiagonal(mInvInertiaArray, 3, 3, mIinvArray, 3, 3);
}

void RigidBody::update1(float dt)
{
	if (!mIsSimulatingPhysics) return;

	//update velocities	
	mV += dt * mForce * mShape->getMassInv();
	mW += dt * mIinv * (mTorque - glm::cross(mW, glm::cross(mW, mI * mW)));
}

void RigidBody::update2(float dt)
{
	if (!mIsSimulatingPhysics) return;

	//update positions
	mXprev = mX;
	mX += dt * mV;
	mQprev = mQ;

	glm::quat Wquad(0.0f, mW[0], mW[1], mW[2]);
	glm::quat qdot = 0.5f * (Wquad * mQ);

	mQ += dt * qdot;

	calculateInternalData();
}

void RigidBody::postUpdate(float dt)
{
	if (!mIsSimulatingPhysics) return;

	mV = (mX - mXprev) / dt;
	auto dQ = mQ * glm::inverse(mQprev);
	
	//mW = 2.0f * glm::vec3(dQ[0], dQ[1], dQ[2]) / dt;
	mW = 2.0f * glm::vec3(dQ.x, dQ.y, dQ.z) / dt;
	mW = dQ.w >= 0.0f ? mW : -mW;
}
