//#define GLEW_STATIC
//#include <gl/glew.h>
#include <glad/glad.h>
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

// transform Matrix
glm::mat4 model, view, proj;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// pointlight
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.3f, -1.6f, -3.0f),
	glm::vec3(-1.7f, 0.9f, 1.0f)
};

// --- main code ---
GLFWwindow* window;
int screenWidth, screenHeight;
GLfloat currentTime = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;

int main(int argc, char* argv[])
{
	// opengl Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glewExperimental = GL_TRUE;
	//glewInit();
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	// Ïß¿ò»òÌî³ä
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader modelBaseShader = Shader("modelBase.vert", "modelBase.frag");
	Shader modelNormalShader = Shader("modelNormal.vert", "modelNormal.frag", "modelNormal.geom");
	
	Model ourModel = Model("Nanosuit/nanosuit.obj");

	while (!glfwWindowShouldClose(window))
	{
		// calculate deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// Start Draw
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keysProcess();

		modelBaseShader.Use();

		view = glm::mat4(1.0f);
		proj = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		glUniformMatrix4fv(glGetUniformLocation(modelBaseShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(modelBaseShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(modelBaseShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].linear"), 0.009);
		glUniform1f(glGetUniformLocation(modelBaseShader.Program, "pointLights[0].quadratic"), 0.0032);

		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].linear"), 0.009);
		glUniform1f(glGetUniformLocation(modelBaseShader.Program, "pointLights[1].quadratic"), 0.0032);

		//glUniform1f(glGetUniformLocation(modelAnim.Program, "time"), glfwGetTime());

		ourModel.Draw(modelBaseShader);


		modelNormalShader.Use();

		glUniformMatrix4fv(glGetUniformLocation(modelNormalShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(modelNormalShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(modelNormalShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		ourModel.Draw(modelNormalShader);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//FreeImage_DeInitialise();
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
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

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

// geometryBase
/*
#define GLEW_STATIC
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// resources
GLfloat points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // Bottom-left
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* window;
int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);
	
	Shader baseShader = Shader("geometryBase.vert", "geometryBase.frag", "geometryBase.geom");

	GLuint VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof points, &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof GLfloat, (GLvoid*)(2*sizeof GLfloat));
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		baseShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
*/