//#define GLEW_STATIC
//#include <gl/glew.h>
#include <glad/glad.h>
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
	// Positions          // Normals          // Texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

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
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

GLfloat cubePosition[] = {
	// Positions        
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

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
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,

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

	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,

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
	0.0f,  1.0f,  0.0f,

};
GLfloat cubeTexCoords[] = {
	// Texture Coords
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

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
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

glm::vec3 dirLight = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::vec3 lampPosition[] = {
	glm::vec3(-2.0f,  2.0f,  3.0f),
	glm::vec3(-2.0f, -2.0f, -3.0f),
	glm::vec3( 1.5f, -1.0f,  3.0f),
	glm::vec3(-1.5f,  1.0f, -2.3f)
};

// opengl
void glInit();
void initVAOandVBO();
void deleteVAOandVBO();
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

	//glewExperimental = GL_TRUE;
	//glewInit();
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLuint cubeVAO, cubeVBO, lampVAO, lampVBO;
void initVAOandVBO()
{
	// cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	// method 0£º
/*
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);
*/

	// method 1£º use glBufferSubData() to write glBuffer
/*
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof cubeVertices, cubeVertices);   // it's not good :(
*/
// or
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof cubePosition, cubePosition);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof cubePosition, sizeof cubeNormal, cubeNormal);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof cubePosition + sizeof cubeNormal, sizeof cubeTexCoords, cubeTexCoords);
	
	// method 2£º write glBuffer with ptr and memcpy
/*
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, nullptr, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, nullptr, GL_STATIC_DRAW);
	GLvoid* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, cubeVertices, sizeof cubeVertices);
	glUnmapBuffer(GL_ARRAY_BUFFER);
*/
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GL_FLOAT, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GL_FLOAT, (GLvoid*)(3 * sizeof GL_FLOAT));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof GL_FLOAT, (GLvoid*)(6 * sizeof GL_FLOAT));
*/
// for glBufferSubData
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof GLfloat, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof GLfloat, (GLvoid*)sizeof cubePosition);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof GLfloat, (GLvoid*)(sizeof cubePosition + sizeof cubeNormal));
	
	glBindVertexArray(0);

	// lamp
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);
	
	//glBindBuffer(GL_COPY_READ_BUFFER, cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindBuffer(GL_COPY_WRITE_BUFFER, lampVBO);

	//glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof cubePosition);
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof cubePosition);

	glBindVertexArray(lampVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof GLfloat, nullptr);
	
	glBindVertexArray(0);
}

void deleteVAOandVBO()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lampVAO);

	glDeleteBuffers(1, &lampVBO);
	glDeleteBuffers(1, &cubeVBO);
}

Shader baseShader, lampShader;
GLuint boxTexture, specTexture;
void loadResources()
{
	baseShader = Shader("base.vert", "base.frag");
	lampShader = Shader("lamp.vert", "lamp.frag");

	boxTexture = TextureManager::Inst()->LoadTexture("box.png", GL_RGBA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	specTexture = TextureManager::Inst()->LoadTexture("box_specular.png", GL_RGBA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

glm::mat4 model, view, proj;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
void DrawScene()
{
	view = glm::mat4();
	proj = glm::mat4();
	model = glm::mat4();

	view = camera.GetViewMatrix();
	proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);
	model = glm::rotate(model, glm::radians(lastFrame) * 20, glm::vec3(1.0f, 0.2f, 0.5f));

	baseShader.Use();

	glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(baseShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// shader.viewPos
	glUniform3f(glGetUniformLocation(baseShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	// shader.dirLight
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.direction"), dirLight.x, dirLight.y, dirLight.z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

	// shader.pointLights
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].position"), lampPosition[0].x, lampPosition[0].y, lampPosition[0].z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[0].quadratic"), 0.032f);

	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].position"), lampPosition[1].x, lampPosition[1].y, lampPosition[1].z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[1].quadratic"), 0.032f);

	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].position"), lampPosition[2].x, lampPosition[2].y, lampPosition[2].z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[2].quadratic"), 0.032f);

	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].position"), lampPosition[3].x, lampPosition[3].y, lampPosition[3].z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(baseShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(baseShader.Program, "pointLights[3].quadratic"), 0.032f);

	// shader.spotLight
	glm::vec3 lampPos = glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z) - 0.1f*camera.Front;
	glm::vec3 spotDir = glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.position"), lampPos.x, lampPos.y, lampPos.z);
	glUniform3f(glGetUniformLocation(baseShader.Program, "spotLight.spotDir"), spotDir.x, spotDir.y, spotDir.z);
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(baseShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));
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
	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(boxTexture);
	glActiveTexture(GL_TEXTURE1);
	TextureManager::Inst()->BindTexture(specTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	// lamp
	lampShader.Use();

	glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	glBindVertexArray(lampVAO);

	model = glm::mat4();
	model = glm::translate(model, -dirLight*5.0f);
	model = glm::scale(model, glm::vec3(0.2f));
	// rotate
	float xz = acos(glm::dot(glm::normalize(glm::vec3(dirLight.x, 0.0f, dirLight.z)), glm::vec3(0.0f, 0.0f, 1.0f)));
	model = glm::rotate(model, xz, glm::vec3(0.0f, 1.0f, 0.0f));

	float yz = acos(glm::dot(glm::normalize(glm::vec3(0.0f, dirLight.y, dirLight.z)), glm::vec3(0.0f, 0.0f, 1.0f)));
	model = glm::rotate(model, yz, glm::vec3(1.0f, 0.0f, 0.0f));

	glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 0.2f, 0.2f);
	glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	for(int i = 1; i < 4 ; ++i)
	{
		model = glm::mat4();
		model = glm::translate(model, lampPosition[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		
		glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
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