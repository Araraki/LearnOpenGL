#define GLEW_STATIC
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// res
GLfloat cubeVertices[] = {
	// Positions          // Normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void keysProcess();

// opengl
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

	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLuint cubeVAO, cubeVBO;
void initVAOandVBO()
{
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GL_FLOAT, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GL_FLOAT, (GLvoid*)(3 * sizeof GL_FLOAT));

	glBindVertexArray(0);
}

void deleteVAOandVBO()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
}

Shader baseShader;
void loadResources()
{
	baseShader = Shader("base.vert", "base.frag");
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::mat4 model, view, proj;
void DrawScene()
{
	view = glm::mat4();
	proj = glm::mat4();
	model = glm::mat4();

	view = camera.GetViewMatrix();
	proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
	
	baseShader.Use();
	
	glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	
	// shader.viewPos
	glUniform3f(glGetUniformLocation(baseShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	// shader.dirLight
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
	// shader.pointLights
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].position"), -2.0f, 2.0f, 3.0f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[0].quadratic"), 0.032);

	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].position"), -2.0f, -2.0f, -3.0f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[1].linear"), 0.09);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[1].quadratic"), 0.032);

	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].position"), 1.5f, -1.0f, 3.0f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[2].linear"), 0.09);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[2].quadratic"), 0.032);

	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].position"), -1.5f, 1.0f, -2.3f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[3].linear"), 0.09);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[3].quadratic"), 0.032);
	// shader.spotLight
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.spotDir"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.cutOff"), glm::radians(12.5f));
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.outerCutOff"), glm::radians(17.5f));
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.quadratic"), 0.032f);
	// shader.material
	glUniform1i(glGetUniformLocation(baseShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(baseShader.Program, "material.specular"), 1);
	glUniform1f(glGetUniformLocation(baseShader.Program, "material.shininess"), 64.0f);
	
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

GLfloat currentTime = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;
int main(int argc, char* argv[])
{
	glInit();

	initVAOandVBO();

	loadResources();

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