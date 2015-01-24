#include "global.h"
#include "gui.h"

void window::Draw()
{
	my_graphic->Draw();
	for ( auto iter = window_elements.begin(); iter != window_elements.end(); ++iter )
	{
		( *iter )->Draw();
	}
}

void text::Draw()
{
	Fcout.text_position = position;
	Fcout.print( my_string );
}

void button::Draw()
{
	my_graphic->Draw();
	my_text_ptr->Draw();
}

/********************************
# Dynamic Text :: Draw
# Takes a string and replaces key words with variable values found using a list of pointers

* Created By:		Josh Cooper
* Modified By:		Josh Cooper	on Dec. 30, 2014
********************************/
void dynamic_text::Draw()
{
	std::string output;
	std::string var_value;
	auto var_iter = variable_list.begin();

	for ( int i = 0; i < my_string.size(); ++i )
	{
		char temp = my_string[i];
		if ( temp == '%' )
		{
			char first = my_string[++i];
			char second = my_string[++i];

			if ( second == '%' )
			{
				switch ( first )
				{
					case 'i':
						var_value = std::to_string( *(int*)( *var_iter ) );
						break;

					case 'f':
						var_value = std::to_string( *(float*)( *var_iter ) );
						break;

					case 'd':
						var_value = std::to_string( *(double*)( *var_iter ) );
						break;

					default:
						assert( true );
				}
				output.append( var_value );
			}
			else if ( my_string[++i] == '%' )
			{
				switch ( temp )
				{
					case 'f':
						var_value = std::to_string( ( *(float*)( *var_iter ) ) * 100 );
						break;

					case 'd':
						var_value = std::to_string( ( *(double*)( *var_iter ) ) * 100 );
						break;

					default:
						assert( true );
				}
				output.append( var_value );
			}
			else
			{
				assert( true );
			}
		}
		else
		{
			output.append( 1, temp );
		}
	}
	Fcout.text_position = position;
	Fcout.print( output );
}