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
		std::shared_ptr<T> createResourceFromFile(const char* file_path)
		{
			return std::dynamic_pointer_cast<T>(createResourceFromFileConcrete(file_path));
		}

		template<typename T>
		std::shared_ptr<T> createResourceFromFile(const char* file_path1, const char* file_path2)
		{
			return std::dynamic_pointer_cast<T>(createResourceFromFileConcrete(file_path1, file_path2));
		}

	private:
		MResourceSharedPtr createResourceFromFileConcrete(const char* file_path);
		MResourceSharedPtr createResourceFromFileConcrete(const char* file_path1, const char* file_path2);

		std::unordered_map<std::string, MResourceSharedPtr> m_map_resource;

		MGame* m_game = nullptr;
	};
}