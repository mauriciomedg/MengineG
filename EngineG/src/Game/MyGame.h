#pragma once

#include "../AllMHeaders.h"

namespace MG
{
	class MyPlayer;
	class MyGame : public MGame
	{
	public:
		MyGame();
		~MyGame();

		virtual void create() override;
		virtual void update(f32 dt) override;
		virtual void quit() override;

	private:
		MyPlayer* m_player = nullptr;
		f32 m_theta = 0.0f;
	};
}