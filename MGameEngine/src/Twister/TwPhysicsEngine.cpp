#include "TwPhysicsEngine.h"
#include "../Components/MRigidBodyComponent.h"
#include "../Twister/TwRigidBody.h"
#include "../Twister/MathLibrary/TwMat4.h"

using namespace MG;

namespace MG
{
	class TwWorld
	{

	public:
		TwWorld() {}
		~TwWorld() {}

		unsigned int addRigidBody(float mass, const TwVec3& halfSize)
		{
			unsigned int size = m_rigidBodies.size();

			TwDynamicShape* shape = TwDynamicBox::create(halfSize, mass);
			TwRigidBody* body = TwRigidBody::create(shape, TwMat4(), false);
			m_rigidBodies.insert(std::make_pair(size + 1, body));

			return size + 1;
		};

		void removeRigidBody() {};

	private:
		std::map<unsigned int, TwRigidBody*> m_rigidBodies;
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
	unsigned int id = m_world->addRigidBody(rigidBodyComponent->getMass(), rigidBodyComponent->getHalfSize());
	rigidBodyComponent->setRigidId(id);
}

void TwPhysicsEngine::addComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MRigidBodyComponent*>(component))
		m_rigidBodieComponents.emplace(c);
}

void TwPhysicsEngine::removeComponent(MComponent* component)
{
	if (auto c = dynamic_cast<MRigidBodyComponent*>(component))
		m_rigidBodieComponents.erase(c);
}

bool TwPhysicsEngine::update(float dt)
{
	return true;
}
