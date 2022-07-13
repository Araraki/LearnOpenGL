#pragma once
//#include <gl/glew.h>
#include <glad/glad.h>
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

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), FPS_Camera(false)
	{
		this->Position = pos;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), FPS_Camera(false)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() const
	{
		return GetMyViewMatrix(this->Position, this->Position + this->Front, this->Up);
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}
	
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;

		if (FPS_Camera)
		{
			if (direction == FORWARD)
				this->Position += fpsFront * velocity;
			if (direction == BACKWARD)
				this->Position -= fpsFront * velocity;
			if (direction == LEFT)
				this->Position -= fpsRight * velocity;
			if (direction == RIGHT)
				this->Position += fpsRight * velocity;
		}
		else
		{
			if (direction == FORWARD)
				this->Position += this->Front * velocity;
			if (direction == BACKWARD)
				this->Position -= this->Front * velocity;
			if (direction == LEFT)
				this->Position -= this->Right * velocity;
			if (direction == RIGHT)
				this->Position += this->Right * velocity;
		}

		if (direction == UP)
			this->Position += this->Up * velocity;
		if (direction == DOWN)
			this->Position -= this->Up * velocity;
	}

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = GL_TRUE)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;

		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));

		this->Front = glm::normalize(front);
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));

		fpsFront = glm::normalize(glm::vec3(this->Front.x, 0, this->Front.z));
		fpsRight = glm::normalize(glm::vec3(this->Right.x, 0, this->Right.z));
	}
	
	glm::mat4 GetMyViewMatrix(glm::vec3 pos, glm::vec3 center, glm::vec3 up) const
	{
		glm::vec3 zAxis = glm::normalize(pos - center);
		glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(up), zAxis));
		glm::vec3 yAxis = glm::cross(zAxis, xAxis);

		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xAxis.x;
		rotation[1][0] = xAxis.y;
		rotation[2][0] = xAxis.z;
		rotation[0][1] = yAxis.x;
		rotation[1][1] = yAxis.y;
		rotation[2][1] = yAxis.z;
		rotation[0][2] = zAxis.x;
		rotation[1][2] = zAxis.y;
		rotation[2][2] = zAxis.z;

		glm::mat4 translation = glm::mat4(1.0f);
		translation[3][0] = -this->Position.x;
		translation[3][1] = -this->Position.y;
		translation[3][2] = -this->Position.z;

		return rotation * translation;
	}
};

