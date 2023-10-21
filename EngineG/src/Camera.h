#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:

	Camera();

	void update(GLFWwindow* window);
	const glm::mat4& getViewMatrix() { return vMat; };
	const glm::mat4& getProjectionMatrix() { return pMat; };

private:
	float cameraX;
	float cameraY;
	float cameraZ;
	int width;
	int height;
	float aspect;

	glm::mat4 pMat;
	glm::mat4 vMat;
};