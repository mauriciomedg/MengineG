#pragma once
#include <memory>
#include <vector>
#include <map>
#include "../OGraphicsEngine/MPrerequisites.h"
#include "../EventSystem.h"

namespace MG
{
	class TwPhysicsEngine;
	class MGraphicsEngine;
	class MEntitySystem;
	class MGame
	{
	public:
		MGame();
		virtual ~MGame();

		void run();

		MEntitySystem* getEntitySystem();
		MGraphicsEngine* getGraphicEngine();
		MResourceManager* getResourceManager();
		TwPhysicsEngine* getPhysicsEngine();
		EventSystem& getEventSystem() { return m_eventSystem; };
	protected:
		virtual void create();
		virtual void update(f32 dt) = 0;
		virtual void quit();

	private:
		void updateInternal();

	protected:
		bool m_isRunning = true;
		f32 m_lastTime = 0.0f;
				
		std::unique_ptr<MEntitySystem> m_entitySystem;
		std::unique_ptr<MGraphicsEngine> m_graphicEngine;
		std::unique_ptr<MResourceManager> m_resourceManager;
		std::unique_ptr<TwPhysicsEngine> m_physicsEngine;

		EventSystem m_eventSystem;

		//for testing		
		
		//
		//MVAOSharedPtr m_vertexArrayObject;
		//MShaderProgSharedPtr m_shader;
		//MUniformBufferSharedPtr m_uniform;

	};
}