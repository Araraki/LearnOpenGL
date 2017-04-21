#define GLEW_STATIC
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "TextureManager.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// Input
void keysProcess();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Resources
GLfloat cubeVertices[] = {
	// Positions          // Texture Coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};
struct PointLight
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
PointLight lamp[] = {
	{
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(0.05f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		1.0f, 0.009f, 0.0032f
	},
	{
		glm::vec3(-1.7f, 0.9f, 1.0f),
		glm::vec3(0.05f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		1.0f, 0.009f, 0.0032f
	}
};

// OpenGL
GLFWwindow* window;
int screenWidth, screenHeight;
void glInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);

	// input callback
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	// hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Ïß¿ò»òÌî³ä
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

GLuint lampVAO, lampVBO;
void initVAOandVBO()
{
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);

	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(lampVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, (GLvoid *)(3*sizeof GLfloat));

	glBindVertexArray(0);
}

Shader nanosuitShader, lampShader;
Model nanosuitModel;
void LoadResources()
{
	nanosuitShader = Shader("base.vert", "base.frag");
	lampShader = Shader("lamp.vert", "lamp.frag");

	nanosuitModel = Model("Nanosuit/nanosuit.obj");
}

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// matrix
glm::mat4 model, view, proj;
void DrawScene()
{
	view = glm::mat4();
	proj = glm::mat4();
	model = glm::mat4();
	view = camera.GetViewMatrix();
	proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

	// nanosuit model
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	
	nanosuitShader.Use();

	glUniformMatrix4fv(glGetUniformLocation(nanosuitShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(nanosuitShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(nanosuitShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].position"),	lamp[0].position.x, lamp[0].position.y, lamp[0].position.z);
	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].ambient"),		lamp[0].ambient.x,	lamp[0].ambient.y,	lamp[0].ambient.z);
	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].diffuse"),		lamp[0].diffuse.x,	lamp[0].diffuse.y,	lamp[0].diffuse.z);
	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].specular"),	lamp[0].specular.x, lamp[0].specular.y, lamp[0].specular.z);
	glUniform1f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].constant"),	lamp[0].constant);
	glUniform1f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].linear"),		lamp[0].linear);
	glUniform1f(glGetUniformLocation(nanosuitShader.Program, "pointLights[0].quadratic"),	lamp[0].quadratic);

	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].position"),	lamp[1].position.x, lamp[1].position.y, lamp[1].position.z);
	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].ambient"),		lamp[1].ambient.x,	lamp[1].ambient.y,	lamp[1].ambient.z);
	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].diffuse"),		lamp[1].diffuse.x,	lamp[1].diffuse.y,	lamp[1].diffuse.z);
	glUniform3f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].specular"),	lamp[1].specular.x, lamp[1].specular.y, lamp[1].specular.z);
	glUniform1f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].constant"),	lamp[1].constant);
	glUniform1f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].linear"),		lamp[1].linear);
	glUniform1f(glGetUniformLocation(nanosuitShader.Program, "pointLights[1].quadratic"),	lamp[1].quadratic);

	nanosuitModel.Draw(nanosuitShader);

	// lamp
	glBindVertexArray(lampVAO);
	lampShader.Use();

	glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	for (int i = 0; i < 2; ++i)
	{
		model = glm::mat4();
		model = glm::translate(model, lamp[i].position);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), lamp[i].diffuse.x, lamp[i].diffuse.y, lamp[i].diffuse.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

// --- main code ---
GLfloat currentTime = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;
int main(int argc, char* argv[])
{	
	glInit();
	
	initVAOandVBO();
	
	LoadResources();

	while (!glfwWindowShouldClose(window))
	{
		// calculate deltaTime
		currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// Start Draw
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keysProcess();		

		DrawScene();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &lampVBO);
	glDeleteVertexArrays(1, &lampVAO);

	FreeImage_DeInitialise();
	TextureManager::Inst()->UnloadAllTextures();
	glfwTerminate();

	return 0;
}

// input handle
bool keys[1024];

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
}

bool firstMouse = true;
GLfloat lastX = WIDTH / 2;
GLfloat	lastY = HEIGHT / 2;

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
