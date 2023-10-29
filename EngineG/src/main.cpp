#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Inputs.h"

#include "Utils.h"
#include "Cube.h"
#include "Camera.h"
#include "Pyramid.h"

#include "GEntity.h"
#include "physicsEngine/ParticlesSimulation.h"
#include "physicsEngine/PhysicsWorld.h"

#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[2];
GLuint mvLoc, projLoc;

////////////////////////////
Particle* phyPyramid = new Particle;
ParticleSystem* pSystem = new ParticleSystem;
PhysicsWorld* pWorld = new PhysicsWorld;
GEntity* gameObject = new GEntity;

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
	
	gameObject->init(vbo);
	pWorld->mBodies.push_back(gameObject->rigidBody);
	pWorld->init();
	//onePyramid.init(vbo);
	
	//phyPyramid->x = onePyramid.mPos;
	phyPyramid->v = glm::vec3(0.0f, 15.0f, 0.0f);
	//pSystem->p.push_back(phyPyramid);
	//pSystem->init();

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
}

float x = 0.0f; // location of triangle on x axis
float v_i = 1.0f; // offset for moving the triangle
float v = v_i;
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
	x += v * elapsed; // move the triangle along x axis
	if (x > 1.0f) v = -v_i; // switch to moving the triangle to the left
	if (x < -1.0f) v = v_i; // switch to moving the triangle to the right
	///////////////
	camera.update(window);
	
	//
	EulerStep(pSystem, elapsed);
	//onePyramid.mPos = phyPyramid->x;
	gameObject->consumeInput();
	pWorld->runSimulation(elapsed);
	gameObject->update(currentTime, &camera, renderingProgram);
	///////////////
	
	//onePyramid.update(currentTime, &camera, renderingProgram);

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