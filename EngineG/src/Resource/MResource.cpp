#include "MResource.h"
#include "MResourceManager.h"

using namespace MG;

MResource::MResource(const wchar_t* file_path, MResourceManager* resourceManager)
	: m_full_path(file_path), m_resourceManager(resourceManager)
{
}

MResource::~MResource()
{
}
