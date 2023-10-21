#include "Camera.h"

Camera::Camera()
	: cameraX(0.0f), cameraY(30.0f), cameraZ(60.0f)
{	
}

void Camera::update(GLFWwindow* window)
{
	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// build view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
}

