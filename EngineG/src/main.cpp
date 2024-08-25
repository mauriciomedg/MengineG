//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "MGameEngine.h"
#include <thread>
#include <vector>
//#include "Inputs.h"

//#include "Utils.h"
//#include "Camera.h"
//
//#include "GEntity.h"
//#include "physicsEngine/PhysicsWorld.h"
//#include "Cube.h"
//
//#include "Game/MyGame.h"
//
//#include "testDLL.h"
//
//#include "pybind11/pybind11.h"
//#include <pybind11/embed.h>
//namespace py = pybind11;
//
//#define numVAOs 1
//#define numVBOs 7
//GLuint renderingProgram;
//GLuint vao[numVAOs];
//GLuint vbo[numVBOs];
//GLuint mvLoc, projLoc;
//
//////////////////////////////
//PhysicsWorld* pWorld = new PhysicsWorld;
//
//std::vector<GEntityBox*> gameObjectArray;
//GEntityBoxControlled* boxControlled = new GEntityBoxControlled(pWorld);
//
//////////////////////////////
//
//using namespace std;
//
//GLuint createShaderProgram() 
//{
//	string vertShaderStr = Utils::readShaderSource("shaders/vertShader.glsl");
//	string fragShaderStr = Utils::readShaderSource("shaders/fragShader.glsl");
//
//	const char* vshaderSource = vertShaderStr.c_str();
//	const char* fshaderSource = fragShaderStr.c_str();
//	
//	return Utils::createShaderProgram(vshaderSource, fshaderSource);
//}
//
//Camera* camera = new Camera;
//
//Sphere* sphere = new Sphere;
//
//Model* model = new Model;
//
////Pyramid onePyramid;
//
//void testBallSocket()
//{
//	std::vector<GEntityBox*> toConnect;
//	for (int j = 0; j < 2; ++j)
//	{
//		GEntityBox* obj = new GEntityBox(pWorld);
//		glm::vec3 Pos(20.0f, 45.0 - j * 5, 0.0f);
//		auto R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//		auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
//		obj->init(&vbo[0], &vbo[1], Mat, 3.0f, 1.0f, true);
//		obj->setAffectedByGravity(true);
//		gameObjectArray.push_back(obj);
//		toConnect.push_back(obj);
//	}
//
//	pWorld->addRigidPointConstraint(toConnect[0]->getPhysicsID(), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(20.0f, 65.0f, 0.0f), 10.0f);
//	pWorld->addRigidPointRigidPointConstraint(toConnect[0]->getPhysicsID(), glm::vec3(-3.0f, -3.0f, -3.0f), toConnect[1]->getPhysicsID(), glm::vec3(3.0f, 3.0f, 3.0f), 15.0f);
//
//}
//
//void init(GLFWwindow* window) 
//{
//	renderingProgram = createShaderProgram();
//	glGenVertexArrays(numVAOs, vao);
//	glBindVertexArray(vao[0]);
//	glGenBuffers(numVBOs, vbo);
//
//	// Camera
//	glm::vec3 Pos = glm::vec3(0.0f, 21.0f, 60.0f);
//	//auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
//	auto Mat = glm::translate(glm::mat4(1.0f), Pos);
//	camera->init(window, Mat, pWorld, false);
//
//	// box controlled
//	Pos = glm::vec3(-10.0f, 30.0f, 0.0f);
//	auto R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
//	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
//	boxControlled->init(&vbo[0], &vbo[1], Mat, 5.0f, 0.8f, true);
//	
//	for (int j = 0; j < 1; ++j)
//	{
//		GEntityBox* obj = new GEntityBox(pWorld);
//		glm::vec3 Pos(50.0f, 25.0, 0.0f);
//		auto R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//		auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
//		obj->init(&vbo[0], &vbo[1], Mat, 10.0f, 1.0f, true);
//		obj->setAffectedByGravity(true);
//		gameObjectArray.push_back(obj);
//	
//	}
//	
//	testBallSocket();
//	
//	// Sphere
//	Pos = glm::vec3(-5.0f, 30.0f, 0.0f);
//	Mat = glm::translate(glm::mat4(1.0f), Pos);
//	sphere->init(&vbo[2], &vbo[3], Mat, 10.0f);
//	// 
//	
//	// Model
//	Pos = glm::vec3(0.0f, 25.0, 0.0f);
//	Mat = glm::translate(glm::mat4(1.0f), Pos);
//	model->init(&vbo[4], &vbo[5], &vbo[6], Mat);
//	// Plane
//	Pos = glm::vec3(0.0f, -5.0f, 0.0f);
//	//R = glm::mat4(1.0f);
//	R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
//	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
//	pWorld->instanciatePrimitivePlane(Mat, 1.0, false);
//	
//	pWorld->init();
//}
//
//double lastTime = 0.0f;
//double currentTime = 0;
//
//void display(GLFWwindow* window, double currentTime)
//{
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
//	glEnable(GL_CULL_FACE);
//	glUseProgram(renderingProgram);
//
//	// get the uniform variables for the MV and projection matrices
//	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
//	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
//	//GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"
//	
//	//////////////
//	double current = glfwGetTime();
//	double elapsed = current - lastTime;
//	currentTime += elapsed;
//
//	//
//	Inputs::get().update(window);
//	//
//	pWorld->simulating(elapsed);
//	camera->update();
//
//	for (GEntityBox* obj : gameObjectArray)
//	{
//		obj->update(camera, renderingProgram);
//	}
//
//	boxControlled->update(camera, renderingProgram);
//	///
//
//	//sphere->update(camera, renderingProgram);
//	model->update(camera, renderingProgram);
//
//	lastTime = glfwGetTime();
//
//	//Utils::drawLine(glm::vec3(0.0f), glm::vec3(10.0f), 10.0f);
//
//	//GLfloat line[] = {
//	//	10.0f, 10.0f, 10.0f,
//	//	0.0f, 0.0f, 0.0f
//	//};
//	//
//	//glEnable(GL_LINE_SMOOTH);
//	//glLineWidth(10);
//	//glEnableClientState(GL_VERTEX_ARRAY);
//	//glVertexPointer(3, GL_FLOAT, 0, line);
//	//glDrawArrays(GL_LINES, 0, 2);
//	//glDisableClientState(GL_VERTEX_ARRAY);
//	//glDisable(GL_LINE_SMOOTH);
//}
//
//void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight)
//{
//	camera->reshapeWindow(window, newWidth, newHeight);
//}
//
//int add(int i, int j) {
//	return i + j;
//}
//
//PYBIND11_EMBEDDED_MODULE(example, m) {
//	m.doc() = "pybind11 example plugin"; // optional module docstring
//	m.def("add", &add, "A function that adds two numbers");
//}
//
//void say()
//{
//	std::cout << "dsaasfda" << std::endl;
//}
//
//PYBIND11_EMBEDDED_MODULE(embeddedmold, m)
//{
//	m.doc() = "Embb mod";
//	m.def("say", &say);
//}

class RunEngine
{
private:
	RunEngine() {};
	RunEngine(const RunEngine&);
	RunEngine& operator=(const RunEngine&);

	void runEngine();

public:

	~RunEngine();
	void start();
	void quit()
	{
		if (m_game)
			m_game->quit();
					
		if (m_first.joinable())
			m_first.join();

		m_game = nullptr;
	}

	void createEntity(float pX, float pY, float pZ)
	{
		m_game->createEntity(pX, pY, pZ);
	}

	void createEntityMesh()
	{
		m_game->createEntityMesh(nullptr, 0, nullptr, 0, nullptr, 0);
	}

	static RunEngine& getInstace();

private:
	std::thread m_first;
	MGameEngine* m_game = nullptr;
};

void RunEngine::runEngine()
{
	m_game = new MGameEngine();
	m_game->run();
	delete m_game;
}

RunEngine::~RunEngine()
{
	quit();
}

void RunEngine::start()
{
	std::thread firstTemp([&]() { this->runEngine(); });
	m_first.swap(firstTemp);
}

RunEngine& RunEngine::getInstace()
{
	static RunEngine instance;
	return instance;
}

/// //////////////////////////////////////////////
void getInstance()
{
	RunEngine::getInstace();
}

void start()
{
	RunEngine::getInstace().start();
}

void createEntity(float pX, float pY, float pZ)
{
	RunEngine::getInstace().createEntity(pX, pY, pZ);
}

void createEntityMesh()
{
	RunEngine::getInstace().createEntityMesh();
}

void quit()
{
	RunEngine::getInstace().quit();
}

void RunEngineInAnotherProcess()
{
	getInstance();

	int c = 0;

	while (c < 5)
	{
		std::cin >> c;

		if (c == 0)
		{
			start();
		}
		else if (c == 2)
		{
			createEntityMesh();
		}
		else if (c == 4)
		{
			quit();
		}
	}
}

int main(void) 
{

	//std::vector<int> dataVec;
	//
	//int dataArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//int dataArray2[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, 100, 200, 300, 400, 500, 600, 700, 800 };
	//
	//int textureId = 0;
	//for (int i = 0; i < 9; i = i + 3)
	//{
	//	std::cout << dataArray[i] << " "  << dataArray[i + 1] << " " << dataArray[i + 2] << std::endl;
	//	std::cout << dataArray2[textureId] << " " << dataArray2[textureId + 1] << std::endl;
	//	textureId += 2;
	//}
	//
	//
	//unsigned dataArraySize = sizeof(dataArray) / sizeof(int);
	//
	//// Method 1: Copy the array to the vector using back_inserter.
	//{
	//	std::copy(&dataArray[0], &dataArray[dataArraySize], back_inserter(dataVec));
	//}


	//std::cout << runExample(0.0) << std::endl;

	//Embedding Python in C++ with pybind11 - SCRIPTING [3/4]
	//py::scoped_interpreter guard{};
	
	//py::exec("import embeddedmold\nembeddedmold.say()");

	//auto sys = py::module::import("sys"); // sys bp::module
	//py::print(sys.attr("path"));
	//auto hellowModule = py::module::import("TetsPython");
	
	//Py_Initialize();
	////PyRun_SimpleString("from time import time,ctime\n"
	////	"print 'Today is',ctime(time())\n");
	////FILE* f;
	//
	//PyObject* obj = Py_BuildValue("s", "TetsPython.py");
	//FILE* file = _Py_fopen_obj(obj, "r+");
	//if (file)
	//	PyRun_SimpleFile(file, "TetsPython.py");
	//Py_Finalize();

	//MG::MyGame game;
	//{
	//	MGameEngine game;
	//	game.run();
	//}

	RunEngineInAnotherProcess();

	//game.create();
	//game.run();

	//OpenGL Chemo https://www.youtube.com/watch?v=Bcs56Mm-FJY&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=12&ab_channel=TheCherno
	//C++ 3D Game Tutorial 16
	//C++ 3D Game Tutorial 15

	//OpenGL / C++ 3D Tutorial 25 - Mesh class (A humble beginning!)
	//C++ OpenGL 3D Game Tutorial 2: Making OpenGL 3D Engine
	//C++ 3D Game Tutorial Series 33

	//if (!glfwInit()) { exit(EXIT_FAILURE); }
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	//glfwMakeContextCurrent(window);
	//
	//if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	//glfwSwapInterval(1);
	//init(window);
	//glfwSetWindowSizeCallback(window, window_reshape_callback);
	//
	//Inputs& inputs = Inputs::get();
	//inputs.mapInput("MoveForward", "up", 1.0f);
	//inputs.mapInput("MoveLeft", "left", -1.0f);
	//inputs.mapInput("MoveRight", "right", 1.0f);
	//inputs.mapInput("MoveForwardCamera", "w", 1.0f);
	//inputs.mapInput("MoveBackwardCamera", "s", -1.0f);
	//inputs.mapInput("MoveLeftCamera", "a", -1.0f);
	//inputs.mapInput("MoveRightCamera", "d", 1.0f);
	//
	//while (!glfwWindowShouldClose(window)) {
	//	display(window, glfwGetTime());
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();	
	//}
	//glfwDestroyWindow(window);
	//glfwTerminate();
	//exit(EXIT_SUCCESS);
}