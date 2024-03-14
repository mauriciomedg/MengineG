#pragma once
#include "Constraint.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct RigidBody;
class Contact;

namespace MG
{
	class ContactConstraint : public Constraint
	{
	public:
		ContactConstraint(Contact* c);

		virtual void execute(float dt);
		virtual void update2(float dt);

		RigidBody* mBody[2];
		float mDepth;
	private:

		void computeMassInverse(std::vector<float>& mInv);
		
		void computeDeltaX(const std::vector<float>& J,
			const std::vector<float>& Minv,
			const std::vector<float>& C,
			std::vector<float>& Kinv,
			std::vector<float>& dX);

		void updatePosition(const std::vector<float>& dX);

	private:

		glm::vec3 mLocalContactPoint[2];
		glm::vec3 mContactPoint;
		//glm::vec3 mDr[2];
		glm::vec3 mN;
		float mTotalCorrection;
		glm::vec3 mV[2];
		glm::vec3 mXprev[2];
		glm::quat mQprev[2];

		float mLambdaN = 0.0f;
		float mLambdaT = 0.0f;
	};
}