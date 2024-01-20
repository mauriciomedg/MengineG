#include "RigidPointRigidPointConstraint.h"
#include "../RigidBody.h"
#include "../MathUtils.h"

#include <vector>

namespace MG
{
	RigidPointRigidPointConstraint::RigidPointRigidPointConstraint(RigidBody* b1, const glm::vec3& localPost1, RigidBody* b2, const glm::vec3& localPost2, float distanceOffset)
	{
		mBody[0] = b1;
		mBody[1] = b2;
		mLocalPost[0] = localPost1;
		mLocalPost[1] = localPost2;
		mDistanceOffset = distanceOffset;
	}

	void RigidPointRigidPointConstraint::execute(float dt)
	{
		if (!mBody[0] || !mBody[1]) return;

		RigidBody* b1 = mBody[0];
		RigidBody* b2 = mBody[1];

		glm::vec3 localPost1(mLocalPost[0]);
		glm::vec3 localPost2(mLocalPost[1]);

		glm::mat3 R1(b1->mQ);
		glm::vec3 r1 = R1 * localPost1;
		glm::vec3 x1 = b1->mX + r1;

		glm::mat3 R2(b2->mQ);
		glm::vec3 r2 = R2 * localPost2;
		glm::vec3 x2 = b2->mX + r2;

		glm::vec3 X(x2 - x1);
		glm::vec3 C = X - mDistanceOffset * glm::normalize(X);

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
		// Jacobian
		std::vector<float> dCdx1({
			-1.0f,  0.0f,  0.0f,
			0.0f,  -1.0f, 0.0f,
			0.0f,  0.0f,  -1.0f
			});

		std::vector<float> dCdx2({
			1.0f,  0.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  0.0f,  1.0f
			});

		std::vector<float> skew_r1({
			0.0f,  -r1[2],  r1[1],
			r1[2],   0.0f, -r1[0],
			-r1[1],  r1[0],  0.0f
			});

		std::vector<float> skew_r2({
			0.0f,  r2[2],  -r2[1],
			-r2[2],   0.0f, r2[0],
			r2[1],  -r2[0],  0.0f
			});

		std::vector<float> J; //<3, 6>
		addBlockColumn(J, 3, 0, dCdx1, 3);
		addBlockColumn(J, 3, 3, skew_r1, 3);
		addBlockColumn(J, 3, 6, dCdx2, 3);
		addBlockColumn(J, 3, 9, skew_r2, 3);

		///////////////////////////////////////////////////////
		std::vector<float> MinvJt;
		
		auto rc = glm::sqrt(Minv.size());
		MinvJt.resize(rc * 3);
		multiply(Minv, rc, rc, false, J, 3, rc, true, MinvJt);

		std::vector<float> K;
		K.resize(3 * 3);

		multiply(J, 3, rc, false, MinvJt, rc, 3, false, K);
		/////

		std::vector<float> p;
		std::vector<float> Kinv;
		choldc(K, 3, p);
		invA(K, 3, p, Kinv);

		std::vector<float> matC({ -C[0], -C[1], -C[2] });

		std::vector<float> A;
		multiply(MinvJt, 12, 3, false, Kinv, 3, 3, false, A);
		std::vector<float> dX;
		multiply(A, 12, 3, false, matC, 1, 3, true, dX);

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