#pragma once

#ifndef MRIGIDBODYCOMPONENT_H
#define MRIGIDBODYCOMPONENT_H

#include "../AllMHeaders.h"
#include "../MathLibrary/TwMat4.h"
#include "../MathLibrary/TwVec3.h"

#include "../EventSystem.h" // temporal, we need it because of the dataShape

namespace MG
{
	class MRigidBodyComponent : public MComponent
	{
		
	protected:
		virtual void onCreateInternal() override;

	public:
		void updateShape(dataSphape data);
		void updateTransform(const TwMat4& transform);

		float getMass() const { return m_mass; }
		const TwVec3& getHalfSize() const { return m_halfSize;	}
		void setRigidId(unsigned int rigidId) { m_rigidId = rigidId; }
		virtual ~MRigidBodyComponent();

	private: 

		float m_mass = 1.0f;
		TwVec3 m_halfSize;
		unsigned int m_rigidId = 0;
	};
}

#endif // MRIGIDBODYCOMPONENT_H
