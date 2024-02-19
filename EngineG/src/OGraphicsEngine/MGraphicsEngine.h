#pragma once
#include "MPrerequisites.h"
#include "RenderSystem/MRenderSystem.h"

namespace MG
{
	struct MeshData
	{
		MMeshSharedPtr mesh;
		MMaterialSharedPtr material;
	};

	class MGraphicsEngine
	{
	public:

		MGraphicsEngine();
		~MGraphicsEngine();

		MRenderSystem* getRenderSystem();

		bool update(const MeshData& meshData);
	private:
		
		std::unique_ptr<MRenderSystem> m_renderSystem;
	};
}