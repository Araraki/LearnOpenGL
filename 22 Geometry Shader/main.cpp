#define GLEW_STATIC
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "TextureManager.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// resources
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

GLfloat cubePosition[] = {
	// Positions        
	-0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f
};
GLfloat cubeNormal[] = {
	// Normals          
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,

	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,
	0.0f,  0.0f,  1.0f,

	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,

	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f
};
GLfloat cubeTexCoords[] = {
	// Texture Coords
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

glm::vec3 dirLight = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::vec3 lampPosition[] = {
	glm::vec3(-2.0f,  2.0f,  3.0f),
	glm::vec3(-2.0f, -2.0f, -3.0f),
	glm::vec3(1.5f, -1.0f,  3.0f),
	glm::vec3(-1.5f,  1.0f, -2.3f)
};

// opengl
void glInit();
void initVAOandVBO();
void deleteVAOandVBO();
void loadShader();
void DrawScene();

// resources
void loadResources();

// input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void keysProcess();

GLFWwindow* window;
GLfloat currentTime, deltaTime, lastFrame;
int main(int argc, char* argv[])
{
	glInit();

	initVAOandVBO();

	loadShader();

	loadResources();

	currentTime = deltaTime = lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keysProcess();

		DrawScene();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	deleteVAOandVBO();

	glfwTerminate();

	return 0;
}

// opengl
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

	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_PROGRAM_POINT_SIZE);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void initVAOandVBO()
{
	
}

void deleteVAOandVBO()
{

}

Shader baseShader, lampShader;
GLuint uboMatrices;
void loadShader()
{
	baseShader = Shader("base.vert", "base.frag");
	lampShader = Shader("lamp.vert", "lamp.frag");

	glUniformBlockBinding(baseShader.Program, glGetUniformBlockIndex(baseShader.Program, "Matrices"), 0);
	glUniformBlockBinding(lampShader.Program, glGetUniformBlockIndex(lampShader.Program, "Matrices"), 0);

	// (proj/view) ubo
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof glm::mat4, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof glm::mat4);
}

void loadResources()
{

}

glm::mat4 model, view, proj;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
void DrawScene()
{
	proj = glm::mat4();
	view = glm::mat4();

	proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof glm::mat4, glm::value_ptr(proj));

	view = camera.GetViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof glm::mat4, sizeof glm::mat4, glm::value_ptr(view));


}

// input
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
}