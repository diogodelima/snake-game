#include "game.h"
#include "game_load_cfgs.h"
#include "gameboard.h"


#define CFG_PATHNAME    "levels.txt"

#define WINDOW_WIDTH    1200
#define WINDOW_HEIGHT    800

#define SNAKE_VELOCITY   250
#define TICKS_PER_SECOND (1000 / SNAKE_VELOCITY)


Game      game;
// GameBoard gameboard;


#define KEY_SPACE           ' '
#define KEY_RIGHT_ARROW     0x4000004f
#define KEY_LEFT_ARROW      0x40000050
#define KEY_DOWN_ARROW      0x40000051
#define KEY_UP_ARROW        0x40000052

void mouseHandler(MouseEvent me)
{
    // printf("(%d, %d)\n", me.x, me.y);
}


int ticks = 0;

void timerHandler(){
	
	if (game.ended)
		return;
		
	int ticks_per_second = TICKS_PER_SECOND;
	int velocity = game.levels[game.currentLevel].velocity;
	
	if (velocity != 0)
		ticks_per_second = 1000 / velocity;
	
    ticks = (ticks + 1) % ticks_per_second;
    if (ticks == 0) {
        game_second_passed(&game);
    }

    // TODO: chamar uma função de game para realizar movimento, por exemplo:
    Snake *snake = &game.levels[game.currentLevel].snake;
    int value = game_snake_move_one_step(&game);
    int energy = snake -> foodEnergy;
    TextBox textBox;
    int x = game.gameboard.board_width / 2;
    int y = game.gameboard.board_height / 2;	
    
    switch (value){
		
		case -1:
			//snake eat food
			break;
		case -2:
			//snake hit wall
			textbox_init_text(&textBox, x, y, "Snake died! Game over", TEXT_LARGE_FONT, DEAD_BACK_COLOUR, DEAD_TEXT_COLOUR);
			x -= textbox_get_size(&textBox).width / 2;
			textbox_set_position(&textBox, x, y);
			textbox_show(&textBox);
			game.reason = -2;
			game.ended = true;
			break;
		case -3:
			//snake hit body
			textbox_init_text(&textBox, x, y, "Snake died! Game over", TEXT_LARGE_FONT, DEAD_BACK_COLOUR, DEAD_TEXT_COLOUR);
			x -= textbox_get_size(&textBox).width / 2;
			textbox_set_position(&textBox, x, y);
			textbox_show(&textBox);
			game.reason = -3;
			game.ended = true;
			break;
		case -4:
			//level finished
			if (game.currentLevel == game.nlevels - 1)
				textbox_init_text(&textBox, x, y, "Last level terminated", TEXT_LARGE_FONT, LEVELPASS_BACK_COLOUR, LEVELPASS_TEXT_COLOUR);
			else textbox_init_text(&textBox, x, y, "Level terminated", TEXT_LARGE_FONT, LEVELPASS_BACK_COLOUR, LEVELPASS_TEXT_COLOUR);
			x -= textbox_get_size(&textBox).width / 2;
			textbox_set_position(&textBox, x, y);
			textbox_show(&textBox);
			game.reason = -4;
			game.ended = true;
			break;
		case -5:
			//timeout
			textbox_init_text(&textBox, x, y, "Level Timeout. Game Over", TEXT_LARGE_FONT, DEAD_BACK_COLOUR, DEAD_TEXT_COLOUR);
			x -= textbox_get_size(&textBox).width / 2;
			textbox_set_position(&textBox, x, y);
			textbox_show(&textBox);
			game.reason = -5;
			game.ended = true;
			break;
		case 0:
			//success
			break;
		default:
			printf("ERROR: SNAKE MOVE ONE STEP");
		
	}
	
	TextBox snake_size_tb = game.gameboard.snake_size_tb;
	char str_size[] = "Size: ";
	char str_size1[20];
	snprintf (str_size1, 15, "%s%.3d", str_size, snake -> body.size);
	textbox_set_text(&snake_size_tb, str_size1);
	textbox_show(&snake_size_tb);
	
	if (energy != 0){
		
		snake -> eating = true;
		if (energy < 0){
			
			for (int i = 0; i < snake -> body.size; i++){
				Point part = snake -> body.parts[i];
				gameboard_clean_cell(&game.gameboard, part.x, part.y);
			}
			
			snake -> body.size--;
			snake -> foodEnergy++;	
		}
		
		if (energy > 0){
			snake -> body.size++;
			snake -> foodEnergy--;
		}
		
		//int squares = game.levels[game.currentLevel].cells_x * game.levels[game.currentLevel].cells_y - snake -> body.size - 1 - level_get_wall_count(&game.levels[game.currentLevel]); //falta remover as quadrículas das walls
		
		if (snake -> body.size <= 0){
			//game end, loss
			exit(0);
			return;
		}
		
		/*if (squares <= 0){
			//game end, victory
			exit(0);
			return;
		}*/
	}else snake -> eating = false;
	
}

void keyboardHandler(KeyEvent ke)
{
	Snake *snake = &game.levels[game.currentLevel].snake;
    if ( ke.state == KEY_PRESSED ) {
        // printf("[pressed] 0x%x %d - %c\n", ke.keysym, ke.keysym, ke.keysym);
    
        switch (ke.keysym) {

            case KEY_SPACE:
            
				if (game.ended && (game.currentLevel >= game.nlevels - 1 || game.reason == -2 || game.reason == -3 || game.reason == -5))
					exit(0);
					
				if (!game.ended)
					return;
            
                // go to next level
                game_next_level(&game);
                game_update_board(&game);
                game_draw_board(&game);
                int velocity = game.levels[game.currentLevel].velocity;
                if (velocity != 0)
					graph_regist_timer_handler(timerHandler, velocity);
				else graph_regist_timer_handler(timerHandler, SNAKE_VELOCITY);
                break;
                
            case KEY_RIGHT_ARROW:
				
				if (snake -> direction == RIGHT || snake -> direction == LEFT)
					break;
				
				snake -> direction = RIGHT;
				break;
			case KEY_LEFT_ARROW:
			
				if (snake -> direction == RIGHT || snake -> direction == LEFT)
						break;
			
				snake -> direction = LEFT;
				break;
			case KEY_DOWN_ARROW:
			
				if (snake -> direction == UP || snake -> direction == DOWN)
						break;
			
				snake -> direction = DOWN;
				break;
			case KEY_UP_ARROW:
			
				if (snake -> direction == UP || snake -> direction == DOWN)
					break;
			
				snake -> direction = UP;
				break;
        }
    }
}

int main ()
{
    // Init game from file configuration
    if (!game_cfg_load_configurations(&game, CFG_PATHNAME) == SUCCESS) {
        printf("ERROR loading configuration file\n");
        return -1;
    }

    graph_init2("Snake Game ...", game_get_win_width(&game), game_get_win_height(&game));

    // graph_regist_mouse_handler(mouseHandler);
    graph_regist_key_handler  (keyboardHandler);
    int velocity = game.levels[game.currentLevel].velocity;
    if (velocity != 0)
		graph_regist_timer_handler(timerHandler, velocity);
	else graph_regist_timer_handler(timerHandler, SNAKE_VELOCITY);

    game_init_board(&game);
    game_draw_board(&game);

    graph_start();

    return 0;
}
