#pragma once

#ifndef TWRIGIDBODY_H
#define TWRIGIDBODY_H

#include "MathLibrary/TwVec3.h"
#include "MathLibrary/TwQuat.h"
#include "MathLibrary/TwMat3.h"
#include "MathLibrary/TwMat4.h"

#include "Primitive/TwShape.h"

namespace MG
{
	class TwRigidBody
	{

	public:
		TwRigidBody(std::shared_ptr<TwShape>& shape, bool isStatic);

		~TwRigidBody() {};

		TwVec3 m_X;
		TwQuat m_Q;

		std::shared_ptr<TwShape> m_shape;

		//Derived quantites
		TwVec3 m_V;
		TwVec3 m_W; //w(t)
		TwMat3 m_Iinv;
		TwMat3 m_I;
		
		bool m_isStatic = false;

	};
}

#endif // TWRIGIDBODY_H