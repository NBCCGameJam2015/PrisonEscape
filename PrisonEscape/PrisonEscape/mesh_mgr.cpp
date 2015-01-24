#include "mesh_mgr.h"
#include "vbo_data.h"
#include "global.h"
#include "log.h"

void s3d_mesh_mgr::set_Shader( GLSLProgram* shader_3d )
{
	this->shader_3d = shader_3d;
}

void s3d_mesh_mgr::Load_All()
{
	// prefix sl for string local type
	using slFile_Name = std::string;
	using slFile_Path = std::string;
	std::map<slFile_Name, slFile_Path> path_map = File_Manager::Get_Files( ".s3d" );
	std::map<slFile_Name, slFile_Path>::iterator path_itor = path_map.begin();
	for ( ; path_itor != path_map.end(); ++path_itor )
	{
		if ( mesh_map.find( path_itor->first ) == mesh_map.end() )
		{
			Load_Mesh( path_itor->first, path_itor->second );
		}
	}
}

void s3d_mesh_mgr::Load_Mesh( std::string file, std::string path )
{
	mesh* mymesh = Import_S3D_File( path + file );
	if ( mymesh )
	{
		mesh_map.emplace( file, mymesh );
	}
}

/****************************************************
* Imports meshes from an s3d file
* 
* Created by:	Josh Cooper		Date: Oct. 20, 2014
* Modified by:	Josh Cooper		Date: Dec. 29, 2014
*****************************************************/
mesh* s3d_mesh_mgr::Import_S3D_File( std::string file_path )
{
	GLuint total_vertices = 0;
	GLuint total_indices = 0;
	float* Vertices = nullptr;
	unsigned int* Indices = nullptr;
	std::string texture_file;

	ifstream File_Handle;
	File_Handle.open( file_path, std::ios::in );
	if ( File_Handle.is_open() )
	{
		try
		{
			File_Handle >> total_vertices;
			Vertices = new float[total_vertices * 8];
			for ( int i = 0; i < total_vertices * 8; ++i )
			{
				File_Handle >> Vertices[i];
			}
			File_Handle >> total_indices;
			Indices = new unsigned int[total_indices];
			for ( int i = 0; i < total_indices; ++i )
			{
				File_Handle >> Indices[i];
			}
			File_Handle >> texture_file;

			if ( File_Handle.fail() )
				throw File_Handle.rdstate();

			File_Handle.close();

			mesh_data info( total_vertices, total_indices, Vertices, Indices, texture_file );
			mesh* mymesh = new mesh( shader_3d, info );
			return mymesh;
		}
		catch ( ... )
		{
			File_Handle.close();
			delete Vertices;
			delete Indices;
			assert( "Critical Failure Loading S3D File " );
		}
	}
	else
	{
		assert( "Could not Open S3D File" );
	}
	return nullptr;
}

mesh* s3d_mesh_mgr::Get_Mesh( std::string file_name )
{
	mesh_iter = mesh_map.find( file_name );
	if ( mesh_iter != mesh_map.end() )
	{
		return mesh_iter->second;
	}
	return nullptr;
}