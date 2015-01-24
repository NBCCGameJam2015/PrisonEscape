#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <blit3d\Blit3D.h>

#include <mutex>

#include "game.h"


extern Blit3D* get_blit3d();
extern game& get_game_engine();

extern string textures;
extern string models;
extern string graphic_dir;
extern string otherfiles;

extern FFont Fcout;
extern std::mutex View_Mutex;
extern glm::mat4 View_Matrix;
extern glm::mat4 View_Angle;
extern glm::mat4 View_Position;

extern unsigned int seed_value;
extern unsigned int seed_value2;

void **Matrix2D( int ncols, int  nrows, int element_size, int first_row, int first_col );
extern void Init();
extern void DeInit();
extern void Update(double seconds);
extern void Draw();
extern void DoInput(int key, int scancode, int action, int mods);
extern void DoCursor(double x, double y);

#endif
