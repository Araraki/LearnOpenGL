#include <iostream>
//#define GLEW_STATIC
//#include <gl\glew.h>
#include <glad\glad.h>
#include <glfw\glfw3.h>

#include "Shader.h"
#include "Sprite.h"

const GLuint WIDHT = 800, HEIGHT = 600;

GLfloat triangleVertices[] =
{	// vertex			color
	0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};
GLfloat cubeVertices[] =
{	// vertex			color
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,	
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
};
GLuint VAO;
GLuint triangleVBO, triangleVAO,
	   cubeVBO, cubeVAO;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		GLfloat tmp = triangleVertices[3];
		triangleVertices[3] = triangleVertices[4];
		triangleVertices[4] = triangleVertices[5];
		triangleVertices[5] = tmp;
		glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof triangleVertices, triangleVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		if (VAO != cubeVAO)
			VAO = cubeVAO;
		else
			VAO = triangleVAO;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDHT, HEIGHT, "Playground", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "GLFWwindow error" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	std::cout << "glewInit error" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDHT, HEIGHT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Shader shader("default.vert", "default.frag");

	Sprite triangle();
	// triangle
	glGenVertexArrays(1, &triangleVAO);
	glGenBuffers(1, &triangleVBO);
	glBindVertexArray(triangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof triangleVertices, triangleVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	VAO = triangleVAO;

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	glDeleteVertexArrays(1, &triangleVAO);
	glDeleteBuffers(1, &triangleVBO);

	glfwTerminate();
	return 0;
}