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
		
		mLocalContactPoint[0] = glm::inverse(glm::mat3(mBody[0]->mQ)) * (c->contactPoint - mBody[0]->mX);
		mLocalContactPoint[1] = glm::inverse(glm::mat3(mBody[1]->mQ)) * (c->contactPoint - c->contactDepth * c->contactNormal - mBody[1]->mX);

		//mDr[0] = c->contactPoint - mBody[0]->mX;
		//mDr[1] = c->contactPoint - mBody[1]->mX;
		mN = c->contactNormal;
		mDepth = glm::min(-c->contactDepth, 0.0f);
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
		
		if (C >= 0)
			return;

		float s = 2.0f;
		float Cmodif = glm::min(0.0f, C - mDepth) + s * std::max(C, mDepth);
		
		C = Cmodif;

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

		//std::vector<float> J( //<1, 12>
		//	{ -mN[0],  -mN[1],  -mN[2], -r1crossN[0],  -r1crossN[1],  -r1crossN[2],
		//	   mN[0],  mN[1],  mN[2], r2crossN[0],  r2crossN[1],  r2crossN[2] });

		std::vector<float> J( //<1, 12>
			{ mN[0],  mN[1],  mN[2], r1crossN[0],  r1crossN[1],  r1crossN[2],
			   -mN[0],  -mN[1], -mN[2],-r2crossN[0], -r2crossN[1], -r2crossN[2] });


		///////////////////////////////////////////////////////
		std::vector<float> MinvJt;
		
		auto rc = glm::sqrt(Minv.size());
		MinvJt.resize(rc * 1);
		multiply(Minv, rc, rc, false, J, 1, rc, true, MinvJt);

		std::vector<float> K;
		K.resize(1 * 1);

		multiply(J, 1, rc, false, MinvJt, rc, 1, false, K);
		/////

		std::vector<float> p;
		std::vector<float> Kinv;
		choldc(K, 1, p);
		invA(K, 1, p, Kinv);

		std::vector<float> matC({ -C });

		std::vector<float> A;
		multiply(MinvJt, 12, 1, false, Kinv, 1, 1, false, A);
		std::vector<float> dX;
		multiply(A, 12, 1, false, matC, 1, 1, true, dX);

		//////////////////////////////////////////////////////////////
		
		glm::quat Wquad(0.0f, dX[3], dX[4], dX[5]);
		glm::quat qdot = 0.5f * (Wquad * b1->mQ);
		b1->mQ += qdot;

		glm::quat Wquad2(0.0f, dX[9], dX[10], dX[11]);
		glm::quat qdot2 = 0.5f * (Wquad2 * b2->mQ);
		b2->mQ += qdot2;
		
		b1->mX += glm::vec3(dX[0], dX[1], dX[2]);
		b2->mX += glm::vec3(dX[6], dX[7], dX[8]);
	}
}