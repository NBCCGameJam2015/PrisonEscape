#ifndef _TEXTURE_MGR_H_
#define _TEXTURE_MGR_H_

#define TEXTURE_MANAGER_MAX_TEXTURES 31

//Platform Specific
#include <Windows.h>

//Third Party / Tool box
#include <winfs_tools.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>

//C++ STL
#include <unordered_set>
#include <map>

//Project
#include "file_mgr.h"

using texid = GLuint;


namespace coope
{
	struct tex
	{
		texid ID;
		bool unload;
		unsigned int refcount;
		unsigned int width, height;
	};
}

class Texture_Manager
{
	private:
		std::map<std::string, texid> file_map;
		std::map<std::string, texid>::iterator file_itor;

		std::map<texid, coope::tex*> texture_map;
		std::map<texid, coope::tex*>::iterator texture_itor;

		GLuint Bound_IDs[TEXTURE_MANAGER_MAX_TEXTURES];// = { -1 }; //currently bound texture
		bool useMipMaps = true;

	public:
		Texture_Manager()
		{
		}
		~Texture_Manager()
		{
			for ( auto itor : texture_map )
			{
				delete itor.second;
			}
			auto iter = texture_map.begin();
		}
		void LoadAll();
		void LoadTexture(std::string file, std::string path);
		GLuint GetID(std::string File, bool count_increase = true);
		coope::tex* GetTexPtr( texid ID );

		void FreeTexture(GLuint texid);
		void BindTexture(GLuint texid, GLuint texture_unit = GL_TEXTURE0);
};

#endif
