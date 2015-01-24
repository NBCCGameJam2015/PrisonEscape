#include "2d_graphic_mgr.h"
#include "global.h"

Graphic_Manager::Graphic_Manager()
{
	graphic_directory = "C:/Cheryl_Assets/2dgraphics/";
}

void Graphic_Manager::set_shader( GLSLProgram* shader )
{
	shader_2d = shader;
}

void Graphic_Manager::Load_All()
{
	// prefix sl for string local type
	using slFile_Name = std::string;
	using slFile_Path = std::string;
	std::map<slFile_Name, slFile_Path> path_map = File_Manager::Get_Files( ".png" );
	std::map<slFile_Name, slFile_Path>::iterator path_iter = path_map.begin();
	for ( ; path_iter != path_map.end(); ++path_iter )
	{
		if ( path_iter->second == graphic_directory 
			 &&  graphic_map.find( path_iter->first ) == graphic_map.end() )
		{
			Load_Graphic( path_iter->first );
		}
	}
}

void Graphic_Manager::Load_Graphic( std::string filename )
{
	GLuint texture_id = get_game_engine().txtr_mgr.GetID( filename );
	coope::tex* texture = get_game_engine().txtr_mgr.GetTexPtr( texture_id );

	float width = texture->width;
	float height = texture->height;
	float half_width = texture->width / 2;
	float half_height = texture->height / 2;

	float* vertices = new float[20];

	vertices[0] = 0;				// X
	vertices[1] = height;		// Y
	vertices[2] = 0.0f;			// Z
	vertices[3] = 0.0f;			// nU
	vertices[4] = 1.0f;			// nV

	vertices[5] = 0;
	vertices[6] = 0;
	vertices[7] = 0.0f;
	vertices[8] = 0.0f;
	vertices[9] = 0.0f;

	vertices[10] = width;
	vertices[11] = 0;
	vertices[12] = 0.0f;
	vertices[13] = 1.0f;
	vertices[14] = 0.0f;

	vertices[15] = width;
	vertices[16] = height;
	vertices[17] = 0.0f;
	vertices[18] = 1.0f;
	vertices[19] = 1.0f;

	render_data info( 4, vertices );
	info.texture_file = filename;
	info.tex_id = texture_id;
	graphic* g = new graphic( shader_2d, info );

	graphic_map[filename] = g;
}

graphic* Graphic_Manager::Get_Graphic( std::string filename )
{
	graphic_iter = graphic_map.find( filename );
	if ( graphic_iter != graphic_map.end() )
	{
		return graphic_iter->second;
	}
	return nullptr;
}
