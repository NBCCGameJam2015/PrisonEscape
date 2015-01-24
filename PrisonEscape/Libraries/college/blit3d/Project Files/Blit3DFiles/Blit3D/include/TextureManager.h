/*
Darren's Texture Manager

Example texture manager, demonstrates reference counting and
texture binding state management.

Uses the excellent Free Image library as it's image loader.

Supports GLEW if you use that to load OpenGL extensions under Windows.

Version 2.0, added support for multi-texturing
Version 1.8, added BindTexture(std::string filename) to bind by filename
Version 1.7, uses Free Image instead of SOIL as it's image loading library
Version 1.6, wrap type defaults to GL_CLAMP_TO_EDGE

Version 1.5, added all calls the glTexParameter funcs inside the TextureManager,
and changed LoadTexture() so that it takes an argument for texture wrap type

Version 1.4, added AddLoadedTexture() to let FBO add textures to the TM

*/

#pragma once

//comment out this define if you are NOT using GLEW under windows
#define _USING_GLEW_

#include <windows.h>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

//#ifdef _WIN32 
//	#define WIN32_LEAN_AND_MEAN
//	#include <windows.h>
//	#ifdef _USING_GLEW_
//		#include <gl/glew.h>
//	#else
//		#include <gl/gl.h>
//	#endif
//#else
//	#include <gl/gl.h>
//#endif

#include <FreeImage.h>
#include <map>
#include <algorithm>
#include "glslprogram.h"


struct tex
{
public:
	GLuint texId; //opengl texture object id
	GLuint refcount; //reference counter...how many objects are using this texture
	bool unload; //do we unload this texture and free it's id when the refcount is 0?
	int width, height;
};

struct ltstr
{
	bool operator()(std::string s1, std::string s2) const
  {
	  return strcmp(s1.c_str(), s2.c_str()) < 0;
  }
};

//cthe maximun texture units OpenGL supports
#define TEXTURE_MANAGER_MAX_TEXTURES 31

// This object will allocate, track references, and free all textures
class TextureManager
{
private:
	std::map<std::string, tex *, ltstr> textures; //list of textures and associated id's, in a map
	GLuint currentId[TEXTURE_MANAGER_MAX_TEXTURES]; //currently bound texture
	std::map<std::string, tex *, ltstr>::iterator itor; //might as well save an iterator to use on our map
	
public:
	std::string texturePath; //relative path to the files

	int texureLocation; // Store the location of our texture sampler in the shader

	void InitShaderVar(GLSLProgram *the_shader, const char * samplerName, int shaderVar = 0); //initalizes the shader variable for the sampler

	GLuint LoadTexture(std::string filename, bool useMipMaps = false, GLuint texture_unit = GL_TEXTURE0, GLuint wrapflag = GL_CLAMP_TO_EDGE);
	void FreeTexture(std::string filename); 
	void BindTexture(GLuint bindId, GLuint texture_unit = GL_TEXTURE0);
	void BindTexture(std::string filename, GLuint texture_unit = GL_TEXTURE0);
	void SetTexturePath(std::string path);
	void AddLoadedTexture(std::string name, GLuint bindId);//used by FBO add pre-created textures
	bool FetchDimensions(std::string name, GLfloat &width, GLfloat &height);
	TextureManager(void);
	~TextureManager(void);
};


