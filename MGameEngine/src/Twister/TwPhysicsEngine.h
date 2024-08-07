#pragma once

#ifndef TWPHYSICSENGINE_H
#define TWPHYSICSENGINE_H

#include <set>
#include <map>

namespace MG
{
	class MComponent;
	class MRigidBodyComponent;
	
	class TwWorld;

	class TwPhysicsEngine
	{

	public:

		TwPhysicsEngine();
		~TwPhysicsEngine();

		void addComponent(MComponent* component);
		void removeComponent(MComponent* component);
		void createRigidBody(MRigidBodyComponent* rigidBodyComponent);

		bool update(float dt);
	private:

		TwWorld* m_world;

		std::set<MRigidBodyComponent*> m_rigidBodieComponents;
	};
}

#endif // TWPHYSICSENGINE_H