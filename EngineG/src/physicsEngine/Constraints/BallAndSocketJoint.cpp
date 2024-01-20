#include "BallAndSocketJoint.h"
#include "../RigidBody.h"
#include "../MathUtils.h"

#include <vector>

namespace MG
{
	BallAndSocketJoint::BallAndSocketJoint(RigidBody* b1, RigidBody* b2)
	{
		mBody[0] = b1;
		mBody[1] = b2;
	}

	BallAndSocketJoint::BallAndSocketJoint()
	{
		mBody[0] = nullptr;
		mBody[1] = nullptr;
	}

	void BallAndSocketJoint::prepareData(RigidBody* b, glm::vec3& C, std::vector<float>& skew_r)
	{
				
		//int r = glm::sqrt(mMinv.size());
		//int c = r;
		//
		//addBlockDiagonal(mMinv, r, c, b->mInvInertiaArray, 6, 6);
		//
		//std::vector<float> dCdx({
		//	1.0f,  0.0f,  0.0f,
		//	0.0f,   1.0f, 0.0f,
		//	0.0f,  0.0f,  1.0f
		//	});
		//
		//r = 3;// mJ.size() > 0 ? 3 : 0;
		//c = r > 0 ? mJ.size() / r : 0;
		//addBlockColumn(mJ, r, c, dCdx, 3);
		//
		//r = 3;
		//c = mJ.size() / r;
		//addBlockColumn(mJ, r, c, skew_r, 3);
	}

	void BallAndSocketJoint::execute(float dt)
	{
		if (!mBody[0] || !mBody[1]) return;

		//mMinv.clear();
		//mJ.clear();

		RigidBody* b1 = mBody[0];
		RigidBody* b2 = mBody[1];

		glm::vec3 localPost1(-5.0f, -5.0f, -5.0f);
		glm::vec3 localPost2(3.0f, 3.0f, 3.0f);
		
		float d0 = 10.0f;

		glm::mat3 R1(b1->mQ);
		glm::vec3 r1 = R1 * localPost1;
		glm::vec3 x1 = b1->mX + r1;

		glm::mat3 R2(b2->mQ);
		glm::vec3 r2 = R2 * localPost2;
		glm::vec3 x2 = b2->mX + r2;

		glm::vec3 X(x2 - x1);
		glm::vec3 C = X - d0 * glm::normalize(X);

		// mass
		std::vector<float> Minv;
		Minv.resize(3 * 3, 0.0f);
		for (int j = 0; j < 3; ++j)
			Minv[j + j * 3] = 1.0f / b1->mMass;

		addBlockDiagonal(Minv, 3, 3, b1->mIinvArray, 3, 3);
		//
		std::vector<float> Minv2;
		Minv2.resize(3 * 3, 0.0f);
		for (int j = 0; j < 3; ++j)
			Minv2[j + j * 3] = 1.0f / b2->mMass;

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
		
		{
			glm::vec3 n(glm::normalize(X));
						
			float w1 = glm::dot(n, b1->mIinv * n);
			float w2 = glm::dot(n, b2->mIinv * n);

			float w = w1 + w2;

			float cef1 = 1.0f;// w1 / w;
			float cef2 = 1.0f;// w2 / w;

			glm::quat Wquad(0.0f, dX[3] * cef1, dX[4] * cef1, dX[5] * cef1);
			glm::quat qdot = 0.5f * (Wquad * b1->mQ);
			b1->mQ += qdot;

			glm::quat Wquad2(0.0f, dX[9] * cef2, dX[10] * cef2, dX[11] * cef2);
			glm::quat qdot2 = 0.5f * (Wquad2 * b2->mQ);
			b2->mQ += qdot2;
		}

		float w1 = 1 / b1->mMass;
		float w2 = 1 / b2->mMass;
		float w = w1 + w2;

		float cef1 = 1.0f;//w1 / w;
		float cef2 = 1.0f;//w2 / w;

		b1->mX += glm::vec3(dX[0], dX[1], dX[2]) * cef1;
		//b1->calculateInternalData();

		b2->mX += glm::vec3(dX[6], dX[7], dX[8]) * cef2;
		//b2->calculateInternalData();
	}
}