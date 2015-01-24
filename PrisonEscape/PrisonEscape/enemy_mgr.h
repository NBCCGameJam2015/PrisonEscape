#ifndef ENEMY_MGR_H_
#define ENEMY_MGR_H_

#include "Enemy.h"
#include <vector>

class Enemy_Mgr
{
private:
	std::vector<Enemy*>& Get_Enemy_List_Instance()
	{
		static std::vector<Enemy*> Enemy_List;
		return Enemy_List;
	}
protected:
	/*inline unsigned int hash( int x, int y )
	{
		union local_union
		{
			short myarray[2];
			unsigned int R;
		};
		local_union my_var;
		my_var.myarray[0] = x;
		my_var.myarray[1] = y;
		return my_var.R;
	}*/
public:
	Enemy* Get(int x, int y)
	{
		for ( auto Enemy : Get_Enemy_List_Instance() )
		{
			if ( Enemy->x == x && Enemy->y == y )
			{
				return Enemy;
			}
		}
		return nullptr;
	}
	void Add( Enemy* e )
	{
		Get_Enemy_List_Instance().push_back( e );
	}

};


#endif
