#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Inputs.h"

#include "Utils.h"
#include "Camera.h"

#include "GEntity.h"
#include "physicsEngine/PhysicsWorld.h"
#include "Delegate.h"

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[2];
GLuint mvLoc, projLoc;

////////////////////////////
PhysicsWorld* pWorld = new PhysicsWorld;

std::vector<GEntityBox*> gameObjectArray;
GEntityBoxControlled* gameObject2 = new GEntityBoxControlled(pWorld);

////////////////////////////

using namespace std;

GLuint createShaderProgram() 
{
	string vertShaderStr = Utils::readShaderSource("shaders/vertShader.glsl");
	string fragShaderStr = Utils::readShaderSource("shaders/fragShader.glsl");

	const char* vshaderSource = vertShaderStr.c_str();
	const char* fshaderSource = fragShaderStr.c_str();
	
	return Utils::createShaderProgram(vshaderSource, fshaderSource);
}

Camera camera;

//Pyramid onePyramid;

void init(GLFWwindow* window) 
{ 
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(2, vbo);
	
	// set body gameObject2 in the scene
	glm::vec3 Pos = glm::vec3(3.0f, 40.0f, 25.0f);
	auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	gameObject2->init(&vbo[1], Mat, true);
	//

	for (int h = 0; h < 10; ++h)
	{
		for (int j = 0; j < 3; ++j)
		{
			GEntityBox* obj = new GEntityBox(pWorld);
			glm::vec3 Pos(-5.0f + j * 10.0f, 20 + h * 5.0f, 25.0f);
			auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::vec3(1.0f, 0.0f, 0.0f));
			auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
			obj->init(&vbo[0], Mat, true);
			gameObjectArray.push_back(obj);
		}
	}

	//
	Pos = glm::vec3(0.0f, 20.0f, 0.0f);
	R = glm::mat4(1.0f);
	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	pWorld->instanciatePrimitivePlane(Mat, false);
	//
	pWorld->init();
}

double lastTime = 0.0f;
double currentTime = 0;

void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset"

	//////////////
	double current = glfwGetTime();
	double elapsed = current - lastTime;
	currentTime += elapsed;

	//
	Inputs::get().update();
	//
	///
	camera.update(window);
	///
	pWorld->runSimulation(elapsed);

	for (GEntityBox* obj : gameObjectArray)
	{
		obj->update(&camera, renderingProgram);
	}

	gameObject2->update(&camera, renderingProgram);
	///
	
	lastTime = glfwGetTime();
}

int main(void) 
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);
	
	Inputs& inputs = Inputs::get();
	inputs.mapInput("MoveForward", 'w', 1.0f);
	inputs.mapInput("MoveLeft", 'a', -1.0f);
	inputs.mapInput("MoveRight", 'd', 1.0f);

	std::thread first([&]() { inputs.updateIO(); });     // spawn new thread that calls updateIO()
		
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();	
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}