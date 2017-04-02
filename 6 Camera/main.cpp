#define GLEW_STATIC
#include <gl\glew.h>
#include <glfw\glfw3.h>

#include <FreeImage.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

// Vertex
GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

float blend = 0.5f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
	{
		if (blend >= 1.0f)	return;
		blend += 0.01f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
	{
		if (blend <= 0.0f)	return;
		blend -= 0.01f;
	}
}

GLuint VAO, VBO;
Shader ourShader;
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

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);	

	// 线框或填充
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void vaoInit()
{
	// 顶点数组对象（Vertex Array Object, VAO）
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void vboInit()
{
	// 顶点缓冲对象（vertex buffer objects, VBO)
	glGenBuffers(1, &VBO);
	// 复制顶点数组到VBO中(提供给OpenGL使用)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 定义存放顶点属性数据的数组
	// 参数：要配置哪一个顶点属性, 顶点属性的大小, 数据的类型, 是否希望数据被标准化, 步长（stride,连续的顶点属性之间间隔有多少), 位置数据在缓冲中起始位置的偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
	glEnableVertexAttribArray(0); // 指定了需要启用的顶点属性数组的索引
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

const int texCount = 2;
char* images[texCount] = { "ok.jpeg", "happy.jpeg" };
GLuint textures[texCount];

void textureLoad()
{
	FreeImage_Initialise(TRUE);

	FIBITMAP* bitmap;
	unsigned char* bits;	
	int w, h;

	for (int i = 0; i < texCount; i++)
	{
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		
		bitmap = FreeImage_Load(FreeImage_GetFileType(images[i]), images[i], JPEG_DEFAULT);
		bits = FreeImage_GetBits(bitmap);
		w = FreeImage_GetWidth(bitmap);
		h = FreeImage_GetHeight(bitmap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
		glGenerateMipmap(GL_TEXTURE_2D);

		FreeImage_Unload(bitmap);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void setTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

	glUniform1f(glGetUniformLocation(ourShader.Program, "blend"), blend);
}

glm::mat4 model;
glm::mat4 view;
glm::mat4 proj;

GLuint modelLoc;
GLuint viewLoc;
GLuint projLoc;

void setTransform()
{
	view = glm::mat4();
	proj = glm::mat4();

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	proj = glm::perspective(45.0f, (float)screenWidth / screenHeight, 0.1f, 100.0f);

	modelLoc = glGetUniformLocation(ourShader.Program, "model");
	viewLoc = glGetUniformLocation(ourShader.Program, "view");
	projLoc = glGetUniformLocation(ourShader.Program, "proj");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}

void useTransform(int index)
{
	model = glm::mat4();
	model = glm::translate(model, cubePositions[index]);
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 20.0f * (index + 1)), glm::vec3(1.0f, 0.3f, 0.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

int main(int argc, char* argv[])
{	
	glInit();	

	// ---main code---
	ourShader = Shader("default.vs", "default.frag");
	
	vaoInit();
	vboInit();
	textureLoad();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		setTextures();
		setTransform();

		// 进行渲染的时候，绑定对应的VAO
		glBindVertexArray(VAO);
		
		for (GLuint i = 0; i < 10; ++i)
		{
			useTransform(i);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// 使用完毕之后清除绑定
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
		// ---code end---
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	FreeImage_DeInitialise();
	glfwTerminate();

	return 0;
}