#ifndef GUI_H
#define GUI_H
#include "2d_graphic.h"
#include "ffont.h"


/********************************
# Base Class
# Without `element` the GUI cannot be managed

* Created By:		Josh Cooper
* Modified By:		Josh Cooper	on Dec. 25, 2014
********************************/
class element
{
	public:
		glm::vec3 position;
		virtual void Draw() = 0;
};

/********************************
# Window Class
# This exists so that N elements can be nested within a single element (a window)

* Created By:		Josh Cooper
* Modified By:		Josh Cooper	on Dec. 30, 2014
********************************/
class window : public element
{
	private:
		graphic* my_graphic;
		std::vector<element*> window_elements;

	public:
		window( graphic* my_graphic, std::vector<element*> window_elements )
		{
			this->my_graphic = my_graphic;
			this->window_elements.swap( window_elements );
		}
		virtual void Draw();
};

/********************************
# Text Class
# Draws text

* Created By:		Josh Cooper
* Modified By:		Josh Cooper	on Dec. 30, 2014
********************************/
class text : public element
{
	protected:
		std::string my_string;

	public:
		text( std::string my_text )
		{
			this->my_string = my_text;
		}
		virtual void Draw();
};

/********************************
# Button Class
# Interactive button

* Created By:		Josh Cooper
* Modified By:		Josh Cooper	on Dec. 30, 2014
********************************/
class button : public element
{
	private:
		graphic* my_graphic;
		text* my_text_ptr;

	public:
		button( graphic* my_graphic, text* text_ptr )
		{
			this->my_graphic = my_graphic;
			this->my_text_ptr = text_ptr;
		}
		virtual void Draw();
};

/********************************
# Dynamic Text Class
# Draw variables as text without changing the text every time the variable values change
# ex:  base string: "Health: %i"
# ex:  output: "Health: 210"

* Created By:		Josh Cooper
* Modified By:		Josh Cooper	on Dec. 30, 2014
********************************/
class dynamic_text : public text
{
	private:
		std::vector<void*> variable_list;

	public:
		dynamic_text( std::string my_text, std::vector<void*> variable_list ) : text( my_text )
		{
			this->variable_list.swap( variable_list );
		}
		virtual void Draw();
};

#endif
