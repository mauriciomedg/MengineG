#pragma once
#include <memory>
#include <vector>
#include <map>
#include "../OGraphicsEngine/MPrerequisites.h"

namespace MG
{
	class MGraphicsEngine;
	class MEntitySystem;
	class UniformData;
	class MGame
	{
	public:
		MGame();
		virtual ~MGame();

		void run();

		MEntitySystem* getEntitySystem();

	protected:
		virtual void create();
		virtual void update(f32 dt) {};
		virtual void quit();

	private:
		void updateInternal();

	protected:
		bool m_isRunning = true;
		f32 m_lastTime = 0.0f;

		std::unique_ptr<MEntitySystem> m_entitySystem;
		//for testing		
		f32 m_theta = 0.0f;
		//
		MVAOSharedPtr m_vertexArrayObject;
		MShaderProgSharedPtr m_shader;
		MUniformBufferSharedPtr m_uniform;
	};
}