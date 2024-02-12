#pragma once
#include <memory>
#include <vector>
#include "../OGraphicsEngine/MPrerequisites.h"

namespace MG
{
	class MGraphicsEngine;

	class MGame
	{
	public:
		MGame();
		void init();
		~MGame();

		void update();
		void quit();
	protected:
		bool m_isRunning = true;
		std::unique_ptr<MGraphicsEngine> m_GraphicsEngine;
		std::vector<ui32> m_modelsToRender;
		std::vector<ui32> m_shaders;
	};
}