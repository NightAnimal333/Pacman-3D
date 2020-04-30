#include "camera.h"

Camera::Camera() {

	this->yaw = -90.f;
	this->pitch = 0.f;
	this->lastX = 780 / 2;
	this->lastY = 600 / 2;
	this->firstMouse = true;

	this->cameraPos = glm::vec3(1.5f, 0.0f, 17.5f);
	this->cameraFace = glm::vec3(0.0f, 0.0f, -1.0f);

	this->projectionMatrix = glm::perspective(
		glm::radians(45.f),	// FOV
		800.0f / 640.0f,	// Aspect Ratio
		0.1f,				// Near clipping plane
		100.0f				// Far clipping plane
	);


}

void Camera::calculateView(GLuint shader) {
	viewMatrix = glm::lookAt(
		cameraPos,						// the position of your camera, in world space
		cameraPos + cameraFace,			// where you want to look at, in world space
		glm::vec3(0.f, 1.f, 0.f)		// probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
);

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(viewMatrix));

	glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, glm::value_ptr(cameraPos));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(shader, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

}


void Camera::moveCamera(int direction, double delta)
{
	float speedNS = 6.f;
	float speedEW = 1.2f;
	float dt = delta;

	if (direction == 0) {
		cameraPos.x += (speedNS * dt) * faceForward.x;
		cameraPos.z += (speedNS * dt) * faceForward.z;
	}
	else if (direction == 2) {
		cameraPos.z -= (speedNS * dt) * faceForward.z;
		cameraPos.x -= (speedNS * dt) * faceForward.x;
	}

	if (direction == 1) {
		cameraPos -= glm::normalize(glm::cross(faceForward, glm::vec3(0.0, 1.0, 0.0))) * (speedEW * dt);
	}
	else if (direction == 3) {
		cameraPos += glm::normalize(glm::cross(faceForward, glm::vec3(0.0, 1.0, 0.0))) * (speedEW * dt);
	}
}

void Camera::mouseCallback(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.f) {
		pitch = 89.f;
	}
	if (pitch < -89.f) {
		pitch = -89.f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw));

	cameraFace = (glm::normalize(direction));
	faceForward = (cameraFace * glm::vec3(0.3f));
}

glm::vec3 Camera::getCameraPosition(){

	return this->cameraPos;

}

void Camera::setCameraPosition(glm::vec3 spawnPosition){

	this->cameraPos = spawnPosition;

}

glm::vec3 Camera::getCameraFace() {

	return this->faceForward;

}
