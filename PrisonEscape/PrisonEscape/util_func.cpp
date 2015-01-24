#include <sstream>
#include <string>

#include "util_func.h"
#include "file_mgr.h"


std::wstring& get_exec_path()
{
	static wchar_t buffer[MAX_PATH];
	static bool done = false;
	if ( !done )
	{
		GetModuleFileName( NULL, buffer, MAX_PATH );
		done = true;
	}
	static std::wstring execution_path( buffer );
	return execution_path;
}

inline std::string coords_ToString(std::pair<int, int> position)
{
	std::stringstream temp;
	temp << " --X:" << position.first << ", Y:" << position.second << "-- ";
	return temp.str();
}
