#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <blit3d/Blit3D.h>
#include "vbo_data.h"

class graphic
{
private:
	GLSLProgram* shader_2d = nullptr;

	render_data		info;
	std::string		&texture_file = info.texture_file;
	GLuint				&tex_id = info.tex_id;

	GLuint				&VAO_id = info.VAO_id;

	GLuint				&Vertex_VBO_id = info.Vertex_VBO_id;
	GLuint				&total_vertices = info.total_vertices;
	float*				&Vertices = info.Vertices;

	glm::mat4 modelMatrix; // Store the model matrix 

public:
	graphic( GLSLProgram* shader, render_data info );
	~graphic();

	GLfloat dest_x; 
	GLfloat dest_y;
	GLfloat angle;
	GLfloat alpha;

	void Draw();
};

#endif
