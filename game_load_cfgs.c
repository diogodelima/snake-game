#include "game_load_cfgs.h"
#include "strutils.h"

#include <stdio.h>
#include <string.h>



void game_cfg_print_error (int error, int line_number)
{
    switch(error) {
        case ERROR_INT_EXPECTED         : printf("%6d | Integer value expected.\n", line_number); break;
        case ERROR_TARGET_EXPECTED      : printf("%6d | 'TARGET' token expected.\n", line_number); break;
        case ERROR_END_OF_LINE_EXPECTED : printf("%6d | End of line expected.\n", line_number); break;
        case ERROR_HORIZ_VERT_EXPECTED  : printf("%6d | 'HORIZ' or 'VERT' token expected.\n", line_number); break;
        case ERROR_INVALID_COMMAND      : printf("%6d | Invalid command!\n", line_number); break;
        case ERROR_VELOCITY_EXPECTED    : printf("%6d | 'VELOCITY' must be between 100ms and 1000ms\n", line_number); break;
        case ERROR_SURROUND_EXPECTED    : printf("%6d | 'SURROUND' must be 0 or 1\n", line_number); break;
        case ERROR_COLISION_EXPECTED    : printf("%6d | 'DIE', 'IGNORE' or 'CUT' token expected.\n", line_number); break;
        
        case ERROR_WALL_WITHOUT_LEVEL   : printf("%6d | Setting a wall out of a level!\n", line_number); break;
        case ERROR_FOOD_WITHOUT_LEVEL   : printf("%6d | Setting food out of a level!\n", line_number); break;
        case ERROR_SNAKE_WITHOUT_LEVEL  : printf("%6d | Setting snake out of a level!\n", line_number); break;
        
        case ERROR_TOO_MANY_LEVELS      : printf("%6d | Defining too many Levels in game! (see MAX_LEVELS in game.h).\n", line_number); break;
        case ERROR_TOO_MANY_WALLS       : printf("%6d | Defining too many Walls in Level! (see MAX_WALLS in game.h).\n", line_number); break;
        case ERROR_TOO_MANY_FOODS       : printf("%6d | Defining too many Foods in Level! (see MAX_FOODS in game.h).\n", line_number); break;
        default: printf("Invalid Error!"); return;
    }
}


/**
 * definição do tamanho da janela tem de ser o primeiro comando no ficheiro
 * 
 * parâmetros: <window_size_x> < window_size_y>
 * 
 *     window_size_x - dimensão em pixel do comprimento da janela
 *     window_size_y - dimensão em pixel da altura da janela
 */
int game_cfg_parse_game_cmd (Game *game, const char line[], int *pos)
{
    int win_size_x;
    int win_size_y;

    if (str_next_int(line, pos, &win_size_x) != 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &win_size_y) != 0) return ERROR_INT_EXPECTED;

    game_init(game, win_size_x, win_size_y);

    return SUCCESS;
}


/**
 * cabeçalho para descrição de nível de jogo
 * 
 * parâmetros: <cells_x> <cells_y> target <target_size> <target_timeout> 
 * 
 *    cells_x        - número de quadrículas do tabuleiro de jogo em comprimento
 *    cells_y        - número de quadrículas do tabuleiro de jogo em altura
 *    target_size    - tamanho da cobrar a atingir no nível
 *    target_timeout - tempo máximo em segundos para alcançar o tamanho da cobra no nível
 */
int game_cfg_parse_level_cmd (Game *game, const char line[], int *pos)
{
    int cells_x;
    int cells_y;
    int target_size;
    int target_timeout;
    int surround;
    int velocity = 0;

    if (str_next_int(line, pos, &cells_x) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &cells_y) < 0) return ERROR_INT_EXPECTED;

    char target[MAX_COMMAND];
    if (str_next_word(line, pos, target, 7) < 0) return ERROR_TARGET_EXPECTED;

    if (str_next_int(line, pos, &target_size) < 0)    return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &target_timeout) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &surround) < 0) return ERROR_INT_EXPECTED;
    if (str_next_word(line, pos, target, 9) >= 0 && str_next_int(line, pos, &velocity) < 0) return ERROR_INT_EXPECTED;
    
    if (surround != 0 && surround != 1)
		return ERROR_SURROUND_EXPECTED;
    
    if (velocity != 0 && (velocity < 100 || velocity > 1000))
		return ERROR_VELOCITY_EXPECTED;

    if (!str_is_at_end(line, pos)) return ERROR_END_OF_LINE_EXPECTED;

    return game_add_level(game, cells_x, cells_y, target_size, target_timeout, velocity, surround);
}


/*
 * parâmetros <type> <pos_x> <pos_y> <size>
 *   
 *   type -> tipo da parede, vertical (vert) ou horizontal (horiz)
 *   pos_x -> posição (0.. cells_x - 1) do início da parede em x
 *   pos_y -> posição (0.. cells_y - 1) do início da parede em y
 *   size  -> dimensão da parede em quadrícula de jogo 
 *
 * as paredes têm de ser colocadas antes dos outros personagens do nível
 */
int game_cfg_parse_wall_cmd (Game *game, const char line[], int *pos)
{
    char typeStr[MAX_COMMAND];
    int  type;
    int  pos_x;
    int  pos_y;
    int  size;

    if (str_next_word(line, pos, typeStr, MAX_COMMAND) < 0) return ERROR_HORIZ_VERT_EXPECTED;
    
    if (strcmp("HORIZ", typeStr) == 0) type = HORIZONTAL;
    else if (strcmp("VERT", typeStr) == 0) type = VERTICAL;
    else return ERROR_HORIZ_VERT_EXPECTED;

    if (str_next_int(line, pos, &pos_x) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &pos_y) < 0) return ERROR_INT_EXPECTED;

    if (str_next_int(line, pos, &size) < 0)    return ERROR_INT_EXPECTED;
    
    if (!str_is_at_end(line, pos)) return ERROR_END_OF_LINE_EXPECTED;
    
    return game_add_wall(game, type, pos_x, pos_y, size);
}


/*
 * parâmetros: <pos_x> <pos_y> <lifetime> <energy>
 *
 *   pos_x    - posição (0.. cells_x - 1) do início da comida em x
 *   pos_y    - posição (0.. cells_y - 1) do início da comida em y
 *   lifetime - tempo de vida da comida na posição atual (em segundos)
 *   energy   - energia transmitida à cobra  (-6..-1 ou 1..6)
 */
int game_cfg_parse_food_cmd (Game *game, const char line[], int *pos)
{
    int pos_x;
    int pos_y;
    int lifetime;
    int energy;

    if (str_next_int(line, pos, &pos_x) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &pos_y) < 0) return ERROR_INT_EXPECTED;

    if (str_next_int(line, pos, &lifetime) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &energy)   < 0) return ERROR_INT_EXPECTED;
    
    if (!str_is_at_end(line, pos)) return ERROR_END_OF_LINE_EXPECTED;

    return game_add_food(game, pos_x, pos_y, lifetime, energy);
}


/*
 * descrição do estado inicial da cobra
 *
 * parâmetros: <pos_x> <pos_y> <time_to_eat> <penalty>
 * 
 *   pos_x       - posição (0.. cells_x - 1) da cabeça da cobra em x
 *   pos_y       - posição (0.. cells_y - 1) da cabeça da cobra em y
 *   time_to_eat - limite de tempo para cobra comer. 
 *                 sofre redução detemanho se não o fizer.
 *   penalty     - redução do tamanho a aplicar em caso de falta de alimento  
 */
int game_cfg_parse_snake_cmd (Game*game, const char line[], int *pos)
{
    int pos_x;
    int pos_y;
    int time_to_eat;
    int penalty;
    int colision = DIE;
    char colisionStr[] = {"DIE"};

    if (str_next_int(line, pos, &pos_x) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &pos_y) < 0) return ERROR_INT_EXPECTED;

    if (str_next_int(line, pos, &time_to_eat) < 0) return ERROR_INT_EXPECTED;
    if (str_next_int(line, pos, &penalty)   < 0) return ERROR_INT_EXPECTED;
    
    char target[MAX_COMMAND];
    if (str_next_word(line, pos, target, 9) >= 0 && str_next_word(line, pos, colisionStr, 7) < 0) return ERROR_COLISION_EXPECTED;
    
    if (strcmp("DIE", colisionStr) == 0) colision = DIE;
    else if (strcmp("IGNORE", colisionStr) == 0) colision = IGNORE;
    else if (strcmp("CUT", colisionStr) == 0) colision = CUT;
    else return ERROR_COLISION_EXPECTED;
    
    if (!str_is_at_end(line, pos)) return ERROR_END_OF_LINE_EXPECTED;

    return game_add_snake(game, pos_x, pos_y, time_to_eat, penalty, colision);
}



/**
 *
 */
int game_cfg_load_configurations (Game *game, const char pathname[])
{
    FILE *fileCfg = fopen(pathname, "r");

    if (fileCfg == NULL) {
        printf("Error opening configuration file (%s)\n", pathname);
        return -1;
    }

    char line[MAX_LINE];
    int  line_number = 0;


    while (fgets(line, MAX_LINE, fileCfg) != NULL) {
        
        line_number++;
        
        str_trim_comments(line, LINE_COMMENT);
        str_trim(line);
        if (str_is_empty(line)) continue;

        str_to_upper(line);

        char command[MAX_COMMAND];
        int  pos   = 0;
        int  error = SUCCESS;

        str_next_word(line, &pos, command, MAX_COMMAND);

        if (strcmp("GAME",  command) == SUCCESS) {
            error = game_cfg_parse_game_cmd(game, line, &pos);
            if (error < 0) {
                game_cfg_print_error(error, line_number);
                return error;
            }  
        } 
        else if (strcmp("LEVEL", command) == SUCCESS) {
            error = game_cfg_parse_level_cmd(game, line, &pos);
            if (error < 0) {
                game_cfg_print_error(error, line_number);
                return error;
            } 
        }
        else if (strcmp("WALL", command) == SUCCESS) {
            error = game_cfg_parse_wall_cmd(game, line, &pos);
            if (error < 0) {
                game_cfg_print_error(error, line_number);
                return error;
            } 
        }
        else if (strcmp("FOOD", command) == SUCCESS) {
            error = game_cfg_parse_food_cmd(game, line, &pos);
            if (error < 0) {
                game_cfg_print_error(error, line_number);
                return error;
            } 
        }        
        else if (strcmp("SNAKE", command) == 0) {
            error = game_cfg_parse_snake_cmd(game, line, &pos);
            if (error < 0) {
                game_cfg_print_error(error, line_number);
                return error;
            } 
        }
        else {
            error = ERROR_INVALID_COMMAND;
            if (error < 0) {
                game_cfg_print_error(error, line_number);
                return error;
            } 
        }
    }
    fclose(fileCfg);
    return SUCCESS;
}
