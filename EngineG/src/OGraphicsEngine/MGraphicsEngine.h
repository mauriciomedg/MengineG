#pragma once
#include "MPrerequisites.h"
#include "RenderSystem/MRenderSystem.h"

namespace MG
{
	class MGraphicsEngine
	{
	public:

		MGraphicsEngine();
		~MGraphicsEngine();

		MRenderSystem* getRenderSystem();

	private:
		
		std::unique_ptr<MRenderSystem> m_renderSystem;
	};
}