#include "global.h"
#include "ffont.h"
#include <string>

extern logger oLog;

FFont::FFont()
{
	font_size = 16.0f;
	text_angle = 0.0f;
	text_alpha = 1.0f;

	info.total_vertices = 4 * 5 * 256;
	float* verts = new float[info.total_vertices];
	info.Vertices = verts;
	info.texture_file = "font.png";
	for ( int loop = 0; loop<256; loop++ )
	{
		float cx = ( (float)( loop % 16 ) ) / 16.0f;				// X Position Of Current Character
		float cy = ( (float)( loop / 16 ) ) / 16.0f;				// Y Position Of Current Character

		verts[( loop * 20 )] = 0;
		verts[1 + ( loop * 20 )] = 0;		// Vertex Coord (Bottom Left)
		verts[2 + ( loop * 20 )] = 0;
		verts[3 + ( loop * 20 )] = cx;
		verts[4 + ( loop * 20 )] = 1 - ( cy + 1.f / 16 );	// Texture Coord (Bottom Left)

		verts[5 + ( loop * 20 )] = 1;
		verts[6 + ( loop * 20 )] = 0;	// Vertex Coord (Bottom Right)
		verts[7 + ( loop * 20 )] = 0;
		verts[8 + ( loop * 20 )] = cx + ( 1.f / 16 );
		verts[9 + ( loop * 20 )] = 1 - ( cy + 1.f / 16 );	// Texture Coord (Bottom Right)

		verts[10 + ( loop * 20 )] = 1;
		verts[11 + ( loop * 20 )] = 1;	// Vertex Coord (Top Right)
		verts[12 + ( loop * 20 )] = 0;
		verts[13 + ( loop * 20 )] = cx + ( 1.f / 16 );
		verts[14 + ( loop * 20 )] = 1 - cy;	// Texture Coord (Top Right)

		verts[15 + ( loop * 20 )] = 0;
		verts[16 + ( loop * 20 )] = 1;		// Vertex Coord (Top Left)
		verts[17 + ( loop * 20 )] = 0;
		verts[18 + ( loop * 20 )] = cx;
		verts[19 + ( loop * 20 )] = 1 - cy;	// Texture Coord (Top Left)
	}

	std::ifstream data_file;
	data_file.open( "fontMetrics.dat", std::ios::in | std::ios::binary );
	if ( !data_file.is_open() )
	{
		//oLog( Level::Severe ) << "Error while loading widths data file: 'fontMetrics.dat' for FFont";
		//assert( data_file.is_open() );
	}

	short buffer[256]; //memory to read into
	char *buff = (char *)buffer;

	data_file.seekg( 0, std::ios::beg );//seek to start

	//load entire file in one go...much faster than reading 2 chars at a time!
	data_file.read( buff, 512 );
	data_file.close();

	for ( int i = 0; i < 256; ++i )
	{
		widths[i] = (_int32)buffer[i];
	}

	///#############
}

float FFont::WidthText( std::string text )
{
	int letter;
	float width_text = 0;
	float scale = font_size / 128;

	for ( unsigned int i = 0; i < text.size(); ++i )
	{
		letter = Fancy_Font ? text[i] - 32 + 128 : text[i] - 32;

		width_text += widths[letter] * scale;
	}

	return width_text;
}

void FFont::print( std::string text )
{
	glBindVertexArray( VAO_id );
	get_game_engine().txtr_mgr.BindTexture( tex_id );

	modelMatrix = glm::translate( glm::mat4( 1.f ), text_position );
	modelMatrix = glm::rotate( modelMatrix, text_angle, glm::vec3( 0.f, 0.f, 1.f ) );
	shader_2d->use();
	shader_2d->setUniform( "in_Scale", font_size );
	shader_2d->setUniform( "in_Alpha", text_alpha );

	int letter;
	float scale = font_size / 128;
	for ( unsigned int i = 0; i < text.size(); ++i )
	{
		shader_2d->setUniform( "modelMatrix", modelMatrix );
		letter = Fancy_Font ? text[i] - 32 + 128 : text[i] - 32;

		glDrawArrays( GL_QUADS, letter * 4, 4 );
		modelMatrix = glm::translate( modelMatrix, glm::vec3( (float)widths[letter] * scale, 0.f, 0.f ) );
	}

	glBindVertexArray( 0 );
}
