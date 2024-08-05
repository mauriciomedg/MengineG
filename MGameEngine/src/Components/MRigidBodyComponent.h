#pragma once

#include "../AllMHeaders.h"
#include "../Twister/MathLibrary/TwMat4.h"
#include "../EventSystem.h" // temporal

namespace MG
{
	class TwRigidBody;

	class MRigidBodyComponent : public MComponent
	{
		
	protected:
		virtual void onCreateInternal() override;

	public:
		void updateShape(dataSphape data);
		virtual ~MRigidBodyComponent();
		void updateTransform(const TwMat4& transform);
	};
}