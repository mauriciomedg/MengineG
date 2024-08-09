#include "TwCube.h"
#include "../../Factory.h"
#include "iostream"

namespace MG
{
	TwShape* createCube()
	{
		return new TwCube();
	}

	const bool registerResult =
		Factory<TwShape, TwShape::IDENTIFIER_TYPE>::getInstance().registerProduct(TwShape::TW_CUBE, createCube);

	void TwCube::draw()
	{
		std::cout << "drawwing " << std::endl;
	}

}

