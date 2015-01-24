/* Blit3D cross-platform game graphics library, written by Darren Reid

version 0.2 - moved classes into seperate header/cpp files, added spriteSet for automatic deletion of sprites
	created by the sprite factory MakeSprite() calls.
version 0.11
*/

#pragma once

//defining _USE_MATH_DEFINES here means that we have access to some extra defines in math.h
#define _USE_MATH_DEFINES

#ifdef _WIN32
	#define WIN32_EXTRA_LEAN
	#include <Windows.h>
#endif
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <glm/glm.hpp> //GLM OpenGL math library
#include <glm/gtc/type_ptr.hpp> //used to covert glm data to OpenGL data
#include <glm/gtc/quaternion.hpp> //quaternions for handling rotation
#include < glm/gtc/matrix_transform.hpp > //transformation functiona ala OpenGL

#include <stdio.h>
#include <conio.h>
#include <thread>

#include "Logger.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>

#include <Commdlg.h>
#include <cassert>
#include <sstream> 
#include <unordered_set>

#include <atomic>
#include <mutex>

#include "TextureManager.h"
#include "ShaderManager.h"
#include "RenderBuffer.h"
#include "Sprite.h"
#include "BFont.h"

//this macro helps calculate offsets for VBO stuff
//Pass i as the number of bytes for the offset, so be sure to use sizeof() 
//to help calculate bytes accurately.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//structure to store vertex info for textured objects

namespace B3D
{
	class TVertex
	{
	public:
		GLfloat x, y, z;//position		
		GLfloat u, v; //texture coordinates
	};

	class JoystickState
	{
	public:
		int axisCount; //how many axes there are for this joystick...yes, "axes" is the plural of "axis", I know it looks funny
		const float *axisStates; //array of axisCount floats, between -1 and 1 in value
		int buttonCount; //how many buttons there are for this joystick
		const unsigned char *buttonStates; //array of buttonCount unsigned chars, will either be GLFW_PRESS or GLFW_RELEASE in value
	};
}

enum class Blit3DThreadModel { SINGLETHREADED = 1, SIMPLEMULTITHREADED, MULTUTHREADED };

enum class Blit3DWindowModel { DECORATEDWINDOW = 1, FULLSCREEN, BORDERLESSFULLSCREEN};

enum class Blit3DRenderMode { BLIT2D = 0, BLIT3D };


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Sprite;
class BFont;
class RenderBuffer;

class Blit3D
{
public:
	ShaderManager *sManager;
	TextureManager *tManager;

	GLFWwindow* window;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	Blit3DRenderMode mode;

	std::atomic<float> screenWidth, screenHeight;
	Blit3DWindowModel winMode;

	float nearplane, farplane;
	GLSLProgram *shader2d;

	//function pointers
private:
	void (*Init)(void);
	void (*Update)(double);
	void (*Draw)(void);
	void (*DeInit)(void);
	void (*DoInput)(int , int, int, int);
	void (*Sync)(void);
	void(*DoCursor)(double, double);
	void(*DoMouseButton)(int, int, int);
	void(*DoScrollwheel)(double, double);
	void(*DoJoystick)(void);

	std::mutex spriteMutex;
	std::unordered_set<Sprite *> spriteSet;
	
public:	

	Blit3D(Blit3DWindowModel windowMode, int width, int height);

	//for Josh Cooper...I swear, no one else needs this ;)
	Blit3D(); 

	~Blit3D();
	
	int Run(Blit3DThreadModel threadType);
	void Quit(void);

	Sprite *MakeSprite(GLfloat startX, GLfloat startY, GLfloat width, GLfloat height, std::string TextureFileName);
	Sprite *MakeSprite(RenderBuffer *rb);
	void DeleteSprite(Sprite *sprite);
	
	RenderBuffer *MakeRenderBuffer(int width, int height, std::string name);
	
	BFont *MakeBFont(std::string TextureFileName, std::string widths_file, float fontsize);
	
	void Reshape(GLSLProgram *shader);
	void ReshapFBO(int FBOwidth, int FBOheight, GLSLProgram *shader);
	void SetMode(Blit3DRenderMode newMode);
	void SetMode(Blit3DRenderMode newMode, GLSLProgram *shader);
	Blit3DRenderMode GetMode(void);

	//methods for setting callbacks
	void SetInit(void(*func)(void));
	void SetUpdate(void(*func)(double));
	void SetDraw(void(*func)(void));
	void SetDeInit(void(*func)(void));
	void SetDoInput(void(*func)(int, int, int, int));
	void SetSync(void(*func)(void));
	void SetDoCursor(void(*func)(double, double));
	void SetDoMouseButton(void(*func)(int, int, int));
	void SetDoScrollwheel(void(*func)(double, double)); 
	void SetDoJoystick(void(*func)(void));

	//joystick polling, fills out the state struct if it returns true, returns false if the joystick isn't plugged in.
	bool PollJoystick(int joystickNumber, B3D::JoystickState &joystickState);
	//poll joystick to see if it is still plugged in
	bool CheckJoystick(int joystickNumber);
	
	void ShowCursor(bool show);
};
