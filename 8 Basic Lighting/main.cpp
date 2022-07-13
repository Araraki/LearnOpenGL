//#define GLEW_STATIC
//#include <gl/glew.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// Resources
// vertices / normal vector
GLfloat cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// �߿�����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// VAO & VBO
GLuint cubeVAO, cubeVBO;
GLuint planeVAO;

// Shader
Shader depthShader;
Shader lampShader;

// transform
glm::mat4 model, view, proj;
GLuint modelLoc, viewLoc, projLoc;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// light
GLint objectColorLoc, lampColorLoc, lampPosLoc, viewPosLoc;
glm::vec3 lampPos(1.2f, 1.0f, 2.0f);

// --- main code ---
GLfloat currentTime = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;

int main(int argc, char* argv[])
{	
	glInit();

	// shader 
	depthShader = Shader("baselighting.vs", "baselighting.frag");
	lampShader = Shader("lamp.vs", "lamp.frag");

	// model VAO/VBO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	
	// light VAO/VBO
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// calculate deltaTime
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		//std::cout << int(1.0f/deltaTime) << " FPS" << std::endl;

		// Start Draw
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keysProcess();

		// �ƶ���Դ
		lampPos = glm::vec3(1.0f + sin(lastFrame) * 2.0f, sin(lastFrame / 2.0f) * 1.0f, 2.0f);

		// cube ����ǰ����
		depthShader.Use();

		view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		proj = glm::mat4(1.0f);
		proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

		viewLoc = glGetUniformLocation(depthShader.Program, "view");
		projLoc = glGetUniformLocation(depthShader.Program, "proj");
		viewPosLoc = glGetUniformLocation(depthShader.Program, "viewPos");
		lampPosLoc = glGetUniformLocation(depthShader.Program, "lightPos");
		lampColorLoc = glGetUniformLocation(depthShader.Program, "lightColor");
		objectColorLoc = glGetUniformLocation(depthShader.Program, "objectColor");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lampPosLoc, lampPos.x, lampPos.y, lampPos.z);
		glUniform3f(lampColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.3f);

		
		// ���� cube
		glBindVertexArray(cubeVAO);
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(GLfloat(glfwGetTime()) * 20.0f) + i, glm::vec3(1.0f, 0.3f, 0.5f));
			modelLoc = glGetUniformLocation(depthShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// lamp ����ǰ����
		lampShader.Use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lampPos);
		model = glm::scale(model, glm::vec3(0.2f));

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "proj");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// ���� lamp
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
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
	
	if (keys[GLFW_KEY_L])
		camera.FPS_Camera = !camera.FPS_Camera;
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
}
