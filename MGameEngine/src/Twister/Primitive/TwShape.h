#pragma once

#ifndef TWSHAPE_H
#define TWSHAPE_H

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
		virtual void draw() = 0;
	};
}

#endif TWSHAPE_H