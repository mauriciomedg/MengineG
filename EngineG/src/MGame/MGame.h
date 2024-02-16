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
		virtual void onCreate();
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
		std::unique_ptr<MGraphicsEngine> m_GraphicsEngine;
		std::vector<ui32> m_modelsToRender;
		std::vector<ui32> m_shaders;
		std::vector<ui32> m_uniforms;
		std::map<ui32, UniformData> m_uniformData;
	};
}