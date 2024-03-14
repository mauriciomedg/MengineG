#pragma once
#include "../OGraphicsEngine/MPrerequisites.h"
#include "MResource.h"

namespace MG
{
	class MTexture : public MResource
	{
	public:
		MTexture(const char* file_path, MResourceManager* resourceManager);
		MTexture2DSharedPtr& getTexture();
	private:
		MTexture2DSharedPtr m_texture;
	};
}