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
		virtual void update(f32 dt) override;
		virtual void quit() override;
	private:
		f32 m_timeSec = 0.0f;
		MEntity* m_entity = nullptr;
	};
}