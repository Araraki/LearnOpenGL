#pragma once
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const GLfloat YAW			= -90.0f;
const GLfloat PITCH			= 0.0f;
const GLfloat SPEED			= 3.0f;
const GLfloat SENSITIVTY	= 0.05f;
const GLfloat ZOOM			= 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	glm::vec3 WorldUp;

	glm::vec3 fpsFront;
	glm::vec3 fpsRight;

	GLfloat Yaw;
	GLfloat Pitch;

	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;
	
	bool FPS_Camera;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   GLfloat yaw = YAW,
		   GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
		   GLfloat upX, GLfloat upY, GLfloat upZ,
		   GLfloat yaw, GLfloat pitch);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetMyViewMatrix(glm::vec3 pos, glm::vec3 center, glm::vec3 up);
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);
private:
	void updateCameraVectors();
};

