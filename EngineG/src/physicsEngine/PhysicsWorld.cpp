#pragma once

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Contact.h"
#include "CollisionDetection.h"
#include "CollisionResponse.h"
#include "exp-map.h"

#include <iostream>
#include <algorithm>
#include <math.h>


namespace
{
	struct Quat
	{
		Quat(const glm::quat& quater)
		{
			q[0] = quater[1];
			q[1] = quater[2];
			q[2] = quater[3];
			q[3] = quater[0];

		}

		void getMatrix(std::vector<float>& Q)
		{
			float Qtemp[] = {
				-q[0], -q[1], -q[2],
				 q[3],  q[2], -q[1],
				-q[2],  q[3],  q[0],
				 q[1], -q[0],  q[3]
			};
			
			std::for_each(std::begin(Qtemp), std::end(Qtemp), [&](float c) { Q.push_back(c * 0.5f); });
		}

		float operator[] (int i) const
		{
			return q[i];
		}

		float q[4];
	};

	void dd1_dq(const Quat& q, std::vector<float>& Q) //< 3, 4 >
	{
		float Qtemp[] = {
			q[0], -q[1], -q[2], q[3],
			q[1], q[0], q[3], q[2],
			q[2], -q[3], q[0], -q[1] };
		
		std::for_each(std::begin(Qtemp), std::end(Qtemp), [&](float c) { Q.push_back(c * 2.0f); });
	}

	void dd2_dq(const Quat& q, std::vector<float>& Q) //< 3, 4 >
	{
		float Qtemp[] = {
			q[1], q[0], -q[3], -q[2],
			-q[0], q[1], -q[2], q[3],
			q[3], q[2], q[1], q[0] };

		std::for_each(std::begin(Qtemp), std::end(Qtemp), [&](float c) { Q.push_back(c * 2.0f); });
	}

	void dd3_dq(const Quat& q, std::vector<float>& Q) //< 3, 4 >
	{
		float Qtemp[] = {
			q[2], q[3], q[0], q[1],
			-q[3], q[2], q[1], -q[0],
			-q[0], -q[1], q[2], q[3] };

		std::for_each(std::begin(Qtemp), std::end(Qtemp), [&](float c) { Q.push_back(c * 2.0f); });
	}
	
	///// Derivative of R.p. R quaternion matrix.
	void dR_dq(const Quat q, const glm::vec3& p, std::vector<float>& Q) // <3, 4>
	{
		std::vector<float> Q0;
		std::vector<float> Q1;
		std::vector<float> Q2;
		
		dd1_dq(q, Q0);
		dd2_dq(q, Q1);
		dd3_dq(q, Q2);

		for (int i = 0; i < 12; ++i)
			Q.push_back(p[0] * Q0[i] + p[1] * Q1[i] + p[2] * Q2[i]);
		//return p[0] * dd1_dq(q, Q1) + p[1] * dd2_dq(q) + p[2] * dd3_dq(q);
	}

	void swap(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	void multiply(const std::vector<float>& a, int nra, int nca, bool atrans, const std::vector<float>& b, int nrb, int ncb, bool btrans, std::vector<float>& result)
	{
		if (atrans) swap(nra, nca);
		if (btrans) swap(nrb, ncb);
		
		result.resize(nra * ncb, 0.0f);

		for (int i = 0; i < nra; i++) {
			for (int j = 0; j < ncb; j++) {

				result[j + i * ncb] = 0;

				for (int k = 0; k < nrb; k++) {
					//result[j + i * ncb] += a[k + i * nca] * b[j + k * ncb];
					result[j + i * ncb] += (atrans ? a[i + k * nca] : a[k + i * nca]) * (btrans ? b[k + j * ncb] : b[j + k * ncb]);
				}
			}
		}
	}

	template <typename scalar_type>
	void cholesky_decompostion(const std::vector<scalar_type>& A, std::vector<scalar_type>& result)
	{
		size_t n = std::sqrt(A.size());
		result.resize(A.size(), 0.0f);

		for (size_t i = 0; i < n; ++i)
		{
			for (size_t k = 0; k < i; ++k)
			{
				scalar_type value = A[k + i * n];
				for (size_t j = 0; j < k; ++j)
					value -= result[j + i * n] * result[j + k * n];
				result[k + i * n] = value / result[k + k * n];
			}
			scalar_type value = A[i + i * n];
			for (size_t j = 0; j < i; ++j)
				value -= result[j + i * n] * result[j + i * n];
			result[i + i * n] = std::sqrt(value);
		}
	}

	template <typename scalar_type>
	void cholesky_factor(const std::vector<scalar_type>& A, const std::vector<scalar_type>& b, std::vector<scalar_type>& x)
	{
		size_t n = std::sqrt(A.size());
		std::vector<scalar_type> result;
		cholesky_decompostion(A, result);
		
		//forward
		x.resize(n);
		for (int i = 0; i < n; ++i)
		{
			scalar_type val(0.0f);
			for (int j = 0; j < i; ++j)
			{
				val += result[j + i * n] * x[j];
			}
			x[i] = (b[i] - val) / result[i + i * n];
		}

		//backward
		for (int i = n - 1; i >= 0; --i)
		{
			scalar_type val(0.0f);
			for (int j = n - 1; j > i; --j)
			{
				val += result[i + j * n] * x[j];
			}
			x[i] = (x[i] - val) / result[i + i * n];
		}
	}

	void choldc(std::vector<float>& a, int n, std::vector<float>& p)
		//Given a positive - definite symmetric matrix a[1..n][1..n], this routine constructs its Cholesky
		//decomposition, A = L · LT.On input, only the upper triangle of a need be given; it is not
		//modified.The Cholesky factor L is returned in the lower triangle of a, except for its diagonal
		//elements which are returned in p[1..n].
	{
		//void nrerror(char error_text[]);
		p.resize(n, 0.0f);

		int i, j, k;
		float sum;
		for (i = 0; i < n; i++) {
			for (j = i; j < n; j++) {
				for (sum = a[j + i * n], k = i - 1; k > 0; k--) sum -= a[k + i * n] * a[k + j *n];
				if (i == j) {
					if (sum <= 0.0) //a, with rounding errors, is not positive definite.
						std::cout << "choldc failed" << std::endl;
					p[i] = sqrt(sum);
				}
				else a[i + j * n] = sum / p[i];
			}
		}
	}

	void invL(std::vector<float>& a, int n, const std::vector<float>& p)
	{
		for (int i = 0; i < n; i++)
		{
			a[i + i * n] = 1.0 / p[i];
			for (int j = i + 1; j < n; j++) {
				float sum = 0.0;
				for (int k = i; k < j; k++) sum -= a[k + j * n] * a[i + k * n];
				a[i + j * n] = sum / p[j];
			}
		}
	}

	void invA(std::vector<float>& a, int n, const std::vector<float>& p, std::vector<float>& Ainv)
	{
		invL(a, n, p);

		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				a[j + i * n] = 0.0f;
			}
		}

		multiply(a, n, n, true, a, n, n, false, Ainv);
	}
}

FixedConstraint::FixedConstraint()
{
}

void FixedConstraint::execute(float dt)
{
	RigidBody* b1 = body[0];

	glm::vec3 localPost(1.0f, 0.0f, 0.0f);
	
	glm::vec3 x0(-10.0f, 30.0f, 0.0f);

	glm::mat3 R(b1->mQ);
	glm::vec3 x = b1->mX + R * localPost;

	glm::vec3 X = x - x0;
	float C = glm::length(X) - 5.0f;

	glm::vec3 J = glm::normalize(X);

	glm::mat3 Minv;
	Minv[0][0] = 1.0f / b1->mMass;
	Minv[1][1] = 1.0f / b1->mMass;
	Minv[2][2] = 1.0f / b1->mMass;

	glm::vec3 deltaX = (-Minv * J / (glm::dot(J, Minv * J))) * C;

	b1->mX += deltaX;


	//glm::outerProduct
	

	//glm::outerProduct()

	//glm::vec4 localPost(1.0f, 0.0f, 0.0f, 0.0f);
	//
	//RigidBody* b1 = body[0];
	//RigidBody* b2 = body[1];
	//
	//glm::vec3 p1 = b1->mWorldMat * localPost;
	//glm::vec3 p2 = b2->mWorldMat * localPost;
	//
	//glm::vec3 n = glm::normalize(p2 - p1);
	//
	//glm::vec3 r1 = b1->mX - p1;
	//glm::vec3 r2 = b2->mX - p2;
	//
	//float term1 = 1 / b1->mMass;
	//float term3 = glm::dot(n, glm::cross(b1->mIinv * glm::cross(r1, n), r1));
	//
	//float term2 = 1 / b2->mMass;
	//float term4 = glm::dot(n, glm::cross(b2->mIinv * glm::cross(r2, n), r2));
	//
	//float lambda = -10.0f / (term1 + term2 + term3 + term4);
	//
	//glm::vec3 p = lambda * n;
	//
	//if (b1)
	//{
	//	b1->mX += term1 * p * dt;
	//	
	//	auto L = glm::cross(r1, p);
	//	auto w = b1->mIinv * L;
	//
	//	glm::quat Wquad(0.0f, w[0], w[1], w[2]);
	//	glm::quat qdot = 0.5f * (Wquad * b1->mQ);
	//	b1->mQ += qdot * dt;
	//	b1->calculateInternalData();
	//}
	//
	//if (b2)
	//{
	//	b2->mX += term2 * p * dt;
	//
	//	auto L = glm::cross(r2, p);
	//	auto w = b2->mIinv * L;
	//
	//	glm::quat Wquad(0.0f, w[0], w[1], w[2]);
	//	glm::quat qdot = 0.5f * (Wquad * b2->mQ);
	//	b2->mQ -= qdot * dt;
	//	b2->calculateInternalData();
	//}
}

void FixedConstraint::executeConnectionWithOrientation(float dt)
{
	RigidBody* b1 = body[0];

	glm::vec3 localPost(1.0f, 0.0f, 0.0f);

	glm::vec3 x0(-10.0f, 30.0f, 0.0f);

	glm::mat3 R(b1->mQ);
	glm::vec3 x = b1->mX + R * localPost;

	glm::vec3 X(x - x0);
	float C = glm::length(X) - 5.0f;

	/////////////////////////////
	std::vector<float> Minv;
	int n = 6, m = 6;
	Minv.resize(n * m, 0.0f);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if (i == j && i < 3) Minv[j + i * m] = 1.0f / b1->mMass;

			if (i > 2 && j > 2) Minv[j + i * m] = b1->mIinv[i - 3][j - 3];
		}
	}

	/////////////////////////////
	Quat mQ = (b1->mQ);
	//std::vector<float> Q;
	//mQ.getMatrix(Q);
	//
	//std::vector<float> S;
	//S.resize(7 * 6, 0.0f);
	//
	//for (int i = 0; i < 7; ++i)
	//{
	//	for (int j = 0; j < 6; ++j)
	//	{
	//		if (i == j && i < 3) S[j + i * 6] = 1.0f;
	//
	//		if (i > 2 && j > 2) S[j + i * 6] = Q[j - 3 + (i - 3) * 3];
	//	}
	//}

	/////////////////////////////
	std::vector<float> dRdq;
	dR_dq(mQ, localPost, dRdq); //<3, 4>

	std::vector<float> J, Jt; //<3, 6>
	n = 3, m = 7;
	J.resize(n * m, 0.0f);
	Jt.resize(n * m, 0.0f);
	
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if (i == j && i < 3) J[j + i * m] = 1.0f;//glm::normalize(X)[i];

			if (j > 2) J[j + i * m] = dRdq[j - 3 + i * 3];

			Jt[i + j * n] = J[j + i * m];
		}
	}

	/////
	std::vector<float> MinvJt;
	MinvJt.resize(6 * 3);

	multiply(Minv, 6, 6, false, Jt, 6, 3, false, MinvJt);

	std::vector<float> K;
	K.resize(3 * 3);

	multiply(J, 3, 6, false, MinvJt, 6, 3, false, K);
	std::vector<float> copyK(K);
	/////
	// 
	// 
	//std::vector<float> J;
	//J.resize(6 * 7, 0.0f);
	//
	//for (int i = 0; i < 6; ++i)
	//{
	//	for (int j = 0; j < 7; ++j)
	//	{
	//		if (i == j && i < 3) J[j + i * 7] = X[i];
	//
	//		if (i > 2 && j > 2) J[j + i * 7] = dRdq[j - 3 + (i - 3) * 4];
	//	}
	//}
	
	//std::cout << " p " << std::endl;

	//std::for_each(J.begin(), J.end(), [&](float& v) { v *= 1 / glm::length(X); });
	//
	//
	//
	//glm::mat3 Minv;
	//Minv[0][0] = 1.0f / b1->mMass;
	//Minv[1][1] = 1.0f / b1->mMass;
	//Minv[2][2] = 1.0f / b1->mMass;
	//
	//glm::vec3 deltaX = (-Minv * J / (glm::dot(J, Minv * J))) * C;
	//
	//b1->mX += deltaX;


	//glm::outerProduct


	//glm::outerProduct()

	//glm::vec4 localPost(1.0f, 0.0f, 0.0f, 0.0f);
	//
	//RigidBody* b1 = body[0];
	//RigidBody* b2 = body[1];
	//
	//glm::vec3 p1 = b1->mWorldMat * localPost;
	//glm::vec3 p2 = b2->mWorldMat * localPost;
	//
	//glm::vec3 n = glm::normalize(p2 - p1);
	//
	//glm::vec3 r1 = b1->mX - p1;
	//glm::vec3 r2 = b2->mX - p2;
	//
	//float term1 = 1 / b1->mMass;
	//float term3 = glm::dot(n, glm::cross(b1->mIinv * glm::cross(r1, n), r1));
	//
	//float term2 = 1 / b2->mMass;
	//float term4 = glm::dot(n, glm::cross(b2->mIinv * glm::cross(r2, n), r2));
	//
	//float lambda = -10.0f / (term1 + term2 + term3 + term4);
	//
	//glm::vec3 p = lambda * n;
	//
	//if (b1)
	//{
	//	b1->mX += term1 * p * dt;
	//	
	//	auto L = glm::cross(r1, p);
	//	auto w = b1->mIinv * L;
	//
	//	glm::quat Wquad(0.0f, w[0], w[1], w[2]);
	//	glm::quat qdot = 0.5f * (Wquad * b1->mQ);
	//	b1->mQ += qdot * dt;
	//	b1->calculateInternalData();
	//}
	//
	//if (b2)
	//{
	//	b2->mX += term2 * p * dt;
	//
	//	auto L = glm::cross(r2, p);
	//	auto w = b2->mIinv * L;
	//
	//	glm::quat Wquad(0.0f, w[0], w[1], w[2]);
	//	glm::quat qdot = 0.5f * (Wquad * b2->mQ);
	//	b2->mQ -= qdot * dt;
	//	b2->calculateInternalData();
	//}
}

void FixedConstraint::executeConnectionWithExponential(float dt)
{
	RigidBody* b1 = body[0];

	glm::vec3 localPost(1.0f, 1.0f, 1.0f);

	glm::vec3 x0(-10.0f, 30.0f, 0.0f);

	glm::mat3 R(b1->mQ);
	glm::vec3 r1 = R * localPost;
	glm::vec3 x = b1->mX + r1;

	glm::vec3 X(x - x0);
	glm::vec3 C = X - 10.0f * glm::normalize(X);
	
	std::vector<float> Minv;
	int n = 6, m = 6;
	Minv.resize(n * m, 0.0f);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if (i == j && i < 3) Minv[j + i * m] = 1.0f / b1->mMass;

			if (i > 2 && j > 2) Minv[j + i * m] = b1->mIinv[i - 3][j - 3];
		}
	}

	/////////////////////////////
	std::vector<float> J, Jt; //<3, 6>
	n = 3, m = 6;
	J.resize(n * m, 0.0f);
	Jt.resize(n * m, 0.0f);

	std::vector<float> skew_r1({
		0.0f,  r1[2],  -r1[1],
		-r1[2],   0.0f, r1[0],
		r1[1],  -r1[0],  0.0f
		});

	
	//multiply(a, 3, 3, b, 3, 2, result);
	//multiply(a, 2, 3, b, 3, 2, result);
	//multiply(a, 2, 3, b, 3, 4, result);

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if (i == j && i < 3) J[j + i * m] = 1.0f;//glm::normalize(X)[i];

			if (j > 2) J[j + i * m] = skew_r1[j - 3 + i * 3];

			Jt[i + j * n] = J[j + i * m];
		}
	}

	/////
	std::vector<float> MinvJt;
	MinvJt.resize(6 * 3);

	multiply(Minv, 6, 6, false, Jt, 6, 3, false, MinvJt);

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

	std::vector<float> matC({-C[0], -C[1], -C[2]});
	
	std::vector<float> A;
	multiply(MinvJt, 6, 3, false, Kinv, 3, 3, false, A);
	std::vector<float> dX;
	multiply(A, 6, 3, false, matC, 1, 3, true, dX);

	//std::vector<float> resultTest;
	//multiply(Kinv, n, n, false, copyK, n, n, false, resultTest);

	//cholesky_decompostion(K, Kinv);

	//glm::vec3 V = b1->mW * dt;
	//double dRdvi[4][4];
	//double dV[] = { V[0], V[1], V[2] };
	//
	//int r = Partial_R_Partial_EM3(dV, 0, dRdvi);
	//
	//
	//
	/////
	b1->mX += glm::vec3(dX[0], dX[1], dX[2]);

	glm::quat Wquad(0.0f, dX[3], dX[4], dX[5]);
	glm::quat qdot = 0.5f * (Wquad * b1->mQ);

	b1->mQ += qdot;

	b1->calculateInternalData();

	//std::cout << "h" << std::endl;

	//std::for_each(J.begin(), J.end(), [&](float& v) { v *= 1 / glm::length(X); });
	//
	//
	//
	//glm::mat3 Minv;
	//Minv[0][0] = 1.0f / b1->mMass;
	//Minv[1][1] = 1.0f / b1->mMass;
	//Minv[2][2] = 1.0f / b1->mMass;
	//
	//glm::vec3 deltaX = (-Minv * J / (glm::dot(J, Minv * J))) * C;
	//
	//b1->mX += deltaX;


	//glm::outerProduct


	//glm::outerProduct()

	//glm::vec4 localPost(1.0f, 0.0f, 0.0f, 0.0f);
	//
	//RigidBody* b1 = body[0];
	//RigidBody* b2 = body[1];
	//
	//glm::vec3 p1 = b1->mWorldMat * localPost;
	//glm::vec3 p2 = b2->mWorldMat * localPost;
	//
	//glm::vec3 n = glm::normalize(p2 - p1);
	//
	//glm::vec3 r1 = b1->mX - p1;
	//glm::vec3 r2 = b2->mX - p2;
	//
	//float term1 = 1 / b1->mMass;
	//float term3 = glm::dot(n, glm::cross(b1->mIinv * glm::cross(r1, n), r1));
	//
	//float term2 = 1 / b2->mMass;
	//float term4 = glm::dot(n, glm::cross(b2->mIinv * glm::cross(r2, n), r2));
	//
	//float lambda = -10.0f / (term1 + term2 + term3 + term4);
	//
	//glm::vec3 p = lambda * n;
	//
	//if (b1)
	//{
	//	b1->mX += term1 * p * dt;
	//	
	//	auto L = glm::cross(r1, p);
	//	auto w = b1->mIinv * L;
	//
	//	glm::quat Wquad(0.0f, w[0], w[1], w[2]);
	//	glm::quat qdot = 0.5f * (Wquad * b1->mQ);
	//	b1->mQ += qdot * dt;
	//	b1->calculateInternalData();
	//}
	//
	//if (b2)
	//{
	//	b2->mX += term2 * p * dt;
	//
	//	auto L = glm::cross(r2, p);
	//	auto w = b2->mIinv * L;
	//
	//	glm::quat Wquad(0.0f, w[0], w[1], w[2]);
	//	glm::quat qdot = 0.5f * (Wquad * b2->mQ);
	//	b2->mQ -= qdot * dt;
	//	b2->calculateInternalData();
	//}
}

int PhysicsWorld::instanciatePrimitive(const glm::mat4& transform, bool isSimulatingPhysics)
{
	return -1;
}

int PhysicsWorld::instanciatePrimitiveBox(const glm::mat4& transform, glm::vec3& halfSize, bool isSimulatingPhysics)
{
	CollisionBox* box = new CollisionBox;
	box->halfSize = halfSize;
	mPrimitive.push_back(box);
	mPrimitive.back()->body = (isSimulatingPhysics ? new RigidBody() : nullptr);
	if (isSimulatingPhysics) 
	{
		mPrimitive.back()->body->init(transform, halfSize);
		mPrimitive.back()->calculateInternals();
	}
	mPrimitive.back()->calculateInternals(transform);
	mPrimitive.back()->mSimulatePhysics = isSimulatingPhysics;
	return mPrimitive.size() - 1;
}

int PhysicsWorld::instanciatePrimitivePlane(const glm::mat4& transform, bool isSimulatingPhysics)
{
	CollisionPlane* plane = new CollisionPlane;
	plane->direction = transform[1];
	mPrimitive.push_back(plane);
	mPrimitive.back()->body = nullptr;
	mPrimitive.back()->calculateInternals(transform);
	mPrimitive.back()->mSimulatePhysics = isSimulatingPhysics;
	return mPrimitive.size() - 1;
}

void PhysicsWorld::generateContacts()
{
	mCollisionDetection->cData.reset(MAX_CONTACTS);
	for (int i = 0; i < mPrimitive.size(); ++i)
	{
		CollisionBox* box = dynamic_cast<CollisionBox*>(mPrimitive[i]);

		for (int j = i + 1; j < mPrimitive.size(); ++j)
		{
			if (box && !box->mIgnoreCollision)
			{
				CollisionBox* box2 = dynamic_cast<CollisionBox*>(mPrimitive[j]);

				if (box2 && !box2->mIgnoreCollision)
				{
					if (glm::length(box->getAxis(3) - box2->getAxis(3)) <= (glm::length(box->halfSize) + glm::length(box2->halfSize)))
					{
						mCollisionDetection->cData.friction = -1.0f;
						mCollisionDetection->cData.restitution = 0.0f;
						CollisionDetector::boxAndBox(*box, *box2, &mCollisionDetection->cData);
					}
				}

				CollisionPlane* plane = dynamic_cast<CollisionPlane*>(mPrimitive[j]);

				if (plane)
				{
					mCollisionDetection->cData.friction = 0.0f;
					mCollisionDetection->cData.restitution = 0.0f;
					CollisionDetector::boxAndHalfSpace(*box, *plane, &mCollisionDetection->cData);
				}
			}
		}
	}
}

void PhysicsWorld::addMovement(int id, glm::vec3& intensity, float scale)
{
	mPrimitive[id]->body->addMovement(intensity, scale);
}

void PhysicsWorld::move(int id, glm::vec3& velocity)
{
	if (mPrimitive[id]->body)
	{
		mPrimitive[id]->body->move(velocity * mDeltaT);
	}
	else
	{
		const glm::mat4& currentTransform = mPrimitive[id]->getTransform();
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(currentTransform[3]) + velocity * mDeltaT);
		mPrimitive[id]->calculateInternals(translation);
	}
}

void PhysicsWorld::setAffectedByGravity(int id, bool isAffetedByGravity)
{
	if (mPrimitive[id]->body)
	{
		mPrimitive[id]->body->mIsAffectedByGravity = isAffetedByGravity;
	}
}

void PhysicsWorld::setIgnoreCollision(int id, bool ignore)
{
	mPrimitive[id]->mIgnoreCollision = ignore;
}

//void PhysicsWorld::runSimulation(float deltaT)
//{
//	int nbSuperSample = 3;
//
//	deltaT = deltaT / nbSuperSample;
//	mDeltaT = deltaT;
//
//	for (int sample = 0; sample < nbSuperSample; ++sample)
//	{
//		int indexBody = 0;
//		for (int i = 0; i < mPrimitive.size(); i++)
//		{
//			if (mPrimitive[i]->mSimulatePhysics)
//			{
//				RigidBody* bodies = mPrimitive[i]->body;
//				bodies->prepareSystem(&mY[0 + RigidBody::STATE_SIZE * indexBody],
//					&mYdot[0 + RigidBody::STATE_SIZE * indexBody],
//					deltaT,
//					mGravity);
//				++indexBody;
//			}
//		}
//
//		if (mYdot.size() > 0)
//		{
//			ode(&mY[0], &mYdot[0], deltaT, mYdot.size());
//		}
//		
//		indexBody = 0;
//		for (int i = 0; i < mPrimitive.size(); i++)
//		{
//			if (mPrimitive[i]->mSimulatePhysics)
//			{
//				RigidBody* bodies = mPrimitive[i]->body;
//				bodies->update(&mY[0 + RigidBody::STATE_SIZE * indexBody]);
//				mPrimitive[i]->calculateInternals();
//				++indexBody;
//			}
//		}
//
//		generateContacts();
//		mCollisionResponse->update(&mCollisionDetection->cData, mPrimitive);
//	}
//}

void PhysicsWorld::simulating(float deltaT)
{
	int nbSuperSample = 3;

	deltaT = deltaT / nbSuperSample;
	mDeltaT = deltaT;

	///////
	FixedConstraint c;
	int index = 0;
	c.body[0] = mPrimitive[2]->body;
	//for (int i = 0; i < mPrimitive.size(); i++)
	//{
	//	CollisionBox* box = dynamic_cast<CollisionBox*>(mPrimitive[i]);
	//
	//	if (box && !box->mIgnoreCollision)
	//	{
	//		if (box->body)
	//		{
	//			c.body[index] = box->body;
	//			index++;
	//		}
	//	}
	//}
	///////

	for (int sample = 0; sample < nbSuperSample; ++sample)
	{
		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->computeForceAndTorque(deltaT, mGravity);
				bodies->update(deltaT);
				mPrimitive[i]->calculateInternals();
			}
		}

		generateContacts();

		CollisionData* cData = &(mCollisionDetection)->cData;
		for (int j = 0; j < cData->contactCount; j++)
		{
			Contact* c = &(cData->contactArray[j]);
			c->lambda = 0.0f;
		}

		for (int posItt = 0; posItt < 5; ++posItt)
		{
			mCollisionResponse->solvePositions(&mCollisionDetection->cData, deltaT);
			//c.execute(deltaT);
			//c.executeConnectionWithOrientation(deltaT);
			c.executeConnectionWithExponential(deltaT);
		}
			
		mCollisionResponse->update(&mCollisionDetection->cData, mPrimitive);

		for (int i = 0; i < mPrimitive.size(); i++)
		{
			if (mPrimitive[i]->mSimulatePhysics)
			{
				RigidBody* bodies = mPrimitive[i]->body;
				bodies->update2(deltaT);
			}
		}
	
		//mCollisionResponse->update(&mCollisionDetection->cData, mPrimitive);
	}
}

void PhysicsWorld::init()
{
	mGravity = glm::vec3(0.0f, -90.0f, 0.0f);

	int nbSimulatedPhysicsPrimitives = 0;
	for (int i = 0; i < mPrimitive.size(); i++)
	{
		RigidBody* bodies = mPrimitive[i]->body;
		
		if (mPrimitive[i]->mSimulatePhysics)
		{
			++nbSimulatedPhysicsPrimitives;
		}
	}

	mY.resize(RigidBody::STATE_SIZE * nbSimulatedPhysicsPrimitives);
	mYposPrev.resize(RigidBody::POSITION_BASE_STATE_SIZE * nbSimulatedPhysicsPrimitives);
	mYdot.resize(RigidBody::STATE_SIZE * nbSimulatedPhysicsPrimitives);

	mContacts = new Contact[MAX_CONTACTS];
	mCollisionDetection = new CollisionDetection;
	mCollisionDetection->cData.contactArray = mContacts;
	mCollisionDetection->cData.contactsLeft = MAX_CONTACTS;
	mCollisionResponse = new CollisionResponse;
}

const glm::mat4& PhysicsWorld::getPrimitiveLocation(int id) const
{
	return mPrimitive[id]->getTransform();
}
