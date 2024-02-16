#pragma once

#include "../AllMHeaders.h"

namespace MG
{
	class MyGame : public MGame
	{
	public:
		MyGame();
		~MyGame();

		virtual void onCreate() override;
		virtual void update() override;
		virtual void quit() override;
	};
}