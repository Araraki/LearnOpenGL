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
GLfloat vertices[] =
{	// vertex				color					texture coords
	0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,		// 右上
	0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// 右下
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		// 左下
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f		// 左上
};

GLuint indices[] =
{
	0, 1, 3,
	1, 2, 3
};

Shader ourShader;
float blend = 0.0f;

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

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	FreeImage_Initialise(TRUE);

	// ---main code---

	// Shader (compile->link->delete)
	ourShader = Shader("default.vs", "default.frag");

	// 顶点缓冲对象（vertex buffer objects, VBO)
	// 顶点数组对象（Vertex Array Object, VAO）
	// 索引缓冲对象 (Element Buffer Object)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// VAO
	glBindVertexArray(VAO);

	// VBO		// 复制顶点数组到VBO中(提供给OpenGL使用)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// EBO		// 复制索引数组到索引缓冲中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
																				  
	// 定义存放顶点属性数据的数组
	// 参数：要配置哪一个顶点属性, 顶点属性的大小, 数据的类型, 是否希望数据被标准化, 步长（stride,连续的顶点属性之间间隔有多少), 位置数据在缓冲中起始位置的偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));
	glEnableVertexAttribArray(0); // 指定了需要启用的顶点属性数组的索引

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	// 解绑VAO
	glBindVertexArray(0);
	// 线框或填充
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Texture
	GLuint texture, texture2;
	FIBITMAP* bitmap;
	unsigned char* bits;
	int w, h;
	const char *image = "ok.jpeg",
			   *image2 = "happy.jpeg";
	// texture 1
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	
	bitmap = FreeImage_Load(FreeImage_GetFileType(image), image, JPEG_DEFAULT);
	bits = FreeImage_GetBits(bitmap);
	w = FreeImage_GetWidth(bitmap);
	h = FreeImage_GetHeight(bitmap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(bitmap);
	glBindTexture(GL_TEXTURE_2D, 0);

	// texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	bitmap = FreeImage_Load(FreeImage_GetFileType(image2), image2, JPEG_DEFAULT);
	bits = FreeImage_GetBits(bitmap);
	w = FreeImage_GetWidth(bitmap);
	h = FreeImage_GetHeight(bitmap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,	GL_BGR, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(bitmap);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glUniform1f(glGetUniformLocation(ourShader.Program, "blend"), 0.0f);

	// Transform
	//	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//	glm::mat4 trans;
	//	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	//	vec = trans * vec;
	//	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// ---main loop---
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();

		// use Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		glUniform1f(glGetUniformLocation(ourShader.Program, "blend"), blend);
		
		// 进行渲染的时候，绑定对应的VAO
		glBindVertexArray(VAO);

		// use Transform
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime()*1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		
		// use Transform
		trans = glm::mat4();
		trans = glm::translate(trans, glm::vec3(-0.5, 0.5, 0.0f));
		GLfloat scale = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));
		GLuint transformLoc2 = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		// 使用完毕之后清除绑定
		glBindVertexArray(0);
		// ---loop end---

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// ---code end---

	FreeImage_DeInitialise();
	glfwTerminate();

	return 0;
}