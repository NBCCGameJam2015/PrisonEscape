#include <sstream>
#include <atomic>

#include "mesh.h"
#include "global.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using std::stringstream;


mesh::mesh( GLSLProgram* shader, mesh_data info, bool isStripped )
{
	shader->use(); //Cannot remove this line (nothing will draw)
	this->info = info;
	this->info.init();

	shader_3d = shader;
	bStripped = isStripped;
}

mesh::~mesh()
{
	info.deinit();
	delete Vertices;
	delete Indices;
}

void mesh::draw( glm::mat4& modelMatrix )
{
	shader_3d->use();
	shader_3d->setUniform( "modelMatrix", modelMatrix );

	glBindVertexArray( VAO_id );
	get_game_engine().txtr_mgr.BindTexture( tex_id );

	if ( bStripped )
	{
		glDrawElements( GL_TRIANGLE_STRIP, this->total_indices, GL_UNSIGNED_INT, BUFFER_OFFSET( 0 ) );
	}
	else
	{
		glDrawElements( GL_TRIANGLES, this->total_indices, GL_UNSIGNED_INT, BUFFER_OFFSET( 0 ) );
	}
	glBindVertexArray( 0 );
}