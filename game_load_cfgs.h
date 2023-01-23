#ifndef _GAME_LOAD_CFGS_H
#define _GAME_LOAD_CFGS_H

#include "game.h"


#define LINE_COMMENT    '#'

#define MAX_PATHNAME    4096
#define MAX_LINE         256
#define MAX_COMMAND       32

#define SUCCESS                      0

#define ERROR_INT_EXPECTED          -1
#define ERROR_TARGET_EXPECTED       -2
#define ERROR_END_OF_LINE_EXPECTED  -3
#define ERROR_HORIZ_VERT_EXPECTED   -4
#define ERROR_INVALID_COMMAND       -5
#define ERROR_VELOCITY_EXPECTED     -13
#define ERROR_COLISION_EXPECTED     -14
#define ERROR_SURROUND_EXPECTED     -15

#define ERROR_WALL_WITHOUT_LEVEL    -6
#define ERROR_FOOD_WITHOUT_LEVEL    -7
#define ERROR_SNAKE_WITHOUT_LEVEL   -8

#define ERROR_TOO_MANY_LEVELS       -9
#define ERROR_TOO_MANY_WALLS       -10
#define ERROR_TOO_MANY_FOODS       -11

#define ERROR_NO_LEVELS_DEFINED    -12


int game_cfg_load_configurations (Game *game, const char pathname[]);

#endif
