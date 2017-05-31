#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoords;

layout (std140) uniform Matrices
{
uniform	mat4 proj;
uniform	mat4 view;
};

uniform mat4 model;


void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
}