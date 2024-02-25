#pragma once

#include "../OGraphicsEngine/MPrerequisites.h"
#include "MResource.h"
#include <string>
#include <map>
#include <vector>

namespace MG
{
	struct UniformData
	{
		glm::mat4 world;
		glm::mat4 vMat;
		glm::mat4 pMat;
	};

	class MMaterial : public MResource
	{
	public:
		MMaterial(const char* file_path1, const char* file_path2, MResourceManager* resourceManager);
		MShaderProgSharedPtr& getShaderProgram();

		void addUniform(const std::string& name, ui32 size, ui32 slot);
		void addTexture(const MTextureSharedPtr& texture);
		void setUniformData(const std::string& name, void* data);
		const MUniformBufferSharedPtr& getUniform(const std::string& name);
		const MTexture2DSharedPtr& getTexture() const;

	private:
		void setUniformSlot(const char* name, ui32 slot);

	private:
		MShaderProgSharedPtr m_shader;
		std::map<std::string, MUniformBufferSharedPtr> m_uniforms;
		std::vector<MTextureSharedPtr> m_textures;
	};
}