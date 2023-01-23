#include "game_load_cfgs.h"
#include "game.h"


int aleatorio( int inf, int sup ){ /// retorna um nº aleatório entre inf, sup
  return (rand()%(sup-inf+1)+inf);
}

//-----------------------------------------------------------------------------
//    Point
//----------------------------------------------------------------------------- 
void point_init (Point *p, int x, int y)
{
    p->x = x;
    p->y = y;
}

//-----------------------------------------------------------------------------
//    Snake
//----------------------------------------------------------------------------- 

void snake_init (Snake *snake, int pos_x, int pos_y, int time_to_eat, int penalty, int colision)
{
    point_init(&snake->pos, pos_x, pos_y);
    snake->time_to_eat   = time_to_eat;
    snake->penalty       = penalty;
    snake->direction     = RIGHT;
    snake->foodEnergy    = 0;
    snake->colision      = colision;
    snake->eating        = false;
    Body body;
    body.size            = 0;
    snake-> body         = body;
}

Point snake_draw (const Snake *snake, const GameBoard *gb)
{
    // draw snake head 
    gameboard_draw_snake_head(gb, snake->pos.x, snake->pos.y, snake->direction);

    // TODO: draw snake body if exist
    Point pos;
    pos.x = -1;
	pos.y = -1;
    if (snake -> body.size > 0 && snake -> foodEnergy >= 0){
		
		int x = snake -> pos.x;
		int y = snake -> pos.y;
		
		switch (snake -> direction){
			
			case RIGHT:
				x--;
				if (x < 0)
					x = gb -> n_cell_x - 1;
				gameboard_paint_cell(gb, x, y, GAMEBOARD_COLOUR_SNAKE_BODY);
				break;
			case LEFT:
				x++;
				if (x >= gb -> n_cell_x)
					x = 0;
				gameboard_paint_cell(gb, x, y, GAMEBOARD_COLOUR_SNAKE_BODY);
				break;
			case UP:
				y++;
				if (y >= gb -> n_cell_y)
					y = 0;
				gameboard_paint_cell(gb, x, y, GAMEBOARD_COLOUR_SNAKE_BODY);
				break;
			case DOWN:
				y--;
				if (y < 0)
					y = gb -> n_cell_y - 1;
				gameboard_paint_cell(gb, x, y, GAMEBOARD_COLOUR_SNAKE_BODY);
				break;
			default:
				printf("ERROR: SNAKE DIRECTION\n");
			
		}
		
		point_init(&pos, x, y);
		return pos;
	}
	
	return pos;
	
}



//-----------------------------------------------------------------------------
//    Wall
//-----------------------------------------------------------------------------

void wall_init (Wall *wall, int pos_x, int pos_y, int type, int size)
{
    point_init(&wall->start, pos_x, pos_y);
    wall->type  = type;
    wall->size  = size;
}

void wall_draw  (const Wall *wall, const GameBoard *gb)
{
    gameboard_draw_wall(gb, wall->start, wall->type, wall->size);
}

//-----------------------------------------------------------------------------
//    Food
//-----------------------------------------------------------------------------

void food_init (Food *food, int pos_x, int pos_y, int lifetime, int energy)
{
    point_init(&food->pos, pos_x, pos_y);
    food->lifetime = lifetime;
    food->energy   = energy;
    food->eated    = false;
    
    Timer timer;
    int minutes = lifetime / 60;
    int seconds = lifetime % 60;
    timer_set(&timer, minutes, seconds);
    food -> timer = timer;
}

void food_draw (const Food *food, const GameBoard *gb)
{
    gameboard_draw_food(gb, food->pos, food->energy);
}

void spawnFood(Food *food, Game *game, Level level){
	
	while (true){
		
		Snake snake = level.snake;
		int x = aleatorio(0, level.cells_x - 1);
		int y = aleatorio(0, level.cells_y - 1);
		
		if (food -> energy < 0 && (x - snake.pos.x < 0 || y - snake.pos.y < 0))
			continue;
			
		bool c = false;
		
		for (int i = 0; i < snake.body.size; i++){
			
			Point pos = snake.body.parts[i];
			if (pos.x == x || pos.y == y)
				c = true;
			
		}
		
		for (int i = 0; i < level.nwalls; i++){
			
			Wall wall = level.walls[i];
			
			if (wall.type == VERTICAL)
				for (int j = wall.start.y; j < (wall.start.y + wall.size); j++)
					if (wall.start.x == x && j == y)
						c = true;
				
			if (wall.type == HORIZONTAL)
				for (int j = wall.start.x; j < (wall.start.x + wall.size); j++)
					if (wall.start.y == y && j == x)
						c = true;
			
		}
		
		if (c) continue;
		
		Point pos;
		point_init(&pos, x, y);
		
		gameboard_clean_cell(&game -> gameboard, food -> pos.x, food -> pos.y);
		gameboard_draw_food (&game -> gameboard, pos, food -> energy);
		food -> pos = pos;
		food -> eated = false;
		break;
	}
	
}

//-----------------------------------------------------------------------------
//    Level
//-----------------------------------------------------------------------------

void level_init (Level *level, int cell_x, int cell_y, int target_size, int target_timeout, int velocity, int surround)
{
    level->cells_x        = cell_x;
    level->cells_y        = cell_y;
    level->target_size    = target_size;
    level->target_timeout = target_timeout;
    level->nwalls         = 0;
    level->nfood          = 0;
    level->velocity       = velocity;
    level->surround       = surround;
}

int level_add_wall (Level *level, int type, int pos_x, int pos_y, int size) 
{
    if (level->nwalls == MAX_WALLS) return ERROR_TOO_MANY_WALLS;

    wall_init(&level->walls[level->nwalls++], pos_x, pos_y, type, size);
    return SUCCESS;
}

int level_add_food (Level *level, int pos_x, int pos_y, int lifetime, int energy)
{
    if (level->nfood == MAX_FOOD) return ERROR_TOO_MANY_FOODS;

    food_init(&level->food[level->nfood++], pos_x, pos_y, lifetime, energy);

    return SUCCESS;
}

int level_add_snake (Level *level, int pos_x, int pos_y, int time_to_eat, int penalty, int colision)
{
    snake_init(&level->snake, pos_x, pos_y, time_to_eat, penalty, colision);

    return SUCCESS;
}


int level_get_n_cells_x (const Level *level)
{
    return level->cells_x;
}

int level_get_n_cells_y (const Level *level)
{
    return level->cells_y;
}

void level_draw (const Level *level, const GameBoard *gb)
{
    for (int i = 0; i < level->nfood; i++) {
        food_draw(&level->food[i], gb);
    }

    for (int i = 0; i < level->nwalls; i++) {
        wall_draw(&level->walls[i], gb);
    }
    snake_draw(&level->snake, gb);
}

int level_get_wall_count(const Level *level){
	
	int count = 0;
	
	for (int i = 0; i < level -> nwalls; i++)
		count += level -> walls[i].size;
	
	return count;
}

//-----------------------------------------------------------------------------
//    Game
//-----------------------------------------------------------------------------

void game_init (Game *game, int win_size_x, int win_size_y)
{
    game->window_width  =  win_size_x;
    game->window_height =  win_size_y;
    game->nlevels       =   0;
    game->currentLevel  =  -1;
    game -> ended       = false;
}

int game_add_level (Game *game, int cell_x, int cell_y, int target_size, int target_timeout, int velocity, int surround)
{
    if (game->nlevels == MAX_LEVELS) return ERROR_TOO_MANY_LEVELS;

    level_init(&game->levels[game->nlevels++], cell_x, cell_y, target_size, target_timeout, velocity, surround);

    // At least one level already exists - setting current level to first level
    if (game->currentLevel < 0) game->currentLevel = 0;
    return SUCCESS;
}

int game_add_wall  (Game *game, int type, int pos_x, int pos_y, int size)
{
    if (game->nlevels == 0) return ERROR_WALL_WITHOUT_LEVEL;

    return level_add_wall(&game->levels[game->nlevels - 1], type, pos_x, pos_y, size);
}

int game_add_food  (Game *game, int pos_x, int pos_y, int lifetime, int energy)
{
    if (game->nlevels == 0) return ERROR_FOOD_WITHOUT_LEVEL;

    return level_add_food(&game->levels[game->nlevels - 1], pos_x, pos_y, lifetime, energy);
}

int game_add_snake (Game *game, int pos_x, int pos_y, int time_to_eat, int penalty, int colision)
{
    if (game->nlevels == 0) return ERROR_SNAKE_WITHOUT_LEVEL;

    return level_add_snake(&game->levels[game->nlevels - 1], pos_x, pos_y, time_to_eat, penalty, colision);
}


int game_get_win_width (const Game *game)
{
    return game->window_width;
}

int game_get_win_height (const Game *game)
{
    return game->window_height;
}

int game_next_level (Game *game) 
{
    if (game->nlevels == 0) return -1;

	game -> ended = false;
    game->currentLevel++;
    if (game->currentLevel >= game->nlevels) game->currentLevel = 0;
    return game->currentLevel;
}

int game_get_level_cells_x (const Game *game)
{
    return level_get_n_cells_x(&game->levels[game->currentLevel]);
}

int game_get_level_cells_y (const Game *game)
{
    return level_get_n_cells_y(&game->levels[game->currentLevel]);
}


void game_init_board (Game *game)
{
    int n_cells_x = level_get_n_cells_x(&game->levels[game->currentLevel]);
    int n_cells_y = level_get_n_cells_y(&game->levels[game->currentLevel]);
    int level = game -> currentLevel + 1;
    
    gameboard_init(&game->gameboard, n_cells_x, n_cells_y, level, game -> levels[game->currentLevel].snake.body.size);
}

void game_update_board (Game *game)
{
    int n_cells_x = level_get_n_cells_x(&game->levels[game->currentLevel]);
    int n_cells_y = level_get_n_cells_y(&game->levels[game->currentLevel]);
    
    gameboard_update(&game->gameboard, n_cells_x, n_cells_y);
}



void game_draw_board (Game *game)
{
    if (game->currentLevel < 0) return;

    // draw all game board include header
    gameboard_draw(&game->gameboard);
    
    if (game->levels[game->currentLevel].surround == 1){
		level_add_wall(&game->levels[game->currentLevel], HORIZONTAL, 0, 0, game->levels[game->currentLevel].cells_x);
		level_add_wall(&game->levels[game->currentLevel], HORIZONTAL, 0, game->levels[game->currentLevel].cells_y - 1, game->levels[game->currentLevel].cells_x);
		level_add_wall(&game->levels[game->currentLevel], VERTICAL, 0, 0, game->levels[game->currentLevel].cells_y);
		level_add_wall(&game->levels[game->currentLevel], VERTICAL, game->levels[game->currentLevel].cells_x - 1, 0, game->levels[game->currentLevel].cells_y);
	}
   
    // draw level objects: snake, walls, foods
    level_draw(&game->levels[game->currentLevel], &game->gameboard);
    int level = game -> currentLevel + 1;
    gameboard_init(&game -> gameboard, game->levels[game->currentLevel].cells_x, game -> levels[game -> currentLevel].cells_y, level, game -> levels[game->currentLevel].snake.body.size);
}

void game_second_passed (Game *game)
{
	
    gameboard_second_passed(&game->gameboard);
    
    for (int i = 0; i < game -> levels[game -> currentLevel].nfood; i++){
		
		Food *food = &game -> levels[game -> currentLevel].food[i];
		
		timer_down_one_second(&food -> timer);
		
		if (timer_is_timeout(&food -> timer)){
			spawnFood(food, game, game -> levels[game -> currentLevel]);
			int minutes = food -> lifetime / 60;
			int seconds = food -> lifetime % 60;
			timer_set(&food -> timer, minutes, seconds);
		}
		
	}
    
}

bool game_snake_eat_food(Game *game, int x, int y){
	
	Level *level = &game -> levels[game -> currentLevel];
	Snake *snake = &level -> snake;
	
	for (int i = 0; i < level -> nfood; i++){
		
		Food *food = &level -> food[i];
		
		if (food -> pos.x == x && food -> pos.y == y && !food -> eated){
			food -> eated = true;
			snake -> foodEnergy += food -> energy;
			return true;
		}
		
	}
	
	return false;
}

bool game_snake_colision_wall(Game *game, int x, int y){
	
	Level level = game -> levels[game -> currentLevel];

	for (int i = 0; i < level.nwalls; i++){
			
		Wall wall = level.walls[i];
		
		if (wall.type == VERTICAL)
			for (int j = wall.start.y; j < (wall.start.y + wall.size); j++)
				if (wall.start.x == x && j == y)
					return true;
			
		if (wall.type == HORIZONTAL)
			for (int j = wall.start.x; j < (wall.start.x + wall.size); j++)
				if (wall.start.y == y && j == x)
					return true;
			
	}

	return false;
}

bool game_snake_colision_itself(Game *game, int x, int y){
	
	Level *level = &game -> levels[game -> currentLevel];
	Snake *snake = &level -> snake;
	int index = 0;
	
	for (int i = 0; i < level -> snake.body.size; i++){
		
		Point part = snake -> body.parts[i];
		
		if (x == part.x && y == part.y && level -> snake.colision != IGNORE){
			
			if (level -> snake.colision == DIE)
				return true;
	
			if (level -> snake.colision == CUT){
				
				while (level -> snake.body.parts[index].x != part.x || level -> snake.body.parts[index].y != part.y)
					index++;
					
				Point parts[MAX_PARTS];
				
				int j = 0;
				for (int i = index + 1; i <= snake -> body.size; i++){
					parts[j] = snake -> body.parts[i];
					j++;
				}
				
				snake -> body.size -= index + 1;
				
				for (int i = 0; i <= index; i++)
					gameboard_clean_cell(&game -> gameboard, snake -> body.parts[i].x, snake -> body.parts[i].y);
				
				for (int i = 0; i < snake -> body.size; i++)
					snake -> body.parts[i] = parts[i];
				
			}
				
		}
		
	}
	
	return false;
}

/*
 * TODO:
 *  Esta função tem por objetivo mover a cobra uma casa na direcção estabelecida, 
 *  atualizando os campos da estrutura snake, verificando colisões com parede ou comida 
 *  actualizando a posição no tabuleiro
 *  Return: 0 se movimento com sucesso; -1 se comeu, -2 se colidiu com uma parede, -3 se morreu ao colidir com ela própria ou ao sair do mapa, -4 se atingiu o tamanho máximo do level e -5 se o tempo acabou
 */
int game_snake_move_one_step (Game *game)
{
    
    Timer timer = game -> gameboard.timeout_tm;
    Level level = game -> levels[game -> currentLevel];
    Snake *snake = &game -> levels[game -> currentLevel].snake;
	int x = snake -> pos.x;
	int y = snake -> pos.y;
	gameboard_clean_cell(&game -> gameboard, x, y);
    
    switch (snake -> direction){
		
		case RIGHT:
			x++;
			if (x >= level.cells_x)
				x = 0;
			break;
		case LEFT:
			x--;
			if (x < 0)
				x = level.cells_x - 1;
			break;
		case UP:
			y--;
			if (y < 0)
				y = level.cells_y - 1;
			break;
		case DOWN:
			y++;
			if (y >= level.cells_y)
				y = 0;
			break;
		default:
			printf("ERROR: SNAKE DIRECTION\n");
		}
		
	Point pos;
	pos.x = x;
	pos.y = y;
	
	snake -> pos = pos;
	Point lastPos = snake_draw(&game -> levels[game -> currentLevel].snake, &game -> gameboard);

	if (snake -> eating){
		int index = snake -> body.size - 1;
		snake -> body.parts[index] = lastPos;
	}else if (snake -> body.size >= 1){
		
		Point part = snake -> body.parts[0];
		gameboard_clean_cell(&game -> gameboard, part.x, part.y);
		Point parts[MAX_PARTS];
		
		for (int i = 1; i < snake -> body.size; i++){
			parts[i - 1] = snake -> body.parts[i];
			parts[i] = lastPos;
		}
		
		for (int i = 0; i < snake -> body.size; i++)
			snake -> body.parts[i] = parts[i];
		
	}
	
	if (game_snake_eat_food(game, x, y))
		return -1;
	
	if (game_snake_colision_wall(game, x, y))
		return -2;
	
	if (game_snake_colision_itself(game, x ,y))
		return -3;
	
	if (level.snake.body.size >= level.target_size)
		return -4;
		
	if (timer_is_timeout(&timer))
		return -5;
    
    return 0;
}
