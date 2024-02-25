#pragma once

#include "../AllMHeaders.h"

namespace MG
{
	class MyPlayer : public MEntity
	{
	public:
		MyPlayer();
		~MyPlayer();

		virtual void create() override;
		virtual void update(f32 dt) override;

	private:
		void bindAxis();
		void moveForward(float val);
		void moveSide(float val);
		void mouseX(float val);
		void mouseY(float val);
	private:
		f32 m_timeSec = 0.0f;
		MEntity* m_entity = nullptr;
	};
}