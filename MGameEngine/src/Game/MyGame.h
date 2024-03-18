#pragma once

#include "../AllMHeaders.h"
#include <list>
#include <mutex>

namespace MG
{
	struct data
	{
		float m_px;
		float m_py;
		float m_pz;
	};

	class MyPlayer;
	class MyGame : public MGame
	{
	public:
		MyGame();
		~MyGame();

		virtual void create() override;
		virtual void update(f32 dt) override;
		virtual void quit() override;
		void requestCreate(float x, float y, float z);

	private:
		void createEntity(data p);
		void commandCreateEntity(data p);

	private:
		MyPlayer* m_player = nullptr;
		MEntity* m_entity = nullptr;
		f32 m_theta = 0.0f;

		delegate<data> m_delegate_create;
		std::list<data> m_list_data;

		std::mutex m_mutex;
	};
}