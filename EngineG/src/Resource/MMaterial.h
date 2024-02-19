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
	private:
		MShaderProgSharedPtr m_shader;
	};
}