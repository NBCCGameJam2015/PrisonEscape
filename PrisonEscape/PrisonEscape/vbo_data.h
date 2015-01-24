#ifndef VBO_DATA_H
#define VBO_DATA_H

#include <blit3d\Blit3D.h>

struct render_data
{
	std::string texture_file;
	GLuint tex_id;

	GLuint VAO_id;
	GLuint Vertex_VBO_id;
	GLuint total_vertices = 0;
	float* Vertices = nullptr;

	bool initialized = false;

	render_data(){}
	render_data( int nv, float* verts )
	{
		total_vertices = nv;
		this->Vertices = verts;
		
	}
	virtual void init();
	virtual void deinit();
};

struct mesh_data : render_data
{

	GLuint Index_VBO_id;
	GLuint total_indices = 0;
	unsigned int* Indices = nullptr;

	mesh_data(){}
	mesh_data( int nv, int ni, float* verts, unsigned int* indices, std::string tx ) : render_data( nv, verts )
	{
		total_indices = ni;
		this->Indices = indices;
		texture_file = tx;
	}
	virtual void init();
	virtual void deinit();
};

#endif
