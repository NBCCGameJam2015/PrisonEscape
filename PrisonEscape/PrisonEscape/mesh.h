#ifndef S3DMESH_H_
#define S3DMESH_H_
#include <vector>
#include <fstream>
#include <mutex>
#include <debug_toolset.h>
#include "vbo_data.h"

using std::string;
using std::ifstream;


class mesh
{
private:
	GLSLProgram*	shader_3d = nullptr;
	bool bStripped = true;

	mesh_data		info;
	std::string		&texture_file		= info.texture_file;
	GLuint				&tex_id					= info.tex_id;

	GLuint				&VAO_id				= info.VAO_id;

	GLuint				&Vertex_VBO_id	= info.Vertex_VBO_id;
	GLuint				&total_vertices	= info.total_vertices;
	float*				&Vertices				= info.Vertices;

	GLuint				&Index_VBO_id	= info.Index_VBO_id;
	GLuint				&total_indices		= info.total_indices;
	unsigned int*	&Indices				= info.Indices;


	//make TWO vbo's, so we can upload indices as a seperate vbo

protected:
	void Load( string file_path );

	//bool deleteme = false;

public:
	mesh( GLSLProgram* shader, mesh_data info, bool isStripped = true );
	~mesh();
	//bool isHidden() const { return deleteme; }

	void draw( glm::mat4& modelMatrix );
};

#endif
