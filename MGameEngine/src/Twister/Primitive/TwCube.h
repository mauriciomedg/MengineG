#pragma once

#ifndef TWCUBE_H
#define TWCUBE_H

#include "TwShape.h"

namespace MG
{
	class TwCube : public TwShape
	{
	public:
		static void registerCube();
		~TwCube() {}
		virtual void calculateShape(const TwVec3& halfSize, float mass) override;
		virtual void draw() override;
	};
}

#endif TWCUBE_H // TWCUBE_H
