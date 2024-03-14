#include "RigidPointConstraint.h"
#include "../RigidBody.h"
#include "../MathUtils.h"

#include <vector>
#include <algorithm>
#include <iterator>

namespace MG
{
	RigidPointConstraint::RigidPointConstraint(RigidBody* b, const glm::vec3& localPost, const glm::vec3& position, float distanceOffset)
		: mBody(b), mLocalPost(localPost), mPosition(position), mDistanceOffset(distanceOffset)
	{
	}

	void RigidPointConstraint::execute(float dt)
	{
		if (!mBody) return;

		RigidBody* b = mBody;

		glm::mat3 R(b->mQ);
		glm::vec3 r = R * mLocalPost;
		glm::vec3 x = b->mX + r;

		glm::vec3 X(mPosition - x);
		glm::vec3 C = X - mDistanceOffset * glm::normalize(X);

		std::vector<float> Minv;

		Minv.resize(3 * 3, 0.0f);
		for (int j = 0; j < 3; ++j)
			Minv[j + j * 3] = b->mShape->getMassInv();

		addBlockDiagonal(Minv, 3, 3, b->mIinvArray, 3, 3);

		/////////////////////////////
		std::vector<float> J; //<3, 6>
		J.resize(3 * 3, 0.0f);

		for (int j = 0; j < 3; ++j)
			J[j + j * 3] = -1.0f;

		std::vector<float> skew_r1({
			0.0f,  -r[2],  r[1],
			r[2],   0.0f, -r[0],
			-r[1],  r[0],  0.0f
			});

		addBlockColumn(J, 3, 3, skew_r1, 3);

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
		b->mX += glm::vec3(dX[0], dX[1], dX[2]);

		glm::quat Wquad(0.0f, dX[3], dX[4], dX[5]);
		glm::quat qdot = 0.5f * (Wquad * b->mQ);
		b->mQ += qdot;
	}
}