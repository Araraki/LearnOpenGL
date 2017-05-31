#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "TextureManager.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) :VAO(this->_VAO)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		this->setupMesh();
	}
	Mesh(const Mesh& src) :VAO(this->_VAO)
	{
		this->vertices = src.vertices;
		this->indices = src.indices;
		this->textures = src.textures;
		
		this->_VAO = src._VAO;
		this->_VBO = src._VBO;
		this->_EBO = src._EBO;
		this->VAO = src._VAO;
	}
	Mesh& Mesh::operator=(const Mesh& src)
	{
		this->vertices = src.vertices;
		this->indices = src.indices;
		this->textures = src.textures;
		
		this->_VAO = src._VAO;
		this->_VBO = src._VBO;
		this->_EBO = src._EBO;
		this->VAO = src._VAO;

		return *this;
	}

	void Draw(Shader shader)
	{
		GLuint diffuseNr = 1, specularNr = 1, reflectionNr = 1;

		for (GLuint i = 0; i < this->textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::stringstream ss;
			std::string number;
			std::string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++;
			else if (name == "texture_specular")
				ss << specularNr++;
			else if (name == "texture_reflection")
				ss << reflectionNr++;
			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
			TextureManager::Inst()->BindTexture(this->textures[i].id);
		}

		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);

		glBindVertexArray(this->_VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	GLuint& VAO;
private:
	GLuint _VAO, _VBO, _EBO;
	void setupMesh()
	{
		glGenVertexArrays(1, &this->_VAO);
		glGenBuffers(1, &this->_VBO);
		glGenBuffers(1, &this->_EBO);

		glBindVertexArray(this->_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};
