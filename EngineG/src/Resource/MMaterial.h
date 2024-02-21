#pragma once

#include "../OGraphicsEngine/MPrerequisites.h"
#include "MResource.h"

namespace MG
{
	class MMaterial : public MResource
	{
	public:
		MMaterial(const char* file_path1, const char* file_path2, MResourceManager* resourceManager);
		MShaderProgSharedPtr& getShaderProgram();

		void addUniform(const MUniformBufferSharedPtr& uniform);
		void addTexture(const MTextureSharedPtr& texture);
		void setUniformSlot(const char* name, ui32 slot);
		void setUniformData(void* data);
		const MUniformBufferSharedPtr& getUniform() const;
		const MTexture2DSharedPtr& getTexture() const;

	private:
		MShaderProgSharedPtr m_shader;
		MUniformBufferSharedPtr m_uniform;
		MTextureSharedPtr m_texture;
	};
}