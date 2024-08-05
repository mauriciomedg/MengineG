#include "MRigidBodyComponent.h"
#include "../Entity/MEntity.h"

#include "../Twister/TwPhysicsEngine.h"
#include "../Utils.h"

using namespace MG;

MRigidBodyComponent::~MRigidBodyComponent()
{
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->removeComponent(this);
}

void MRigidBodyComponent::onCreateInternal()
{
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->addComponent(this);
}

void MRigidBodyComponent::updateTransform(const TwMat4& transform)
{
	m_entity->getTransform()->setWorldMat(Utils::convertToglmmat4(transform));
}