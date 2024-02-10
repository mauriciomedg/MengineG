#include "ContactConstraint.h"
#include "../RigidBody.h"
#include "../MathUtils.h"
#include "../Contact.h"
#include <iostream>

#include <vector>

namespace MG
{
	ContactConstraint::ContactConstraint(Contact* c)
	{
		mBody[0] = c->body[0];
		mBody[1] = c->body[1];
		
		//mLocalContactPoint[0] = glm::inverse(glm::mat3(mBody[0]->mQ)) * (c->contactPoint - mBody[0]->mX);
		//mLocalContactPoint[1] = glm::inverse(glm::mat3(mBody[1]->mQ)) * (c->contactPoint - c->contactDepth * c->contactNormal - mBody[1]->mX);

		mContactPoint = c->contactPoint;
		mLocalContactPoint[0] = glm::inverse(glm::mat3(mBody[0]->mQ)) * (c->contactPoint + c->contactDepth * c->contactNormal * 0.5f - mBody[0]->mX);
		mLocalContactPoint[1] = glm::inverse(glm::mat3(mBody[1]->mQ)) * (c->contactPoint - c->contactDepth * c->contactNormal * 0.5f - mBody[1]->mX);

		mN = c->contactNormal;
		mTotalCorrection = c->contactDepth;
		mDepth = glm::min(-c->contactDepth, 0.0f);

		glm::vec3 r1 = mContactPoint - mBody[0]->mX;
		glm::vec3 r2 = mContactPoint - mBody[1]->mX;
		mV[0] = mBody[0]->mV + glm::cross(mBody[0]->mW, r1);
		mV[1] = mBody[1]->mV + glm::cross(mBody[1]->mW, r2);
	}

	void ContactConstraint::execute(float dt)
	{
		if (!mBody[0] || !mBody[1]) return;
		
		RigidBody* b1 = mBody[0];
		RigidBody* b2 = mBody[1];
		
		glm::vec3 r1 = (mBody[0]->mQ) * mLocalContactPoint[0];
		glm::vec3 x1 = b1->mX + r1;
		
		glm::vec3 r2 = (mBody[1]->mQ) * mLocalContactPoint[1];
		glm::vec3 x2 = b2->mX + r2;
		
		glm::vec3 X(x2 - x1);
		float C = glm::dot(X, mN);
		
		if (C >= 0.0f)
			return;

		// mass
		std::vector<float> Minv;
		Minv.resize(3 * 3, 0.0f);
		for (int j = 0; j < 3; ++j)
			Minv[j + j * 3] = b1->mShape->getMassInv();
		
		addBlockDiagonal(Minv, 3, 3, b1->mIinvArray, 3, 3);
		//
		std::vector<float> Minv2;
		Minv2.resize(3 * 3, 0.0f);
		for (int j = 0; j < 3; ++j)
			Minv2[j + j * 3] = b2->mShape->getMassInv();
		
		addBlockDiagonal(Minv2, 3, 3, b2->mIinvArray, 3, 3);
		//
		
		addBlockDiagonal(Minv, 6, 6, Minv2, 6, 6);
		
		/////////////////////////////////////////////////////////////
		auto r1crossN = glm::cross(r1, mN);
		auto r2crossN = glm::cross(r2, mN);
		
		std::vector<float> J( //<1, 12>
			{ mN[0],  mN[1],  mN[2], r1crossN[0],  r1crossN[1],  r1crossN[2],
			   -mN[0],  -mN[1], -mN[2],-r2crossN[0], -r2crossN[1], -r2crossN[2] });
		
		




		//if (C >= 0.0f || mTotalCorrection <= 0.0f)
		//	return;
		//
		//float s = 0.01f;
		//float Cmodif = glm::min(0.0f, C - mDepth) + s * std::max(C, mDepth);
		//
		////C = Cmodif;
		//
		//// mass
		//std::vector<float> Minv;
		//Minv.resize(3 * 3, 0.0f);
		//for (int j = 0; j < 3; ++j)
		//	Minv[j + j * 3] = b1->mShape->getMassInv();
		//
		//addBlockDiagonal(Minv, 3, 3, b1->mIinvArray, 3, 3);
		////
		//std::vector<float> Minv2;
		//Minv2.resize(3 * 3, 0.0f);
		//for (int j = 0; j < 3; ++j)
		//	Minv2[j + j * 3] = b2->mShape->getMassInv();
		//
		//addBlockDiagonal(Minv2, 3, 3, b2->mIinvArray, 3, 3);
		////
		//
		//addBlockDiagonal(Minv, 6, 6, Minv2, 6, 6);
		//
		///////////////////////////////////////////////////////////////
		//auto r1crossN = glm::cross(r1, mN);
		//auto r2crossN = glm::cross(r2, mN);
		//
		////std::vector<float> J( //<1, 12>
		////	{ -mN[0],  -mN[1],  -mN[2], -r1crossN[0],  -r1crossN[1],  -r1crossN[2],
		////	   mN[0],  mN[1],  mN[2], r2crossN[0],  r2crossN[1],  r2crossN[2] });
		//
		//std::vector<float> J( //<1, 12>
		//	{ mN[0],  mN[1],  mN[2], r1crossN[0],  r1crossN[1],  r1crossN[2],
		//	   -mN[0],  -mN[1], -mN[2],-r2crossN[0], -r2crossN[1], -r2crossN[2] });
		//
		//
		/////////////////////////////////////////////////////////
		//std::vector<float> MinvJt;
		//
		//auto rc = glm::sqrt(Minv.size());
		//MinvJt.resize(rc * 1);
		//multiply(Minv, rc, rc, false, J, 1, rc, true, MinvJt);
		//
		//std::vector<float> K;
		//K.resize(1 * 1);
		//
		//multiply(J, 1, rc, false, MinvJt, rc, 1, false, K);
		///////
		//
		//std::vector<float> p;
		//std::vector<float> Kinv;
		//choldc(K, 1, p);
		//invA(K, 1, p, Kinv);
		//
		//std::vector<float> matC({ -C });
		//
		//std::vector<float> A;
		//multiply(MinvJt, 12, 1, false, Kinv, 1, 1, false, A);
		//std::vector<float> dX;
		//multiply(A, 12, 1, false, matC, 1, 1, true, dX);
		//
		////////////////////////////////////////////////////////////////
		//
		//glm::quat Wquad(0.0f, dX[3], dX[4], dX[5]);
		//glm::quat qdot = 0.5f * (Wquad * b1->mQ);
		//b1->mQ += qdot;
		//
		//glm::quat Wquad2(0.0f, dX[9], dX[10], dX[11]);
		//glm::quat qdot2 = 0.5f * (Wquad2 * b2->mQ);
		//b2->mQ += qdot2;
		//
		//auto a1 = glm::vec3(dX[0], dX[1], dX[2]);
		//auto a2 = glm::vec3(dX[6], dX[7], dX[8]);
		//
		//auto d = glm::length(a1) + glm::length(a2);
		//
		////mDepth += d;
		//mTotalCorrection -= d;
		//
		//b1->mX += glm::vec3(dX[0], dX[1], dX[2]);
		//b2->mX += glm::vec3(dX[6], dX[7], dX[8]);
		//
		//b1->calculateInternalData();
		//b2->calculateInternalData();
		//
		//glm::vec3 r11 = mContactPoint - mBody[0]->mX;
		//glm::vec3 r22 = mContactPoint - mBody[1]->mX;
		//mV[0] = mBody[0]->mV + glm::cross(mBody[0]->mW, r11);
		//mV[1] = mBody[1]->mV + glm::cross(mBody[1]->mW, r22);
	}

	void ContactConstraint::update2(float dt)
	{
		//if (!mBody[0] || !mBody[1]) return;
		//
		//RigidBody* b1 = mBody[0];
		//RigidBody* b2 = mBody[1];
		//
		////glm::vec3 r1 = (mBody[0]->mQ) * mLocalContactPoint[0];
		////glm::vec3 r2 = (mBody[1]->mQ) * mLocalContactPoint[1];
		//
		//
		//glm::vec3 r1 = mContactPoint - b1->mX;
		//glm::vec3 r2 = mContactPoint - b2->mX;
		//
		//auto v1 = b1->mV + glm::cross(b1->mW, r1);
		//auto v2 = b2->mV + glm::cross(b2->mW, r2);
		//
		////auto v1 = b1->mV + glm::cross(b1->mW, r1);
		////auto v2 = b2->mV + glm::cross(b2->mW, r2);
		//auto vnTilde = glm::dot(mV[0] - mV[1], mN);
		//auto dv = v1 - v2;
		////dv += (v2 - v1) * glm::min(0.5f * dt, 1.0f);
		////auto dw = (b2->mW - b1->mW) * glm::min(0.5f * dt, 1.0f);
		//auto vn = glm::dot(dv, mN);
		////std::cout << vn << " " << 2 * glm::length(glm::vec3(0.0f, -98.0f, 0.0f)) * dt << std::endl;
		////if (vn < 0.01f)
		////	return;
		//float epsilon = 0.5f;
		//float numerator = (-vn + glm::min(-epsilon * vnTilde, 0.0f));
		//float term1 = b1->mShape->getMassInv();
		//float term2 = b2->mShape->getMassInv();
		//float term3 = glm::dot(mN, glm::cross(b1->mIinv * glm::cross(r1, mN), r1));
		//float term4 = glm::dot(mN, glm::cross(b2->mIinv * glm::cross(r2, mN), r2));
		//
		//float term = term1 + term2 + term3 + term4;
		//float j = numerator / term;
		//glm::vec3 force = j * mN;
		//
		/////
		//auto P = force;
		//auto L = glm::cross(r1, force);
		//
		//b1->mV += P * b1->mShape->getMassInv();
		//b1->mW += b1->mIinv * L;
		//b1->calculateInternalData();
		//
		////
		//P = force;
		//L = glm::cross(r2, force);
		//
		//b2->mV -= P * b2->mShape->getMassInv();
		//b2->mW -= b2->mIinv * L;
		//b2->calculateInternalData();
	}
}