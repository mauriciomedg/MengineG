#pragma once

#include "../AllMHeaders.h"
#include "../OGraphicsEngine/MPrerequisites.h"
#include "../OGraphicsEngine/MGraphicsEngine.h"

#include <vector>

namespace MG
{
	class MMeshComponent : public MComponent
	{
	public: 
		MMeshComponent();
		virtual ~MMeshComponent();

		void setMesh(const MMeshSharedPtr& mesh);
		const MMeshSharedPtr& getMesh();

		void addMaterial(const MMaterialSharedPtr& material);
		void removeMaterial(ui32 index);

		const std::vector<MMaterialSharedPtr>& getMaterials();

	protected:
		virtual void onCreateInternal() override;
	private:
		MMeshSharedPtr m_mesh;
		std::vector<MMaterialSharedPtr> m_materials;
	};
}