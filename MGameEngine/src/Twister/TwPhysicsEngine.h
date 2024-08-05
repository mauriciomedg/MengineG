#pragma once
#include <set>

namespace MG
{
	class MComponent;
	class MRigidBodyComponent;

	class TwPhysicsEngine
	{

	public:

		void addComponent(MComponent* component);
		void removeComponent(MComponent* component);
		bool update(float dt);
	private:

		std::set<MRigidBodyComponent*> m_rigidBodies;
	};

}
