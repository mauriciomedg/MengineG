#include "MResourceManager.h"
#include "MMesh.h"
#include "MTexture.h"
#include "MMaterial.h"
#include <memory>

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

MResourceSharedPtr MResourceManager::createResourceFromFileConcrete(const char* file_path)
{
	auto itt = m_map_resource.find(file_path);

	if (itt != m_map_resource.end())
	{
		return itt->second;
	}

	MResourceSharedPtr ptr;
	if (std::string(file_path).find(".obj") != std::wstring::npos)
	{
		ptr = std::make_shared<MMesh>(file_path, this);
	}
	else if (std::string(file_path).find(".jpg") != std::wstring::npos ||
			std::string(file_path).find(".png") != std::wstring::npos ||
			std::string(file_path).find(".bmp") != std::wstring::npos)
	{
		ptr = std::make_shared<MTexture>(file_path, this);
	}

	if (ptr)
	{
		m_map_resource.emplace(file_path, ptr);
		return ptr;
	}

	//Creating the Game Engine
	//https://www.youtube.com/watch?v=N6-AReMDh6A&list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy&index=31&ab_channel=PardCode

	return MResourceSharedPtr();
}

MResourceSharedPtr MResourceManager::createResourceFromFileConcrete(const char* file_path1, const char* file_path2)
{
	auto itt1 = m_map_resource.find(file_path1);
	auto itt2 = m_map_resource.find(file_path2);

	if (itt1 != m_map_resource.end())
	{
		auto material = std::dynamic_pointer_cast<MMaterial>(itt1->second);
		if (material) return std::make_shared<MMaterial>(file_path1, file_path2, this);
	}

	if (itt2 != m_map_resource.end())
	{
		auto material = std::dynamic_pointer_cast<MMaterial>(itt2->second);
		if (material) return std::make_shared<MMaterial>(file_path1, file_path2, this);
	}

	MResourceSharedPtr ptr;

	if (std::string(file_path1).find(".glsl") != std::string::npos &&
		std::string(file_path2).find(".glsl") != std::string::npos)
	{
		ptr = std::make_shared<MMaterial>(file_path1, file_path2, this);
	}

	if (ptr)
	{
		m_map_resource.emplace(file_path1, ptr);
		m_map_resource.emplace(file_path2, ptr);

		return ptr;
	}

	return MResourceSharedPtr();
}
