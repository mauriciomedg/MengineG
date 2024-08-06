#include "MRigidBodyComponent.h"
#include "../Entity/MEntity.h"

#include "../Twister/TwPhysicsEngine.h"
#include "../Utils.h"

#include <iostream>

using namespace MG;

MRigidBodyComponent::~MRigidBodyComponent()
{
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->removeComponent(this);
}

void MRigidBodyComponent::updateShape(dataSphape data)
{
	m_halfSize = data.extend;
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->createRigidBody(this);
}

void MRigidBodyComponent::onCreateInternal()
{
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->addComponent(this);

	auto& events = m_entity->getEntitySystem()->getGame()->getEventSystem().m_events;

	events.insert(std::make_pair(m_entity->getId(), delegate<dataSphape>::from_method<MRigidBodyComponent, &MRigidBodyComponent::updateShape>(this)));
}

void MRigidBodyComponent::updateTransform(const TwMat4& transform)
{
	m_entity->getTransform()->setWorldMat(Utils::convertToglmmat4(transform));
}