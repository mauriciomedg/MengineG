#pragma once
#include "MPrerequisites.h"
#include "RenderSystem/MRenderSystem.h"

namespace MG
{
	struct MeshData
	{
		MMeshSharedPtr mesh;
		MMaterialSharedPtr material;
		float m_theta;
	};

	struct UniformData
	{
		glm::mat4 world;
		glm::mat4 vMat;
		glm::mat4 pMat;
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