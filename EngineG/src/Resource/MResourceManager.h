#pragma once
#include "../OGraphicsEngine/MPrerequisites.h"
#include <unordered_map>

namespace MG
{
	class MGame;
	class MResourceManager
	{
	public:
		MResourceManager(MGame* game);
		~MResourceManager();

		MGame* getMGame();

		template<typename T>
		std::shared_ptr<T> createResourceFromFile(const wchar_t* file_path)
		{
			return std::dynamic_pointer_cast<T>(createResourceFromFileConcrete(file_path));
		}

	private:
		MResourceSharedPtr createResourceFromFileConcrete(const wchar_t* file_path);
		std::unordered_map<std::wstring, MResourceSharedPtr> m_map_resource;

		MGame* m_game = nullptr;
	};
}