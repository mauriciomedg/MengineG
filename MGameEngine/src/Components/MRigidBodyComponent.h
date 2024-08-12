#pragma once

#ifndef MRIGIDBODYCOMPONENT_H
#define MRIGIDBODYCOMPONENT_H

#include "../AllMHeaders.h"
#include "../MathLibrary/TwMat4.h"
#include "../MathLibrary/TwVec3.h"
#include "../Twister/Primitive/TwShape.h"

#include "../EventSystem.h" // temporal, we need it because of the dataShape

namespace MG
{
	class TwShape;

	class MRigidBodyComponent : public MComponent
	{
		
	protected:
		virtual void onCreateInternal() override;

	public:
		void create(dataSphape data);
		void updateTransform(const TwMat4& transform);
		std::shared_ptr<TwShape>& getShape() { return m_shape; };
		bool isStatic() const { return m_isStatic; }
		void setRigidId(unsigned int rigidId) { m_rigidId = rigidId; }
		virtual ~MRigidBodyComponent();

	private: 

		float m_mass = 1.0f;
		bool m_isStatic = false;
		unsigned int m_rigidId = 0;
		std::shared_ptr<TwShape> m_shape;
	};
}

#endif // MRIGIDBODYCOMPONENT_H
