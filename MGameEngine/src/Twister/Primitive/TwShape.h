#pragma once

#ifndef TWSHAPE_H
#define TWSHAPE_H

#include "MathLibrary/TwMat3.h"
#include "MathLibrary/TwMat4.h"

namespace MG
{
	class TwShape
	{
	public:
		typedef const int IDENTIFIER_TYPE;
		static IDENTIFIER_TYPE TW_LINE = 1,
			TW_PLANE = 2,
			TW_CUBE = 3,
			TW_SPHERE = 4,
			TW_CAPSULE = 5;

		virtual ~TwShape() {};
		virtual void calculateShape(const TwVec3& halfSize, float mass) = 0;
		virtual void draw() = 0;

	protected:
		float m_mass;
		float m_massInv;
		TwMat3 m_Ibody;
		TwMat3 m_IbodyInv;

		TwMat4 m_worldMat;
	};
}

#endif TWSHAPE_H