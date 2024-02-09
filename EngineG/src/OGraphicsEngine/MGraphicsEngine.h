#pragma once

struct GLFWwindow;

namespace MG
{
	class MGraphicsEngine
	{
	public:
		MGraphicsEngine();
		void init();
		bool update();

		~MGraphicsEngine();

	private:
		void display();
		GLFWwindow* m_window;
	};
}