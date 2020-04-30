#pragma once

#ifndef __CAMERA_H
#define __CAMERA_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::mat4 viewMatrix;		//Position matrix, where camera will be positioned
	glm::mat4 projectionMatrix;	//Perspective matrix

	glm::vec3 cameraPos;		//Where the camera is positioned in space
	glm::vec3 cameraFace;		//Where the camera is looking
	glm::vec3 faceForward;		//


	float yaw;
	float pitch;
	float lastX;
	float lastY;
	bool firstMouse;

public:
	Camera();
	void calculateView(GLuint shader);
	void mouseCallback(double xpos, double ypos);
	void moveCamera(int direction, double delta);
	glm::vec3 getCameraPosition();
	void setCameraPosition(glm::vec3);
	glm::vec3 getCameraFace();

};

#endif
