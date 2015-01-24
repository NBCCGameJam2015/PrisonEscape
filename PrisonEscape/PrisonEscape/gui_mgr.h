#ifndef GUI_MGR_H
#define GUI_MGR_H
#include <vector>
#include "gui.h"


//must have singleton type of access
class GUI_Manager
{
private:
	std::vector<element*> gui_elements;

public:
	void Draw();
};

#endif
