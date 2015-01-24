#ifndef _FILE_MGR_H_
#define _FILE_MGR_H_

#include <winfs_tools.h>

//C++ STL
#include <unordered_set>
#include <map>
#include <algorithm>


extern std::wstring execution_path;

class File_Manager
{
	private:
		static dir_lister& Get()
		{
			static dir_lister Singleton_Dir_Lister;
			return Singleton_Dir_Lister;
		}
		static std::unordered_set<std::string>& Get_Loaded()
		{
			static std::unordered_set<std::string> Singleton_Loaded_List;
			return Singleton_Loaded_List;
		}

	protected:
	public:
		~File_Manager()
		{
			int i = 0;
		}
		/*static void Register_Directory(std::wstring path)
		{
			if ( Get_Loaded().emplace(path).second )
			{
				Get().Set_Path(path);
				Get().Find_Files();
			}
		}*/
		static void Register_Directory(std::string path)
		{
			if ( Get_Loaded().emplace(path).second )
			{
				Get().Set_Path(path);
				Get().Find_Files();
			}
		}
		static stringmap& Get_Files(std::string extension)
		{
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
			return Get().Get_Files_List(extension);
		}
};


#endif
