#pragma once

#include "../AllMHeaders.h"
#include "../Twister/MathLibrary/TwMat4.h"

namespace MG
{
	class TwRigidBody;

	class MRigidBodyComponent : public MComponent
	{
		std::weak_ptr<TwRigidBody> mRigidBody;

	protected:
		virtual void onCreateInternal() override;

	public:
		virtual ~MRigidBodyComponent();
		void updateTransform(const TwMat4& transform);
	};
}