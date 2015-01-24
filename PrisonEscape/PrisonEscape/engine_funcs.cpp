#include <blit3d\Blit3D.h>

#include <atomic>
#include <stdio.h>
#include <conio.h>
#include <thread>

#include "game.h"
#include "global.h"
#include "log.h"

#ifdef _WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif

//GLOBAL DATA
FFont Fcout;
std::mutex View_Mutex;
glm::mat4 View_Matrix = glm::mat4( 1.0f );
glm::mat4 View_Angle = glm::mat4( 1.0f );
glm::mat4 View_Position = glm::mat4( 1.0f );

Blit3D* get_blit3d()
{
	static Blit3D* the_blit3D = new Blit3D( Blit3DWindowModel::DECORATEDWINDOW, 1031, 950 );
	return the_blit3D;
}

game& get_game_engine()
{
	static game engine;
	return engine;
}

void Init()
{
	get_game_engine().Init();
}
void DeInit()
{
	get_game_engine().DeInit();
}
void Update( double seconds )
{
	get_game_engine().Update( seconds );
}
void Draw()
{
	get_game_engine().Draw();
}
void DoInput( int key, int scancode, int action, int mods )
{
	get_game_engine().DoInput( key, scancode, action, mods );
}
void DoCursor( double x, double y )
{
	get_game_engine().DoCursor( x, y );
}

inline void game::Init()
{
	dbg::FileLog_Mgr::RegisterNewLog( L"Memory Issues.log" );
	dbg::FileLog_Mgr::Start();

	//This is the directory Meshes and Textures are found in
	File_Manager::Register_Directory( textures );
	//File_Manager::Register_Directory( models );
	File_Manager::Register_Directory( graphic_dir );

	// DARREN'S PROJECT DIRECTORY (right click a .cpp or .h tab above and open containing folder, copy paste directory into string)
	File_Manager::Register_Directory( "" );

	Init_GFX();
	Init_Input();

	grid = graphic_mgr.Get_Graphic( "grid.png" );
	grid->dest_x = 0;
	grid->dest_y = 0;
	correct = graphic_mgr.Get_Graphic( "correct.png" );
	correct->dest_x = 0;
	correct->dest_y = 0;
	correct->alpha = 0.5f;
	incorrect = graphic_mgr.Get_Graphic( "incorrect.png" );
	incorrect->dest_x = 0;
	incorrect->dest_y = 0;
	incorrect->alpha = 0.5f;
}

void game::Init_GFX()
{
	// We need to prepare our Shaders
	shader_3d = get_blit3d()->sManager->GetShader( "lighting.vert", "lighting.frag" );
	shader_2d = get_blit3d()->sManager->GetShader( "shader2d.vert", "shader2d.frag" );

	// Load Assets
	///################################
	txtr_mgr.LoadAll(); // Textures
	
	graphic_mgr.set_shader( shader_2d );
	graphic_mgr.Load_All(); // Images (aka graphics)

	mesh_mgr.set_Shader( shader_3d );
	mesh_mgr.Load_All();  // Meshes

	Fcout.set_shader_2d( shader_2d );
	Fcout.Initialize();
	///################################


	// Enable Blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Camera Setup
	View_Angle = glm::rotate( View_Angle, 180.0f, glm::vec3( 0, 0, 1 ) );
	View_Matrix = View_Angle * View_Position;
	glm::vec4 dir = View_Angle * glm::vec4( 0, 0, 1, 0 );
	player_straight_axis = glm::vec3( dir.x, dir.y, dir.z );
	dir = View_Angle * glm::vec4( 1, 0, 0, 0 );
	player_side_axis = glm::vec3( dir.x, dir.y, dir.z );

	
	// Preparing Variable Values
	get_blit3d()->projectionMatrix = glm::perspective( 45.0f, (GLfloat)( get_blit3d()->screenWidth ) / (GLfloat)( get_blit3d()->screenHeight ), 0.1f, 10000.0f );
	glm::vec3 LightPosition = glm::vec3( 1.0f, 1.0f, 1.0f );
	glm::vec3 LightIntensity = glm::vec3( 1.0f, 1.0f, 1.0f );
	glm::vec3 Kd = glm::vec3( 1.0f, 1.0f, 0.2f );
	glm::vec3 Ka = glm::vec3( 0.1f, 0.1f, 0.2f );
	glm::vec3 Ks = glm::vec3( 1.0f, 1.0f, 1.0f );
	GLfloat Shininess = 1.0f;
	
	// 3D Section
	// Binding Values to Variables within shader
	shader_3d->setUniform( "projectionMatrix", get_blit3d()->projectionMatrix );
	shader_3d->setUniform( "LightPosition", LightPosition );
	shader_3d->setUniform( "LightIntensity", LightIntensity );
	shader_3d->setUniform( "Kd", Kd );
	shader_3d->setUniform( "Ka", Ka );
	shader_3d->setUniform( "Ks", Ks );
	shader_3d->setUniform( "Shininess", Shininess );
	shader_3d->setUniform( "in_Alpha", 1.f );
	shader_3d->bindAttribLocation( 0, "in_Position" );
	shader_3d->bindAttribLocation( 1, "in_Normal" );
	shader_3d->bindAttribLocation( 2, "in_Texcoord" );
	shader_3d->printActiveUniforms();
	shader_3d->printActiveAttribs();

	// 2D Section
	// Binding Values to Variables within shader
	shader_2d->setUniform( "in_Scale", 1.0f );
	shader_2d->bindAttribLocation( 0, "in_Position" );
	shader_2d->bindAttribLocation( 1, "in_Texcoord" );
	shader_2d->printActiveUniforms();
	shader_2d->printActiveAttribs();
}

void game::Init_Input()
{
	KeyInput_Mgr.Start();
}

inline void game::DeInit( void )
{
	KeyInput_Mgr.Stop();
}

inline void game::Update( double& seconds )
{
	//View_Matrix = glm::rotate( View_Matrix, 0.5f, glm::vec3( 0, 0, 1 ) );
	SceneGraph.Update( seconds );
}

inline void game::Draw( void )
{
	glClearColor( 0.8f, 0.6f, 0.7f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	get_blit3d()->SetMode( Blit3DRenderMode::BLIT3D );
	this->shader_3d->use();
	View_Mutex.lock();
	this->shader_3d->setUniform( "viewMatrix", View_Matrix );
	View_Mutex.unlock();

	get_blit3d()->SetMode( Blit3DRenderMode::BLIT2D, shader_2d );
}

inline void game::DoInput( int& key, int& scancode, int& action, int& mods )
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
	{
		get_blit3d()->Quit(); //start the shutdown sequence
	}
	else if ( action == GLFW_PRESS )
	{
		KeyInput_Mgr.Add( key );
	}
}

inline void game::DoCursor( double& x, double& y )
{
	mouseMutex.lock();
	cx = (float)x;
	cy = get_blit3d()->screenHeight - (float)y;
	mouseMutex.unlock();
}