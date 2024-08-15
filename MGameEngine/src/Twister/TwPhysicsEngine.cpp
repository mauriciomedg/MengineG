#include "TwPhysicsEngine.h"
#include "MathLibrary/TwMat4.h"
#include "../Components/MRigidBodyComponent.h"
#include "../Components/MCollisionShapeComponent.h"
#include "../Twister/TwRigidBody.h"

using namespace MG;

namespace MG
{
	class TwWorld
	{

	public:
		TwWorld() {}
		~TwWorld() {}

		unsigned int addRigidBody(std::shared_ptr<TwShape>& shape, bool isStatic)
		{
			unsigned int size = m_rigidBodies.size();

			TwRigidBody* body = new TwRigidBody(shape, isStatic);
			m_rigidBodies.insert(std::make_pair(size + 1, body));

			return size + 1;
		};

		void removeRigidBody() {};

	private:
		std::map<unsigned int, std::shared_ptr<TwRigidBody>> m_rigidBodies;
	};
}

TwPhysicsEngine::TwPhysicsEngine()
{
	m_world = new TwWorld; // std::make_unique<TwWorld>();
}

TwPhysicsEngine::~TwPhysicsEngine()
{
	delete m_world;
}

void TwPhysicsEngine::createRigidBody(MRigidBodyComponent* rigidBodyComponent)
{
	unsigned int id = m_world->addRigidBody(rigidBodyComponent->getShape(), rigidBodyComponent->isStatic());
	rigidBodyComponent->setRigidId(id);
}

void TwPhysicsEngine::addComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MRigidBodyComponent*>(component))
		m_rigidBodieComponents.emplace(c);

	if (auto c = dynamic_cast<MCollisionShapeComponent*>(component))
		m_collisionShapeComponent.emplace(c);
}

void TwPhysicsEngine::removeComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MRigidBodyComponent*>(component))
		m_rigidBodieComponents.erase(c);

	if (auto c = dynamic_cast<MCollisionShapeComponent*>(component))
		m_collisionShapeComponent.erase(c);
}

bool TwPhysicsEngine::update(float dt)
{
	return true;
}
