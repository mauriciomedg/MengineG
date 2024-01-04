#include "CollisionResponse.h"
#include <glm/gtc/type_ptr.hpp>
#include "RigidBody.h"
#include "Contact.h"
#include "CollisionDetection.h"
#include <iostream>

namespace
{
	template <typename scalar_type>
	void cholesky_factor(const std::vector<scalar_type>& A, const std::vector<scalar_type>& b, std::vector<scalar_type>& x)
	{
		size_t n = std::sqrt(A.size());
		std::vector<scalar_type> result(A.size(), 0.0f);

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

	void velocity(RigidBody* b, const glm::vec3& P, glm::vec3& v)
	{
		if (b)
		{
			v = b->mV + glm::cross(b->mW, P - b->mX);
		}
	}

	glm::vec3 desiredVelocity(Contact* contact)
	{
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		glm::vec3 v1(0.0f);
		glm::vec3 v2(0.0f);
		velocity(b1, contact->contactPoint, v1);
		velocity(b2, contact->contactPoint, v2);

		glm::vec3 v = v1 - v2;
		return v;
	}

	glm::vec3 applyImpulse(Contact* contact, glm::vec3& N, float epsilon)
	{
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		glm::vec3 localPoint1 = b1 ? contact->contactPoint - b1->mX : glm::vec3(0.0f);
		glm::vec3 localPoint2 = b2 ? contact->contactPoint - b2->mX : glm::vec3(0.0f);

		float numerator = -(1 + epsilon) * glm::dot(desiredVelocity(contact), N);
		float term1 = b1 ? 1 / b1->mMass : 0.0f;
		float term2 = b2 ? 1 / b2->mMass : 0.0f;
		float term3 = b1 ? glm::dot(N, glm::cross(b1->mIinv * glm::cross(localPoint1, N), localPoint1)) : 0.0f;
		float term4 = b2 ? glm::dot(N, glm::cross(b2->mIinv * glm::cross(localPoint2, N), localPoint2)) : 0.0f;

		float term = term1 + term2 + term3 + term4;
		float j = term > 0.0f ? numerator / term : 0.0f;
		glm::vec3 force = j * N;

		return force;
	}

	void resolveFrictionContact(Contact* contact)
	{
		glm::vec3& N = contact->contactNormal;
		auto force = applyImpulse(contact, N, contact->restitution);
		RigidBody* b1 = contact->body[0];
		RigidBody* b2 = contact->body[1];

		if (b1)
		{
			b1->mP += force;
			b1->mL += glm::cross(contact->contactPoint - b1->mX, force);

			b1->mV = b1->mP / b1->mMass;
			b1->mW = b1->mIinv * b1->mL;
		}

		if (b2)
		{
			b2->mP -= force;
			b2->mL -= glm::cross(contact->contactPoint - b2->mX, force);

			b2->mV = b2->mP / b2->mMass;
			b2->mW = b2->mIinv * b2->mL;
		}

		if (contact->friction >= 0.0f)
		{
			glm::vec3 v = desiredVelocity(contact);
			glm::vec3 velToKill = (v - glm::dot(v, N) * N);

			if (glm::length(velToKill) > 0)
			{
				glm::vec3 Nplanar = -glm::normalize(velToKill);
				glm::vec3 forcePlanar = applyImpulse(contact, Nplanar, contact->friction);
				
				if (b1)
				{
					b1->mP += forcePlanar;
					b1->mL += glm::cross(contact->contactPoint - b1->mX, forcePlanar);

					b1->mV = b1->mP / b1->mMass;
					b1->mW = b1->mIinv * b1->mL;
				}

				if (b2)
				{
					b2->mP -= forcePlanar;
					b2->mL -= glm::cross(contact->contactPoint - b2->mX, forcePlanar);

					b2->mV = b2->mP / b2->mMass;
					b2->mW = b2->mIinv * b2->mL;
				}
			}
		}
	}

	//////////////////////
	int colliding(Contact* contact)
	{
		glm::vec3 v = desiredVelocity(contact);

		float velRel = glm::dot(v, contact->contactNormal);
		
		float THRESHOLD = 2.0;
		if (velRel > THRESHOLD)
			return 0; // moving away
		else if (velRel > -THRESHOLD)
			return 1; // resting contact
		else
			return 2; // contact
	}

	//////////////////////

	//template <typename scalar_type>
	//void cholesky_factor(const std::vector<scalar_type>& input, std::vector<scalar_type>& result)
	//{
	//	size_t n = input.size() / 2;
	//	result.resize(input.size());
	//
	//	for (size_t i = 0; i < n; ++i) {
	//		for (size_t k = 0; k < i; ++k) {
	//			scalar_type value = input[k + i * n];//(i, k);
	//			for (size_t j = 0; j < k; ++j)
	//				value -= result[j + i * n] * result[j + k * n];
	//			result[k + i * n] = value / result[k + k * n];
	//		}
	//		scalar_type value = input[i + i * n];
	//		for (size_t j = 0; j < i; ++j)
	//			value -= result[j + i * n] * result[j + i * n];
	//		result[i + i * n] = std::sqrt(value);
	//	}
	//}

	static float THRESHOLD_ACC = 1.0f;

	void compute_b(std::vector<Contact*>& restingContacts, int i, std::vector<float>& b)
	{
		Contact* c_i = restingContacts[i];
		RigidBody* rigid = c_i->body[0];

		glm::vec3 rA = c_i->contactPoint - rigid->mX;
		glm::vec3 ni = c_i->contactNormal;
		
		glm::vec3 term3 = glm::cross(rigid->mW, glm::cross(rigid->mW, rA));
		glm::vec3 term4 = glm::cross(rigid->mL, glm::cross(rigid->mW, rA)) * rigid->mIinv;

		b[i] = glm::dot(term3 + term4, ni);
	}

	void computeA(std::vector<Contact*>& restingContacts, int i, int j, std::vector<float>& A)
	{
		Contact* c_i = restingContacts[i];
		Contact* c_j = restingContacts[j];

		RigidBody* b = c_i->body[0];
		glm::vec3 rA = c_i->contactPoint - b->mX;

		glm::vec3 nj = c_j->contactNormal;
		glm::vec3 pj = c_j->contactPoint;

		float term1 = 1.0f / b->mMass;
		float term3 = glm::dot(nj, b->mIinv * glm::cross(glm::cross((pj - b->mX), nj), rA));
		float term = term1 + term3;
		
		A[j + restingContacts.size() * i] = term3;
	}
	
	void fdirection(std::vector<float>& A, std::vector<Contact*>& restingContacts, int d, const std::vector<int>& U, std::vector<float>& Df)
	{
		Df[d] = 1.0f;
		std::vector<float> Acc;
		std::vector<float> Acd;
		
		int n = restingContacts.size();

		int row = 0;
		for (int i = 0; i < A.size(); i++)
		{
			int column = i % n;
			if (column == n - 1)
				row++;

			if (A[i] > -THRESHOLD_ACC && A[i] < THRESHOLD_ACC)
			{
				if (column == d)
				{
					Acd.push_back(A[i]);
				}
			}
		}

		Acc.resize(U.size() * U.size());
		for (int i = 0; i < U.size(); i++)
		{
			for (int j = 0; j < U.size(); j++)
			{
				Acc[j + U.size() * i] = A[U[j] + restingContacts.size() * U[i]];
			}
		}

		std::vector<float> x;
		cholesky_factor(Acc, Acd, x);
		for (int j = 0; j < x.size(); j++)
		{
			Df[U[j]] = x[j];
		}
	}

	void maxstep(std::vector<float>& f, std::vector<float>& a)
	{

	}

	void drive_to_zero(std::vector<float>& A, std::vector<float>& a, std::vector<Contact*>& restingContacts, int d)
	{
		std::vector<float> Df(restingContacts.size(), 0.0f);
		std::vector<float> Da(restingContacts.size(), 0.0f);

		std::vector<int> U;

		for (int i = 0; i < restingContacts.size(); i++)
		{
			if (A[i] > -THRESHOLD_ACC && A[i] < THRESHOLD_ACC)
			{
				U.push_back(i);
			}
		}

		fdirection(A, restingContacts, d, U, Df);

		for (int i = 0; i < restingContacts.size(); i++)
		{
			for (int j = 0; j < restingContacts.size(); j++)
			{
				Da[i] += A[j + restingContacts.size() * i] * Df[j];
			}
		}

	}

	void restingContactsHandled(std::vector<Contact*>& restingContacts)
	{
		std::vector<float> A;
		std::vector<float> b;
		std::vector<float> f;
		A.resize(restingContacts.size() * restingContacts.size());
		b.resize(restingContacts.size());
		f.resize(restingContacts.size());

		for (int i = 0; i < restingContacts.size(); i++)
		{
			compute_b(restingContacts, i, b);
			for (int j = 0; j < restingContacts.size(); j++)
			{
				computeA(restingContacts, i, j, A);
			}
		}

		std::vector<float> a = b;
		//bool terminate = false;
		//while ()
		{
			for (int i = 0; i < restingContacts.size(); i++)
			{
				if (A[i] <= -THRESHOLD_ACC)
				{
					drive_to_zero(A, a, restingContacts, i);
					break;
				}
			}
		};
	}
	//////////////////////
}

CollisionResponse::CollisionResponse()
{
	//mContacts = new Contact[MaxContacts];
	//mContactItt = mContacts;
}

void CollisionResponse::update(CollisionData* cData, std::vector<CollisionPrimitive*>& primitives)
{
	//collision detection
	//generateContacts(bodies);
	if (cData->contactCount > 0)
	{		
		std::vector<Contact*> restingContacts;
		for (int j = 0; j < cData->contactCount; j++)
		{
			Contact* c = &(cData->contactArray[j]);

			int typeOfC = colliding(c);

			if (typeOfC == 1)
				restingContacts.push_back(c);
			else
				resolveFrictionContact(c);
		}
				
		//restingContactsHandled(restingContacts);
	}
}
