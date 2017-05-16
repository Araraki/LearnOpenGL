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
GLuint boxTexture, specTexture;
GLfloat currentTime, deltaTime, lastFrame;
Shader baseShader, lampShader;

#pragma region Resources
glm::vec3 dirLight = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::vec3 lampPosition[] = {
	glm::vec3(-2.0f,  2.0f,  3.0f),
	glm::vec3(-2.0f, -2.0f, -3.0f),
	glm::vec3(1.5f, -1.0f,  3.0f),
	glm::vec3(-1.5f,  1.0f, -2.3f)
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
			// Positions         // Texture Coords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
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
}
#pragma endregion 

void RenderScene(Shader& shader)
{
	glm::mat4 model = glm::mat4();

	// cube
	model = glm::mat4();
	model = glm::rotate(model, glm::radians(lastFrame) * 20, glm::vec3(1.0f, 0.2f, 0.5f));
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boxTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specTexture);

	RenderCube();

	// lamp
	lampShader.Use();

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
	RenderCube();
	
	glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);
	for (int i = 1; i < 4; ++i)
	{
		model = glm::mat4();
		model = glm::translate(model, lampPosition[i]);
		model = glm::scale(model, glm::vec3(0.2f));
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
	glEnable(GL_CULL_FACE);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// shader
	baseShader = Shader("base.vert", "base.frag");
	lampShader = Shader("lamp.vert", "lamp.frag");

	baseShader.Use();
	glUniform1i(glGetUniformLocation(baseShader.Program, "diffuse"), 0);
	glUniform1i(glGetUniformLocation(baseShader.Program, "specular"), 1);

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
	boxTexture = TextureManager::Inst()->LoadTexture("box.png", GL_BGRA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	specTexture = TextureManager::Inst()->LoadTexture("box_specular.png", GL_BGRA, GL_RGBA, 0, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	currentTime = deltaTime = lastFrame = 0.0f;
	// main loop
	while (!glfwWindowShouldClose(window))
	{
		currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		
		// clean screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// check and call events
		glfwPollEvents();
		keysProcess();

		// update Projection/View matrix
		glm::mat4 proj = glm::perspective(camera.Zoom, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof glm::mat4, glm::value_ptr(proj));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof glm::mat4, sizeof glm::mat4, glm::value_ptr(view));

		baseShader.Use();
		RenderScene(baseShader);
		
		glfwSwapBuffers(window);
	}

	deleteObjects();
	glfwTerminate();

	return 0;
}
