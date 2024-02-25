#pragma once
#include "MPrerequisites.h"
#include "RenderSystem/MRenderSystem.h"
#include <set>

namespace MG
{
	class MGraphicsEngine
	{
	public:

		MGraphicsEngine();
		~MGraphicsEngine();

		MRenderSystem* getRenderSystem();

		bool update();

		void addComponent(MComponent* component);
		void removeComponent(MComponent* component);

	private:
		
		std::unique_ptr<MRenderSystem> m_renderSystem;

		std::set<MMeshComponent*> m_meshes;
	};
}