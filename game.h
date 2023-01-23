#ifndef _GAME_H
#define _GAME_H

#include "pg/pglib.h"
#include "gameboard.h"
#include "game_common.h"

#define MAX_PARTS 1000 // maximum parts of snake body


void point_init (Point *p, int x, int y);


// ENTITIES

//-----------------------------------------------------------------------------
//    Snake
//----------------------------------------------------------------------------- 

typedef struct{
	
	int size;      // the size of snake body
	Point parts[MAX_PARTS];   // all the parts of snake body
	
} Body;

#define DIE    0   // die colision
#define IGNORE 1   // ignore colision
#define CUT    2   // cut colision

typedef struct {
    Point pos;                  // head position on grid coordinates
    int   time_to_eat;          // a penalty occurrs if snake doesn't eat before this time
    int   penalty;              // the penalty size if the snake doesn't eat in time
    int foodEnergy;             // energy of food to increase or decrease snake body
    int colision;               // colision type (die, ignore or cut)
    bool eating;                // snake is eating
    Body  body;                 // snake body
    Direction direction;        // the direction of snake head
} Snake;

void snake_init  (Snake *snake, int pos_x, int pos_y, int time_to_eat, int penalty, int colision);

Point snake_draw  (const Snake *snake, const GameBoard *gb);



//-----------------------------------------------------------------------------
//    Wall
//-----------------------------------------------------------------------------
#define HORIZONTAL  1           // horizontal wall
#define VERTICAL    2           // vertical wall

typedef struct {
    Point start;                // wall start location
    int   type;                 // horizontal or vertical wall
    int   size;                 // wall size
} Wall;


void wall_init  (Wall *wall, int pos_x, int pos_y, int type, int size);

void wall_draw  (const Wall *wall, const GameBoard *gb);

//-----------------------------------------------------------------------------
//    Food
//-----------------------------------------------------------------------------
typedef struct {
    Point pos;
    int   energy;               // define the snake grow if eaten
                                // if negative tell the snake reduction if eaten
    int   lifetime;             // time to life in seconds
    bool  eated;                // food is eated
    Timer timer;
} Food;

void food_init (Food *food, int pos_x, int pos_y, int energy, int lifetime);

void food_draw (const Food *food, const GameBoard *gb);


//-----------------------------------------------------------------------------
//    Level
//-----------------------------------------------------------------------------
#define MAX_WALLS  15
#define MAX_FOOD 4

typedef struct {
    int  cells_x;               // width cells number
    int  cells_y;               // height cells number
    int  target_size;           // snake size to reach in level
    int  target_timeout;        // max time to reach target
    int  nwalls;                // number of level walls
    int velocity;               // speed of snake
    int surround;               //surround map
    Wall walls[MAX_WALLS];
    int  nfood;                 // number of level food items
    Food food[MAX_FOOD];
   
    Snake snake;                // the snake
} Level;


void level_init          (Level *level, int cell_x, int cell_y, int target_size, int target_timeout, int velocity, int surround);
int  level_add_wall      (Level *level, int type, int pos_x, int pos_y, int size);
int  level_add_food      (Level *level, int pos_x, int pos_y, int lifetime, int energy);
int  level_add_snake     (Level *level, int pos_x, int pos_y, int time_to_eat, int penalty, int colision);

int  level_get_n_cells_x (const Level *level);

int  level_get_n_cells_y (const Level *level);

void level_draw          (const Level *level, const GameBoard *gb);

int level_get_wall_count(const Level *level);

//-----------------------------------------------------------------------------
//    Game
//-----------------------------------------------------------------------------
#define MAX_LEVELS 10

typedef struct {
    int       window_width;         // the game window width  
    int       window_height;        // the game windows height
    int       nlevels;              // number of game levels
    Level     levels[MAX_LEVELS];   // the levels
    int       currentLevel;         // current game level
    bool      ended;                // the game is finished
    int       reason;               // reason why the game ended
    GameBoard gameboard;
} Game;

void game_init              (Game *game, int win_size_x, int win_size_y);
int  game_add_level         (Game *game, int cell_x, int cell_y, int target_size, int target_timeout, int velocity, int surround);
int  game_add_wall          (Game *game, int type, int pos_x, int pos_y, int size);
int  game_add_food          (Game *game, int pos_x, int pos_y, int lifetime, int energy);
int  game_add_snake         (Game *game, int pos_x, int pos_y, int time_to_eat, int penalty, int colision);


int  game_get_win_width     (const Game *game);

int  game_get_win_height    (const Game *game);

int  game_next_level        (Game *game);

int  game_get_level_cells_x (const Game *game);

int  game_get_level_cells_y (const Game *game);

void game_init_board        (Game *game);

void game_update_board      (Game *game);

void game_draw_board        (Game *game);

void game_second_passed     (Game *game);

bool game_snake_eat_food(Game *game, int x, int y);

bool game_snake_colision_itself(Game *game, int x, int y);

bool game_snake_colision_wall(Game *game, int x, int y);

int game_snake_move_one_step (Game *game);

#endif
