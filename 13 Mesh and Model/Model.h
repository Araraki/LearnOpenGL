#pragma once
#include <string>

#include <gl/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "TextureManager.h"


class Model
{
public:

private:
	string directory;
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;

};

