#ifndef _S3DMESHLOADER_H_
#define _S3DMESHLOADER_H_

#include "mesh.h"
#include "texture_mgr.h"
#include "file_mgr.h"

#include <map>

/******************************************************
* S3DMesh_Manager Class
* Imports S3D file and stores the information withing the class.
* Created by:	Josh Cooper		Date:	Dec. 4, 2014
* Modified by:	Josh Cooper		Date:	Dec. 4, 2014
*******************************************************/
class s3d_mesh_mgr
{
private:
	GLSLProgram* shader_3d = nullptr;
	std::map<std::string, mesh*> mesh_map;
	std::map<std::string, mesh*>::iterator mesh_iter;

	void Free_Meshes(){}
	inline mesh* Import_S3D_File( std::string file_path );

public:
	s3d_mesh_mgr()
	{
		File_Manager::Register_Directory( "" );
	}
	~s3d_mesh_mgr()
	{
		mesh_iter = mesh_map.begin();
		while ( mesh_iter != mesh_map.end() )
		{
			delete mesh_iter->second;
			mesh_iter++;
		}
		mesh_iter = mesh_map.begin();
	}
	void set_Shader( GLSLProgram* shader_3d );
	void Load_All();
	void Load_Mesh( std::string file, std::string path );
	mesh* Get_Mesh( std::string file_name );
};

#endif