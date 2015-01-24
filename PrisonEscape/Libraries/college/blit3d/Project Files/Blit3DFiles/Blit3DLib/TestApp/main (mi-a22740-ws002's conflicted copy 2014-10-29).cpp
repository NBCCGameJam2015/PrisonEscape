/*
	Example program that shows Blit3D is working
*/
#include "Blit3D.h"
#include <atomic>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Blit3D *blit3D = NULL;

//GLOBAL DATA
GLSLProgram *prog = NULL;

glm::mat4 modelMatrix;
std::atomic<float> angle = 0;

GLuint vbo = 0;
GLuint vao = 0;

//sprite test
Sprite *sprite = NULL;

//bitmap font test
BFont *bfont = NULL;

//cursor test
float cx, cy;

void Init()
{
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */
	float halfWidth = blit3D->screenWidth * 0.5f;
	float halfHeight = blit3D->screenHeight * 0.5f;
	float points[] =
	{
		-1.0f * halfWidth, -1.0f * halfHeight, 0.0f, 0.f, 0.f,
		1.0f * halfWidth, -1.0f * halfHeight, 0.0f, 1.f, 0.f,
		-1.0f * halfWidth, 1.0f * halfHeight, 0.0f, 0.f, 1.f,
		1.0f * halfWidth, 1.0f * halfHeight, 0.0f, 1.f, 1.f
	};


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), points, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(sizeof(float) * 3));

	glBindVertexArray(0); // Disable our Vertex Array Object? 
	glBindBuffer(GL_ARRAY_BUFFER, 0);// Disable our Vertex Buffer Object


	prog = blit3D->sManager->UseShader("shader.vert", "shader.frag"); //load/compile/link

	modelMatrix = glm::mat4(1.f);

	//2d orthographic projection
	blit3D->SetMode(Blit3DRenderMode::BLIT2D);
	
	//3d perspective projection
	//blit3D->projectionMatrix *= glm::perspective(45.0f, (GLfloat)(blit3D->screenWidth) / (GLfloat)(blit3D->screenHeight), 0.1f, 10000.0f);

	//send matrices to the shader
	prog->setUniform("projectionMatrix", blit3D->projectionMatrix);
	prog->setUniform("viewMatrix", blit3D->viewMatrix);
	prog->setUniform("modelMatrix", modelMatrix);

	//send alpha to the shader
	prog->setUniform("in_Alpha", 1.f);

	//attributes
	prog->bindAttribLocation(0, "in_Position");
	prog->bindAttribLocation(1, "in_Texcoord");

	blit3D->tManager->LoadTexture("Logo.png", false);
	blit3D->tManager->LoadTexture("grassMid.png", true);

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	prog->printActiveUniforms();
	prog->printActiveAttribs();

	//load a sprite
	sprite = blit3D->MakeSprite(324, 140, 46, 46, "spritesheet.png");

	//load a font set
	bfont = blit3D->MakeBFont("font.png", "fontMetrics.dat", 48.f);
	assert(bfont != NULL); //make sure it loaded

	cx = blit3D->screenWidth / 2;
	cy = blit3D->screenHeight / 2;
}

void DeInit(void)
{
	//free any sprites still allocated
	if(sprite) delete sprite;
	//free fonts
	if(bfont) delete bfont;
	if(blit3D) delete blit3D;
}

void Update(double seconds)
{
	angle = angle + static_cast<float>(seconds) * 30;
	while(angle > 360) angle = angle - 360;
}

void Draw(void)
{
	glClearColor(0.8f, 0.6f, 0.7f, 0.0f);	//clear colour: r,g,b,a 	
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = (float)glfwGetTime();

	//turn off the cursor
	blit3D->ShowCursor(false);
	

//========BACKGROUND===============
	prog = blit3D->sManager->UseShader("shader.vert", "shader.frag");
	
	blit3D->SetMode(Blit3DRenderMode::BLIT2D, prog); //use this version of setMode when you want to use your own shader instead of the built-in 2d shader
	glBindVertexArray(vao);
	modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(blit3D->screenWidth * 0.5f, blit3D->screenHeight * 0.5f, 0.f));
	prog->setUniform("modelMatrix", modelMatrix);
	blit3D->tManager->BindTexture("Logo.png");

	// draw points 0-4 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//reset depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(0); //turn off vao for now
//=================================

//========Image spinning in 3D=====
	prog = blit3D->sManager->UseShader("shader.vert", "shader.frag");
	
	blit3D->SetMode(Blit3DRenderMode::BLIT3D, prog);//also use this version of setMode when you want to have the shader's matrices automatically updated after changing the mode to 3D
	glBindVertexArray(vao); //we just re-use same vao in this example
	modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(-0.1f, 0.1f, -5000.f));
	modelMatrix = glm::rotate(modelMatrix, (float)angle, glm::vec3(0.f, 1.f, 0.f));
	prog->setUniform("modelMatrix", modelMatrix);
	blit3D->tManager->BindTexture("grassMid.png");

	// draw points 0-4 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);//turn off vao again
//=================================

//========2D STUFF===================
	blit3D->SetMode(Blit3DRenderMode::BLIT2D); //change to 2d mode before drawing sprites/text!
	
	//========SPRITE===================
	sprite->Blit(cx, cy);
	//=================================

	//while still in 2d mode, draw some text
	//=======TEXT======================
	std::string text = "Blit3D is working.";
	float textWidth = bfont->WidthText(false, text);
	bfont->DrawText(false, blit3D->screenWidth - 20 - textWidth, 50, text);	
	//=================================
	
	blit3D->SetMode(Blit3DRenderMode::BLIT3D); //change back to 3d mode when done with 2D rendering
//=================================
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence
}

void DoCursor(double x, double y)
{
	cx = (float)x;
	cy = blit3D->screenHeight - (float)y;
}

int main(int argc, char *argv[])
{
	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN, 720, 480);

	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);
	blit3D->SetDoCursor(DoCursor);	

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SIMPLEMULTITHREADED);
}