#include "TwPhysicsEngine.h"
#include "../Components/MRigidBodyComponent.h"
using namespace MG;

void TwPhysicsEngine::addComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MRigidBodyComponent*>(component))
		m_rigidBodies.emplace(c);
}

void TwPhysicsEngine::removeComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MRigidBodyComponent*>(component))
		m_rigidBodies.erase(c);
}

bool TwPhysicsEngine::update(float dt)
{
	return true;
}
