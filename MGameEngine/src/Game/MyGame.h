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

	struct meshData
	{
		std::vector<float> vertex;
		std::vector<float> indices;
		std::vector<float> textureCoord;
	};

	class MyPlayer;
	class MyGame : public MGame
	{
	public:
		MyGame();
		~MyGame();

		virtual void init() override;
		virtual void update(f32 dt) override;
		virtual void quit() override;
		void requestCreateEntity(float x, float y, float z);
		void requestCreateEntity(float* vertices,
			int vertex_count, 
			int* indices, 
			int index_count,
			float* texture_coord,
			int text_coord_count);

	private:
		void createEntity(data& p);
		void createEntity(meshData& p);
		void commandCreateEntity(data p);
		void commandCreateEntityMesh(meshData p);

	private:
		MyPlayer* m_player = nullptr;
		MEntity* m_entity = nullptr;
		f32 m_theta = 0.0f;

		delegate<data> m_delegate_create;
		delegate<meshData> m_delegate_create_mesh;
		std::list<data> m_list_data;
		std::list<meshData> m_list_mesh_data;

		std::mutex m_mutex;
	};
}