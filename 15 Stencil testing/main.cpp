//#define GLEW_STATIC
//#include <gl/glew.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "TextureManager.h"

const GLuint WIDTH = 800, HEIGHT = 600;

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
GLfloat planeVertices[] = {
	// Positions          // Texture Coords 
	5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
};

// Input
void keysProcess();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// opengl
GLFWwindow* window;
int screenWidth, screenHeight;

void glInit ()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	//glewInit();
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// Ïß¿ò»òÌî³ä
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// VAO & VBO
GLuint cubeVAO, cubeVBO,
	   planeVAO, planeVBO;

// transform
glm::mat4 model, view, proj;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

Shader baseShader, outliningShader;

// --- main code ---
GLfloat currentTime = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;

int main(int argc, char* argv[])
{	
	glInit();

	// shader 
	baseShader = Shader("baselighting.vs", "baselighting.frag");
	outliningShader = Shader("outlining.vs", "outlining.frag");

	// cube VAO/VBO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	// plane VAO/VBO
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glBindVertexArray(planeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// texture
	GLuint tex1 = TextureManager::Inst()->LoadTexture("box.png", GL_RGBA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	while (!glfwWindowShouldClose(window))
	{
		// calculate deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// Start Draw
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		keysProcess();		

		// matrix
		view = glm::mat4();
		proj = glm::mat4();
		view = camera.GetViewMatrix();
		proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

		// 1.plane
		glStencilMask(0x00);

		baseShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(planeVAO);
		TextureManager::Inst()->BindTexture(tex1);
		model = glm::mat4();
		glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		// 2.cube
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilMask(0xff);
		baseShader.Use();

		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->BindTexture(tex1);

		glBindVertexArray(cubeVAO);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);

		// 3.outlining
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);	//glStencilFunc(GL_NOTEQUAL, 5, 0x03);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outliningShader.Use();
		glBindVertexArray(cubeVAO);
		glUniformMatrix4fv(glGetUniformLocation(outliningShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(outliningShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		GLfloat scale = 1.1f;
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale));
		glUniformMatrix4fv(glGetUniformLocation(outliningShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale));
		glUniformMatrix4fv(glGetUniformLocation(outliningShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);
		
		glStencilMask(0xff);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &cubeVBO);

	//FreeImage_DeInitialise();
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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
