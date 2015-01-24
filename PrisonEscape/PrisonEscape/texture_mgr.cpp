#include "texture_mgr.h"
#include <string>


void Texture_Manager::LoadAll()
{
	// prefix sl for string local type
	using slFile_Name = std::string;
	using slFile_Path = std::string;
	std::map<slFile_Name, slFile_Path> path_map = File_Manager::Get_Files(".png");
	std::map<slFile_Name, slFile_Path>::iterator path_itor = path_map.begin();
	for ( ; path_itor != path_map.end(); ++path_itor )
	{
		if ( file_map.find( path_itor->first ) == file_map.end() )
		{
			LoadTexture( path_itor->first, path_itor->second );
		}
	}
}

void Texture_Manager::LoadTexture(std::string file, std::string path)
{
	//we didn't find that texture name, so it is a new texture
	coope::tex *newtex = new coope::tex;

	//add the path to the file
	std::string fullpath;
	fullpath = path + file;
	newtex->refcount = 0;
	newtex->unload = true; //currently setting all textures to unload when refcount = 0;

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	{
		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(fullpath.c_str(), 0);
		//if still unknown, try to guess the file format from the file extension
		if ( fif == FIF_UNKNOWN )
			fif = FreeImage_GetFIFFromFilename(fullpath.c_str());
		//if still unkown, return failure
		if ( fif == FIF_UNKNOWN )
			goto ERROR_HANDLER;

		//check that the plugin has reading capabilities and load the file
		if ( FreeImage_FIFSupportsReading(fif) )
			dib = FreeImage_Load(fif, fullpath.c_str());
		//if the image failed to load, return failure
		if ( !dib )
			goto ERROR_HANDLER;

		//maybe???
		if ( FreeImage_GetBPP(dib) != 32 )  dib = FreeImage_ConvertTo32Bits(dib);

		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		//get the image width and height
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if ( (bits == 0) || (width == 0) || (height == 0) )
			goto ERROR_HANDLER;
	}
		
			
	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
			
	//store the texture ID mapping
	newtex->ID = gl_texID;
	this->file_map[file] = newtex->ID;
	//add the new texture to the map
	this->texture_map[newtex->ID] = newtex;

	//bind to the new texture ID
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	Bound_IDs[0] = newtex->ID;


	//set up some vars for OpenGL texturizing
	GLenum image_format = GL_RGBA;
	GLint internal_format = GL_RGBA;
	GLint level = 0;
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,	0, image_format, GL_UNSIGNED_BYTE, bits); //store the texture data for OpenGL use
			
	//swizzle colors
	GLint swizzleMask[] = { GL_BLUE, GL_GREEN, GL_RED, GL_ALPHA };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

	if (useMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
				
		FreeImage_Unload(dib);
		newtex->width = width;
		newtex->height = height;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //for when we are close
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//when we are far away
	}
	else
	{
		FreeImage_Unload(dib);
		newtex->width = width;
		newtex->height = height;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //for when we are close
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//when we are far away
	}

	//the following turns on a special, high-quality filtering mode called "ANISOTROPY"
	if(GL_EXT_texture_filter_anisotropic)
	{
		GLfloat largest_supported_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
	}

	// the texture stops at the edges with GL_CLAMP_TO_EDGE
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	return void(0);

ERROR_HANDLER:
#ifdef _WIN32			
	MessageBox(0, (LPCWSTR)"Free Image loading error!", (LPCWSTR)((path + file).c_str()), MB_OK | MB_ICONEXCLAMATION);
#else
	std::cout << "Free Image loading error while loading: " << filename << std::endl;
#endif
	return void(0);
}

GLuint Texture_Manager::GetID(std::string File, bool count_increase)
{
	file_itor = file_map.find( File );
	if ( file_itor == file_map.end() )
	{
		return 0;
	}
	else
	{
		if (count_increase) texture_map[file_itor->second]->refcount++;
		return file_itor->second;
	}
}

coope::tex* Texture_Manager::GetTexPtr( texid ID )
{
	auto iter = texture_map.find( ID );
	if ( iter != texture_map.end() )
	{
		return iter->second;
	}
	return nullptr;
}

void Texture_Manager::FreeTexture(GLuint id)
{
	texture_itor = texture_map.find(id); //lookup this texture in our std::map

	if(texture_itor != texture_map.end())
	{
		texture_itor->second->refcount--; //update the refcount
		if(texture_itor->second->refcount <= 0 && texture_itor->second->unload)
		{
			//we have freed the last refernce, so we can delete this texture from memory
			glDeleteTextures(1, &id);

			//if this was the currently bound texture, set currentId to a bad ID value
			//that won't be matched by the next call to BindTexture()
			for (int i = 0; i < TEXTURE_MANAGER_MAX_TEXTURES; ++i)
				if (Bound_IDs[i] == texture_itor->second->ID)
					Bound_IDs[i] = -1;

			delete texture_itor->second; //free the instance of a tex struct

			texture_itor->second = nullptr;
			//clear the texture from the std::map
			texture_map.erase(texture_itor);
		}
	}
}

void Texture_Manager::BindTexture(GLuint id, GLuint texture_unit)
{
	if (Bound_IDs[texture_unit - GL_TEXTURE0] != id)
	{
		glActiveTexture(texture_unit); //needed for programmable shaders
		glBindTexture(GL_TEXTURE_2D, id);
		
		Bound_IDs[texture_unit - GL_TEXTURE0] = id;
	}
}

