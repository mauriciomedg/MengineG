#pragma once
#include <memory>

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
	};
}