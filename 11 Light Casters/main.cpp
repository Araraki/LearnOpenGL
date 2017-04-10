#define GLEW_STATIC
#include <gl/glew.h>
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
	// Positions           // Normals           // Texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

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

	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// 线框或填充
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// VAO & VBO
GLuint modelVAO, modelVBO;
GLuint lampVAO;

// transform
glm::mat4 model, view, proj;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// model
Shader baselightingShader;

// lamp
Shader lampShader;
glm::vec3 lampPos;
glm::vec4 lightVector;
glm::vec3 spotDir;
float cutOff, outerCutOff;

// --- main code ---
GLfloat currentTime = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;

int main(int argc, char* argv[])
{	
	glInit();

	// shader 
	baselightingShader = Shader("baselighting.vs", "baselighting.frag");
	lampShader = Shader("lamp.vs", "lamp.frag");

	// model VAO/VBO
	glGenVertexArrays(1, &modelVAO);
	glGenBuffers(1, &modelVBO);

	glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(modelVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
	
	// light VAO/VBO
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// texture
	TextureManager::Inst()->LoadTexture("box.png", 0, GL_BGRA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	TextureManager::Inst()->LoadTexture("box_specular.png", 1, GL_BGRA, GL_RGBA, 0, 0);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keysProcess();		

		// 光源设置
		// lightVector.w == 0.0 平行光	xyz为平行光方向
		// lightVector.w == 0.5 聚光灯	xyz为聚光灯position
		// lightVector.w == 1.0 点光源	xyz为点光源position
		
		//平行光
		//lightVector = glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f);
		//lampPos = -lightVector;							 
		
		//点光源
		//lightVector = glm::vec4(1.2f, 1.0f, sin(lastFrame)*4-8.0f, 1.0f);
		//lampPos = lightVector;
		
		//聚光灯
		lightVector = glm::vec4(camera.Position.x, camera.Position.y, camera.Position.z, 0.5f);
		lampPos = glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z) - 0.1f*camera.Front;
		spotDir = glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z);
		cutOff = glm::cos(glm::radians(12.5f));
		outerCutOff = glm::cos(glm::radians(17.5f));
		// 观察/投影矩阵配置
		view = glm::mat4();
		proj = glm::mat4();
		view = camera.GetViewMatrix();
		proj = glm::perspective(camera.Zoom, float(screenWidth) / float(screenHeight), 0.1f, 100.0f);

		// cube 绘制前配置
		baselightingShader.Use();

		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->BindTexture(0);
		glActiveTexture(GL_TEXTURE1);
		TextureManager::Inst()->BindTexture(1);

		// transform
		glUniformMatrix4fv(glGetUniformLocation(baselightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(baselightingShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		// shader.lightingCalculate
		glUniform3f(glGetUniformLocation(baselightingShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform4f(glGetUniformLocation(baselightingShader.Program, "light.lightVector"), lightVector.x, lightVector.y, lightVector.z, lightVector.w);

		glUniform3f(glGetUniformLocation(baselightingShader.Program, "light.spotDir"), spotDir.x, spotDir.y, spotDir.z);
		glUniform1f(glGetUniformLocation(baselightingShader.Program, "light.cutOff"), cutOff);		
		glUniform1f(glGetUniformLocation(baselightingShader.Program, "light.outerCutOff"), outerCutOff);

		glUniform1f(glGetUniformLocation(baselightingShader.Program, "light.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(baselightingShader.Program, "light.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(baselightingShader.Program, "light.quadratic"), 0.032f);
				
		//shader.lightParameter		
		glUniform3f(glGetUniformLocation(baselightingShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(baselightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(baselightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

		// shader.material
		glUniform1i(glGetUniformLocation(baselightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(baselightingShader.Program, "material.specular"), 1);
		glUniform1i(glGetUniformLocation(baselightingShader.Program, "material.emission"), 2);
		glUniform1f(glGetUniformLocation(baselightingShader.Program, "material.shininess"), 64.0f);
		
		// 绘制 cube
		glBindVertexArray(modelVAO);
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (glm::radians(lastFrame) * 20) + i, glm::vec3(1.0f, 0.5f, 0.2f));
			glUniformMatrix4fv(glGetUniformLocation(baselightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		
		// lamp 绘制前配置
		lampShader.Use();

		model = glm::mat4();
		model = glm::translate(model, lampPos);
		model = glm::scale(model, glm::vec3(0.2f));

		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view") , 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);

		// 绘制 lamp
		glBindVertexArray(lampVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lampVAO);
	glDeleteVertexArrays(1, &modelVAO);
	glDeleteBuffers(1, &modelVBO);
	FreeImage_DeInitialise();
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
