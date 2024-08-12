#include "TwRigidBody.h"
#include <math.h>

using namespace MG;

TwRigidBody::TwRigidBody(std::shared_ptr<TwShape>& shape, bool isStatic)
	: m_shape(shape), m_isStatic(isStatic)
{
}




