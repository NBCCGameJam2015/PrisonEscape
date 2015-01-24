#include "2d_graphic.h"
#include "global.h"

graphic::graphic( GLSLProgram* shader, render_data info )
{
	shader->use();
	shader_2d = shader;
	this->info = info;
	this->info.init();

	dest_x = 0.f;
	dest_y = 0.f;
	angle = 0.f;
	alpha = 1.f;
}

graphic::~graphic()
{
	info.deinit();
	delete Vertices;
}

void graphic::Draw( void )
{
	glBindVertexArray( VAO_id );
	get_game_engine().txtr_mgr.BindTexture( tex_id );

	modelMatrix = glm::translate( glm::mat4( 1.f ), glm::vec3( dest_x, dest_y, 0.f ) );
	modelMatrix = glm::rotate( modelMatrix, angle, glm::vec3( 0.f, 0.f, 1.f ) );
	
	shader_2d->use();
	shader_2d->setUniform( "modelMatrix", modelMatrix );
	shader_2d->setUniform( "in_Alpha", alpha );

	// draw a quad: 1 quad x 4points per quad = 4 verts, the third argument
	glDrawArrays( GL_QUADS, 0, 4 );

	// bind with 0, so, switch back to normal pointer operation
	glBindVertexArray( 0 );
}