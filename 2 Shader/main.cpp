#include <sstream>
#include <fstream>
#include <iostream>

// GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>

// GLAD
#include <glad/glad.h> 
#include <glfw\glfw3.h>

#include "Shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
/*
GLchar* vertexCode;
GLchar* fragmentCode;

char* filetobuf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr = fopen(file, "rb");						// open 文件
	if (!fptr)	return nullptr;						// 失败返回空
	fseek(fptr, 0, SEEK_END);						// seek 到文件结尾
	length = ftell(fptr);							// 取得文件的总 bytes 长
	buf = static_cast<char*>(malloc(length + 1));	// 为 buffer malloc 空间(注意要+1  最后的'\0')
	fseek(fptr, 0, SEEK_SET);						// seek 到文件开头
	fread(buf, length, 1, fptr);					// 读内容到 buffer
	fclose(fptr);									// close 文件
	buf[length] = 0;								// 最后的 '\0'
	return buf;										// 返回 buffer
}
*/
int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	std::cout << "Failed to initialize GLEW" << std::endl;
	//	return -1;
	//}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// ---main code---

	// Shader (compile->link->delete)
	/*
	GLuint vertexShader, fragmentShader, shaderProgram;

	// Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	vertexCode = filetobuf("vertex.glsl");
	glShaderSource(vertexShader, 1, &vertexCode, nullptr);
	glCompileShader(vertexShader);
	// is vertexShader compile success?
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentCode = filetobuf("fragment.glsl");
	glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
	glCompileShader(fragmentShader);
	// is fragmentShader compile success?
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// is shaderProgram link success?
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	*/
	Shader ourShader("default.vs", "default.frag");

	// Vertex Input
	GLfloat vertices[] =
	{	// vertex			color
		 0.0f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	GLuint indices[] =
	{
		0, 1, 2
	};
	// 顶点缓冲对象（vertex buffer objects, VBO)
	// 顶点数组对象（Vertex Array Object, VAO）
	GLuint VBO, VAO, EBO;

	// 1. 产生并绑定VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. 产生并绑定VBO
	//	  产生并绑定EBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// 3. 复制顶点数组到VBO中(提供给OpenGL使用)
	//	  复制索引数组到索引缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. 给对应的顶点属性数组指定数据
	//	  定义存放顶点属性数据的数组
	
	// 参数：要配置哪一个顶点属性, 顶点属性的大小, 数据的类型, 是否希望数据被标准化, 步长（stride,连续的顶点属性之间间隔有多少), 位置数据在缓冲中起始位置的偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
	glEnableVertexAttribArray(0); // 指定了需要启用的顶点属性数组的索引

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // 指定了需要启用的顶点属性数组的索引
								  // 5. 解绑VBO、VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// ---main loop---
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

//		glUseProgram(shaderProgram);
		ourShader.Use();
		// 6. 进行渲染的时候，绑定对应的VAO
//		GLfloat timeValue = glfwGetTime();
//		GLfloat greenValue = (sin(timeValue) / 2) + 0.5f;
//		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// 7. 使用完毕之后清除绑定
		glBindVertexArray(0);
		// ---loop end---

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VBO);
	glDeleteBuffers(1, &VBO);

	// ---code end---

	glfwTerminate();
	return 0;
}
