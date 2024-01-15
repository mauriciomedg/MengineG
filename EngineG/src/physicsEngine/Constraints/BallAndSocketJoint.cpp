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

	void BallAndSocketJoint::execute(float dt)
	{
		if (!mBody[0]) return;

		RigidBody* b1 = mBody[0];

		glm::vec3 localPost(5.0f, 5.0f, 5.0f);

		glm::vec3 x0(-10.0f, 30.0f, 0.0f);

		glm::mat3 R(b1->mQ);
		glm::vec3 r1 = R * localPost;
		glm::vec3 x = b1->mX + r1;

		glm::vec3 X(x - x0);
		glm::vec3 C = X - 10.0f * glm::normalize(X);

		std::vector<float> Minv;
		int r = 3, c = 3;
		Minv.resize(r * c, 0.0f);
		for (int j = 0; j < c; ++j)
			Minv[j + j * c] = 1.0f / b1->mMass;

		addBlockDiagonal(Minv, r, c, b1->mIinvArray, 3, 3);
	
		/////////////////////////////
		std::vector<float> J; //<3, 6>
		r = 3, c = 3;
		J.resize(r * c, 0.0f);
		
		for (int j = 0; j < c; ++j)
			J[j + j * c] = 1.0f;

		std::vector<float> skew_r1({
			0.0f,  r1[2],  -r1[1],
			-r1[2],   0.0f, r1[0],
			r1[1],  -r1[0],  0.0f
			});

		addBlockColumn(J, r, c, skew_r1, 3);

		/////
		std::vector<float> MinvJt;
		MinvJt.resize(6 * 3);

		multiply(Minv, 6, 6, false, J, 3, 6, true, MinvJt);

		std::vector<float> K;
		K.resize(3 * 3);

		multiply(J, 3, 6, false, MinvJt, 6, 3, false, K);
		std::vector<float> copyK(K);
		/////

		std::vector<float> p;
		std::vector<float> Kinv;
		choldc(K, 3, p);
		invA(K, 3, p, Kinv);
		//////

		std::vector<float> matC({ -C[0], -C[1], -C[2] });

		std::vector<float> A;
		multiply(MinvJt, 6, 3, false, Kinv, 3, 3, false, A);
		std::vector<float> dX;
		multiply(A, 6, 3, false, matC, 1, 3, true, dX);

		/////
		b1->mX += glm::vec3(dX[0], dX[1], dX[2]);

		glm::quat Wquad(0.0f, dX[3], dX[4], dX[5]);
		glm::quat qdot = 0.5f * (Wquad * b1->mQ);
		b1->mQ += qdot;

		b1->calculateInternalData();
	}
}