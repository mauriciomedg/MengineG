#include "TwCube.h"
#include "../../Factory.h"
#include "iostream"

namespace MG
{

	TwCube::TwCube(int a)
	{
		std::cout << "cubo created " << a << std::endl;
	}

	const bool registerResult =
		Factory<TwShape, TYPELIST_1(int), TwShape::IDENTIFIER_TYPE, std::function<TwShape* (int)>>::getInstance().registerProduct(TwShape::TW_CUBE, [](int a){ return new TwCube(a); });// createCube);

	void TwCube::calculateShape(const TwVec3& halfSize, float mass)
	{
		m_mass = mass;

		m_massInv = 1 / mass;

		float x0 = 2.0f * halfSize.x;
		float y0 = 2.0f * halfSize.y;
		float z0 = 2.0f * halfSize.z;

		m_Ibody.setDiagonal(std::pow(y0, 2) + std::pow(z0, 2),
			std::pow(x0, 2) + std::pow(z0, 2),
			std::pow(x0, 2) + std::pow(y0, 2));

		m_Ibody *= m_mass / 12.0f;

		m_IbodyInv = m_Ibody.inverse();
	}

	void TwCube::draw()
	{
		std::cout << "drawwing " << std::endl;
	}

}

