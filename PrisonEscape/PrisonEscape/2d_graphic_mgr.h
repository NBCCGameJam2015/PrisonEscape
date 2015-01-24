#ifndef GRAPHIC_MGR_H
#define GRAPHIC_MGR_H

#include "2d_graphic.h"
#include "file_mgr.h"
#include <map>
#include <string>

class Graphic_Manager
{
private:
	GLSLProgram* shader_2d;
	std::string graphic_directory;
	std::map<std::string, graphic*> graphic_map;
	std::map<std::string, graphic*>::iterator graphic_iter;

protected:
	void Load_Graphic( std::string filename );

public:
	Graphic_Manager();
	~Graphic_Manager()
	{
		graphic_iter = graphic_map.begin();
		while ( graphic_iter != graphic_map.end() )
		{
			delete graphic_iter->second;
			graphic_iter++;
		}
		graphic_iter = graphic_map.begin();
	}
	void set_shader( GLSLProgram* shader );
	void Load_All();
	graphic* Get_Graphic( std::string filename );
};

#endif
