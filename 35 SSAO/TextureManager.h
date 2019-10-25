#pragma once

#include <windows.h>
#include <gl/gl.h>
//#include "FreeImage.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif
#include <vector>
#include <iterator>

class TextureManager
{
public:
	static TextureManager* Inst()
	{
		if (!m_inst)
			m_inst = new TextureManager();
		stbi_set_flip_vertically_on_load(true);
		return m_inst;
	}
	virtual ~TextureManager()
	{
		// call this ONLY when linking with FreeImage as a static library
		//#ifdef FREEIMAGE_LIB
		//FreeImage_DeInitialise();
		//#endif

		UnloadAllTextures();
	}

	/*
	GLuint LoadTexture(const char* filename, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		BYTE* bits(0);
		//image width and height
		unsigned int width(0), height(0);
		//OpenGL's image ID to map to
		GLuint gl_texID;

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return -1;

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if (!dib)
			return -1;

		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		//get the image width and height
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if ((bits == 0) || (width == 0) || (height == 0))
			return -1;

		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		//store the texture ID mapping
		m_texID.push_back(gl_texID);
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);
		//store the texture data for OpenGL use
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
			border, image_format, GL_UNSIGNED_BYTE, bits);

		//Free FreeImage's copy of the data
		FreeImage_Unload(dib);

		//return success
		return gl_texID;
	}
	*/

	GLuint TextureManager::LoadTexture(const char* filename, GLenum image_format, GLint internal_format, GLint level, GLint border)
	{
		int width, height;
		int nrChannels;
		unsigned char* data;
		GLuint gl_texID;

		// load image file
		data = stbi_load(filename, &width, &height, &nrChannels, 0);

		if ((data == 0) || (width == 0) || (height == 0))
			return -1;

		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		//store the texture ID mapping
		m_texID.push_back(gl_texID);
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);
		//store the texture data for OpenGL use
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
			border, image_format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		return gl_texID;
	}

	/*
	GLuint LoadCubemapTexture(std::vector<const char*> filenames, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0)
	{
		GLuint cubemapTex;
		glGenTextures(1, &cubemapTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);

		FREE_IMAGE_FORMAT fif;
		FIBITMAP *dib;
		BYTE* bits;
		unsigned int width, height;

		int i = 0;
		for (auto filename = filenames.begin(); filename != filenames.end(); ++filename, ++i)
		{
			// init
			fif = FIF_UNKNOWN;
			dib = nullptr;
			bits = nullptr;
			width = 0;
			height = 0;

			// load
			fif = FreeImage_GetFileType(*filename, 0);
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(*filename);
			if (fif == FIF_UNKNOWN)
			{
				std::cout << "cubemap load error: " << *filename << std::endl;
				return -1;
			}

			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, *filename);
			if (!dib)
			{
				std::cout << "cubemap load error: " << *filename << std::endl;
				return -1;
			}

			bits = FreeImage_GetBits(dib);
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);

			if (bits == nullptr || width == 0 || height == 0)
			{
				std::cout << "cubemap load error: " << *filename << std::endl;
				return -1;
			}

			// set: right - left - top - bottom - front - back
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				level, internal_format, width, height,
				border, image_format, GL_UNSIGNED_BYTE, bits);

			FreeImage_Unload(dib);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		m_texID.push_back(cubemapTex);

		return cubemapTex;
	}
	*/

	GLuint LoadCubemapTexture(std::vector<const char*> filenames, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0)
	{
		GLuint cubemapTex;
		glGenTextures(1, &cubemapTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);

		//FREE_IMAGE_FORMAT fif;
		//FIBITMAP* dib;
		//BYTE* bits;
		//unsigned int width, height;
		//GLuint gl_texID;

		int i = 0;
		for (auto filename = filenames.begin(); filename != filenames.end(); ++filename, ++i)
		{
			// init
			//fif = FIF_UNKNOWN;
			//dib = nullptr;
			//bits = nullptr;
			int width = 0, height = 0;
			int nrChannels = 0;
			unsigned char* data = nullptr;

			// load
			//fif = FreeImage_GetFileType(*filename, 0);
			//if (fif == FIF_UNKNOWN)
			//	fif = FreeImage_GetFIFFromFilename(*filename);
			//if (fif == FIF_UNKNOWN)
			//{
			//	std::cout << "cubemap load error: " << *filename << std::endl;
			//	return -1;
			//}
			//
			//if (FreeImage_FIFSupportsReading(fif))
			//	dib = FreeImage_Load(fif, *filename);
			//if (!dib)
			//{
			//	std::cout << "cubemap load error: " << *filename << std::endl;
			//	return -1;
			//}
			//
			//bits = FreeImage_GetBits(dib);
			//width = FreeImage_GetWidth(dib);
			//height = FreeImage_GetHeight(dib);

			// load image file
			data = stbi_load(*filename, &width, &height, &nrChannels, 0);

			if (data == nullptr || width == 0 || height == 0)
			{
				std::cout << "cubemap load error: " << *filename << std::endl;
				return -1;
			}

			// set: right - left - top - bottom - front - back
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				level, internal_format, width, height,
				border, image_format, GL_UNSIGNED_BYTE, data);

			//FreeImage_Unload(dib);
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		m_texID.push_back(cubemapTex);

		return cubemapTex;
	}

	//free the memory for a texture
	void UnloadTexture(const unsigned int texID)
	{
		auto id = find(m_texID.begin(), m_texID.end(), texID);
		glDeleteTextures(1, &texID);
		m_texID.erase(id);
	}

	//set the current texture
	void BindTexture(const unsigned int texID)
	{
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	//free all texture memory
	void UnloadAllTextures()
	{
		//start at the begginning of the texture map
		auto i = m_texID.begin();

		while (m_texID.size() != 0)
		{
			glDeleteTextures(1, &m_texID[0]);
			m_texID.erase(m_texID.begin());
		}

		//clear the texture map
		m_texID.clear();
	}

protected:
	TextureManager()
	{
		// call this ONLY when linking with FreeImage as a static library
		//#ifdef FREEIMAGE_LIB
		//FreeImage_Initialise();
		//#endif
		m_inst = nullptr;
	}
	TextureManager(const TextureManager& tm)
	{
		if (!m_inst)
			m_inst = new TextureManager();
		m_texID.clear();
	}
	TextureManager& operator=(const TextureManager& tm);

	static TextureManager* m_inst;
	std::vector<GLuint> m_texID;
};

TextureManager* TextureManager::m_inst = nullptr;