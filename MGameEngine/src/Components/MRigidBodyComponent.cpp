#include "MRigidBodyComponent.h"
#include "../Entity/MEntity.h"

#include "../Utils.h"
#include "../Factory.h"
#include "../Twister/TwPhysicsEngine.h"

#include <iostream>

using namespace MG;

MRigidBodyComponent::~MRigidBodyComponent()
{
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->removeComponent(this);
}

void MRigidBodyComponent::create(dataSphape data)
{
	m_shape.reset(Factory<TwShape, TYPELIST_1(int), TwShape::IDENTIFIER_TYPE, std::function<TwShape*(int)>>::getInstance().createObject(TwShape::TW_CUBE, 2));
	m_shape->calculateShape(data.extend, m_mass);
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->createRigidBody(this);
}

void MRigidBodyComponent::onCreateInternal()
{
	m_entity->getEntitySystem()->getGame()->getPhysicsEngine()->addComponent(this);

	auto& events = m_entity->getEntitySystem()->getGame()->getEventSystem().m_events;

	events.insert(std::make_pair(m_entity->getId(), delegate<dataSphape>::from_method<MRigidBodyComponent, &MRigidBodyComponent::create>(this)));
}

void MRigidBodyComponent::updateTransform(const TwMat4& transform)
{
	m_entity->getTransform()->setWorldMat(Utils::convertToglmmat4(transform));
}