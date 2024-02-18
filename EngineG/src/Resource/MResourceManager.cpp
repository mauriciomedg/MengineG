#include "MResourceManager.h"
#include "MMesh.h"

#include <filesystem>

using namespace MG;

MResourceManager::MResourceManager(MGame* game)
	: m_game(game)
{
}

MResourceManager::~MResourceManager()
{
}

MGame* MG::MResourceManager::getMGame()
{
	return m_game;
}

MResourceSharedPtr MResourceManager::createResourceFromFileConcrete(const wchar_t* file_path)
{
	
	auto itt = m_map_resource.find(file_path);

	if (itt != m_map_resource.end())
	{
		return itt->second;
	}

	if (std::wstring(file_path).empty())//".obj"))
	{
		auto ptr = std::make_shared<MMesh>(file_path, this);
		if (ptr)
		{
			m_map_resource.emplace(file_path, ptr);
			return ptr;
		}
	}

	//Creating the Game Engine
	//https://www.youtube.com/watch?v=N6-AReMDh6A&list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy&index=31&ab_channel=PardCode

	return MResourceSharedPtr();
}
