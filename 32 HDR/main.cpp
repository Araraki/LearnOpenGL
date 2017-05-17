#define GLEW_STATIC
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "TextureManager.h"

const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow* window;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLuint woodTexture;
GLfloat currentTime, deltaTime, lastFrame;
Shader baseShader, lampShader, hdrShader;
GLfloat exposure;

#pragma region Resources
std::vector<glm::vec3> lightPositions{
	glm::vec3(0.0f, 0.0f, 49.5f),
	glm::vec3(-1.4f, -1.9f, 9.0f),
	glm::vec3(0.0f, -1.8f, 4.0f),
	glm::vec3(0.8f, -1.7f, 6.0f)
};
std::vector<glm::vec3> lightColors{
	glm::vec3(200.0f, 200.0f, 200.0f),
	glm::vec3(0.1f,	0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.2f),
	glm::vec3(0.0f, 0.1f, 0.0f)
};
#pragma endregion 

#pragma region RenderFigure
GLuint cubeVAO = -1, cubeVBO = -1;
GLuint planeVAO = -1, planeVBO = -1;
GLuint quadVAO = -1, quadVBO = -1;
void RenderCube()
{
	if (cubeVAO == -1)
	{
		GLfloat cubeVertices[] = {
			// Positions          // Normals           // Texture Coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
		};

		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLfloat, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLfloat, (GLvoid*)(3 * sizeof GLfloat));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof GLfloat, (GLvoid*)(6 * sizeof GLfloat));
		glBindVertexArray(0);
	}

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void RenderPlane()
{
	if (planeVAO == -1)
	{
		GLfloat groundVertices[] = {
			// Positions			// Normals		   // Texture Coords
			25.0f, -0.5f,  25.0f,	0.0f, 1.0f, 0.0f,  25.0f, 0.0f,
			-25.0f, -0.5f, -25.0f,	0.0f, 1.0f, 0.0f,  0.0f, 25.0f,
			-25.0f, -0.5f,  25.0f,	0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

			25.0f, -0.5f,  25.0f,	0.0f, 1.0f, 0.0f,  25.0f, 0.0f,
			25.0f, -0.5f, -25.0f,	0.0f, 1.0f, 0.0f,  25.0f, 25.0f,
			-25.0f, -0.5f, -25.0f,	0.0f, 1.0f, 0.0f,  0.0f, 25.0f
		};
		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);

		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof groundVertices, groundVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLfloat, nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLfloat, (GLvoid*)(3 * sizeof GLfloat));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof GLfloat, (GLvoid*)(6 * sizeof GLfloat));
		glBindVertexArray(0);
	}

	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
void RenderQuad()
{
	if (quadVAO == -1)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof quadVertices, quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, (GLvoid*)(3 * sizeof GLfloat));
		glBindVertexArray(0);
	}

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void deleteObjects()
{
	if (planeVAO != -1)
	{
		glDeleteVertexArrays(1, &planeVAO);
		glDeleteBuffers(1, &planeVBO);
	}
	if (cubeVAO != -1)
	{
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);
	}
	if (quadVAO != -1)
	{
		glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);
	}
}
#pragma endregion 

#pragma region Input Process
bool firstMouse = true;
GLfloat lastX = SCR_WIDTH / 2;
GLfloat	lastY = SCR_HEIGHT / 2;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = float(xpos);
		lastY = float(ypos);
		firstMouse = false;
	}

	GLfloat xoffset = float(xpos) - lastX;
	GLfloat yoffset = lastY - float(ypos);
	lastX = float(xpos);
	lastY = float(ypos);

	camera.ProcessMouseMovement(xoffset, yoffset);
}

bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		camera.FPS_Camera = !camera.FPS_Camera;
}

void keysProcess()
{
	float cameraSpeed = 5.0f * deltaTime;

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_E])
		camera.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (keys[GLFW_KEY_EQUAL])
		exposure += 0.05f;
	if (keys[GLFW_KEY_MINUS])
		exposure -= 0.05f;
}
#pragma endregion 

void RenderScene(Shader& shader)
{
	shader.Use();

	glm::mat4 model = glm::mat4();

	// cube
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 55.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// shader.viewPos
	glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform1i(glGetUniformLocation(shader.Program, "inverse_normals"), true);
	// shader.pointLights
	for (GLuint i = 0; i < lightPositions.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].position")	.c_str()), 1, &lightPositions[i][0]);
		glUniform3fv(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].color")	.c_str()), 1, &lightColors[i][0]);
	}

	// shader.material
	glUniform1i(glGetUniformLocation(shader.Program, "material.diffuseTex"), 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	RenderCube();

	// lamp
	lampShader.Use();

	for (GLuint i = 0; i < lightPositions.size(); ++i)
	{
		model = glm::mat4();
		model = glm::translate(model, lightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniform3fv(glGetUniformLocation(lampShader.Program, "lampColor"), 1, &lightColors[i][0]);
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		RenderCube();
	}
}

int main(int argc, char* argv[])
{
	// opengl init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// shader
	baseShader = Shader("base.vert", "base.frag");
	lampShader = Shader("lamp.vert", "lamp.frag");
	hdrShader = Shader("hdr.vert", "hdr.frag");

	// (proj/view) ubo
	glUniformBlockBinding(baseShader.Program, glGetUniformBlockIndex(baseShader.Program, "Matrices"), 0);
	glUniformBlockBinding(lampShader.Program, glGetUniformBlockIndex(lampShader.Program, "Matrices"), 0);

	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof glm::mat4, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof glm::mat4);

	// texture
	woodTexture = TextureManager::Inst()->LoadTexture("wood.png", GL_BGRA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// fbo
	GLuint hdrFBO;
	glGenFramebuffers(1, &hdrFBO);

	GLuint colorBuffer;
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// rbo
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

	// attach
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	currentTime = deltaTime = lastFrame = 0.0f;
	exposure = 1.0f;
	// main loop
	while (!glfwWindowShouldClose(window))
	{
		currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		
		// check and call events
		glfwPollEvents();
		keysProcess();

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
			// clean screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// update Projection/View matrix
			glm::mat4 proj = glm::perspective(camera.Zoom, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();

			glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof glm::mat4, glm::value_ptr(proj));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof glm::mat4, sizeof glm::mat4, glm::value_ptr(view));

			RenderScene(baseShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		hdrShader.Use();
		glUniform1i(glGetUniformLocation(hdrShader.Program, "hdrBuffer"), 0);
		glUniform1f(glGetUniformLocation(hdrShader.Program, "exposure"), exposure);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		RenderQuad();

		glfwSwapBuffers(window);
	}

	deleteObjects();
	glfwTerminate();

	return 0;
}
