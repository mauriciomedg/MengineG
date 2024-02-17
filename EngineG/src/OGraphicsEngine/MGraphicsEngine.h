#pragma once
#include "MPrerequisites.h"
#include "RenderSystem/MRenderSystem.h"

namespace MG
{
	class MGraphicsEngine
	{
		MGraphicsEngine();
		~MGraphicsEngine();

	public:
		MRenderSystem* getRenderSystem();
		static MGraphicsEngine& getInstance();
	private:
		
		MRenderSystem* m_renderSystem;
	};
}