#include "RigidPointConstraint.h"
#include "../RigidBody.h"
#include "../MathUtils.h"

#include <vector>
#include <algorithm>
#include <iterator>

namespace MG
{
	RigidPointConstraint::RigidPointConstraint(RigidBody* b)
	{
		mBody = b;
	}

	RigidPointConstraint::RigidPointConstraint()
	{
		mBody = nullptr;
	}

	void RigidPointConstraint::prepare(std::vector<float>& J, std::vector<float>& C)
	{
		//if (!mBody) return;
		//
		//RigidBody* b = mBody;
		//
		//glm::vec3 localPost(5.0f, 5.0f, 5.0f);
		//
		//glm::vec3 x0(-10.0f, 50.0f, 0.0f);
		//
		//glm::mat3 R(b->mQ);
		//glm::vec3 r = R * localPost;
		//glm::vec3 x = b->mX + r;
		//
		//glm::vec3 X(x - x0);
		//glm::vec3 C = X - 10.0f * glm::normalize(X);
		//
		//std::vector<float> Minv;
		//
		//Minv.resize(3 * 3, 0.0f);
		//for (int j = 0; j < 3; ++j)
		//	Minv[j + j * 3] = 1.0f / b->mMass;
		//
		//addBlockDiagonal(Minv, 3, 3, b->mIinvArray, 3, 3);
		//
		///////////////////////////////
		//std::vector<float> Jtemp; //<3, 6>
		//Jtemp.resize(3 * 3, 0.0f);
		//
		//for (int j = 0; j < 3; ++j)
		//	Jtemp[j + j * 3] = 1.0f;
		//
		//std::vector<float> skew_r1({
		//	0.0f,  r[2],  -r[1],
		//	-r[2],   0.0f, r[0],
		//	r[1],  -r[0],  0.0f
		//	});
		//
		//addBlockColumn(Jtemp, 3, 3, skew_r1, 3);
		//
		//if(J.empty())
		//{
		//	std::copy(Jtemp.begin(), Jtemp.end(), std::back_inserter(J));
		//}
		//else
		//{
		//	addBlockRow(J, 3, 6, Jtemp, 3);
		//}
		//
		//C.push_back(-C[0]);
		//C.push_back(-C[1]);
		//C.push_back(-C[2]);
	}

	void RigidPointConstraint::prepareData(RigidBody* b, glm::vec3& C, std::vector<float>& skew_r)
	{
				
		int r = glm::sqrt(mMinv.size());
		int c = r;

		addBlockDiagonal(mMinv, r, c, b->mInvInertiaArray, 6, 6);

		std::vector<float> dCdx({
			1.0f,  0.0f,  0.0f,
			0.0f,   1.0f, 0.0f,
			0.0f,  0.0f,  1.0f
			});

		r = 3;// mJ.size() > 0 ? 3 : 0;
		c = r > 0 ? mJ.size() / r : 0;
		addBlockColumn(mJ, r, c, dCdx, 3);

		r = 3;
		c = mJ.size() / r;
		addBlockColumn(mJ, r, c, skew_r, 3);
	}

	void RigidPointConstraint::execute(float dt)
	{
		if (!mBody) return;

		RigidBody* b = mBody;

		//glm::vec3 localPost(5.0f, 5.0f, 5.0f);
		glm::vec3 localPost(3.0f, 3.0f, 3.0f);

		glm::vec3 x0(-10.0f, 30.0f, 0.0f);

		glm::mat3 R(b->mQ);
		glm::vec3 r = R * localPost;
		glm::vec3 x = b->mX + r;

		glm::vec3 X(x0 - x);
		glm::vec3 C = X - 10.0f * glm::normalize(X);

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

		//b->calculateInternalData();

		//if (!mBody) return;
		//
		//mMinv.clear();
		//mJ.clear();
		//
		//RigidBody* b = mBody;
		//glm::vec3 localPost(5.0f, 5.0f, 5.0f);
		//
		//glm::vec3 x0(-10.0f, 30.0f, 0.0f);
		//float d0 = 10.0f;
		//
		//glm::mat3 R(b->mQ);
		//glm::vec3 r = R * localPost;
		//glm::vec3 x = b->mX + r;
		//
		//glm::vec3 X(x - x0);
		//glm::vec3 C = X - d0 * glm::normalize(X);
		//
		//std::vector<float> skew_r({
		//	0.0f,  r[2],  -r[1],
		//	-r[2],   0.0f, r[0],
		//	r[1],  -r[0],  0.0f
		//	});
		//
		//prepareData(b, C, skew_r);
		//
		///////////////////////////////////////////////////////////////
		//std::vector<float> MinvJt;
		//
		//auto rc = glm::sqrt(mMinv.size());
		//MinvJt.resize(rc * 3);
		//multiply(mMinv, rc, rc, false, mJ, 3, rc, true, MinvJt);
		//
		//std::vector<float> K;
		//K.resize(3 * 3);
		//
		//multiply(mJ, 3, rc, false, MinvJt, rc, 3, false, K);
		//std::vector<float> copyK(K);
		///////
		//
		//std::vector<float> p;
		//std::vector<float> Kinv;
		//choldc(K, 3, p);
		//invA(K, 3, p, Kinv);
		////////
		//
		//std::vector<float> matC({ -C[0], -C[1], -C[2] });
		//
		//std::vector<float> A;
		//multiply(MinvJt, 6, 3, false, Kinv, 3, 3, false, A);
		//std::vector<float> dX;
		//multiply(A, 6, 3, false, matC, 1, 3, true, dX);
		//
		////////////////////////////////////////////////////////////////
		//
		//b->mX += glm::vec3(dX[0], dX[1], dX[2]);
		//
		//glm::quat Wquad(0.0f, dX[3], dX[4], dX[5]);
		//glm::quat qdot = 0.5f * (Wquad * b->mQ);
		//b->mQ += qdot;
		//
		//b->calculateInternalData();
		
	}
}