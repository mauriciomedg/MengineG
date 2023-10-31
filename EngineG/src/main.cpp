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

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[2];
GLuint mvLoc, projLoc;

////////////////////////////
PhysicsWorld* pWorld = new PhysicsWorld;
GEntity* gameObject = new GEntity;
GEntity* gameObject2 = new GEntity;

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

float vPositions[] = { 0.25, -0.25, 0.0,
						-0.25, -0.25, 0.0,
						0.25, 0.25, 0.0 };

Camera camera;

//Pyramid onePyramid;

void init(GLFWwindow* window) 
{ 
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(2, vbo);
	
	/// set body gameObject in the scene
	glm::vec3 Pos(-5.0f, 40.0f, 25.0f);
	auto R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::vec3(1.0f, 0.0f, 0.0f));
	auto Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	gameObject->init(&vbo[0], Mat);
	//
	// set body gameObject2 in the scene
	Pos = glm::vec3(5.0f, 40.0f, 25.0f);
	R = glm::rotate(glm::mat4(1.0f), 1.75f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	Mat = glm::translate(glm::mat4(1.0f), Pos) * glm::mat4(R);
	gameObject2->init(&vbo[1], Mat);
	//
	
	//TODO: should be added in the world when there is a solver
	pWorld->mBodies.push_back(gameObject->rigidBody);
	//pWorld->mBodies.push_back(gameObject2->rigidBody);

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

	///
	camera.update(window);
	///
	gameObject->consumeInput();
	pWorld->runSimulation(elapsed);
	gameObject->update(currentTime, &camera, renderingProgram);
	gameObject2->update(currentTime, &camera, renderingProgram);
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