#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H
#include <pg/pglib.h>

#include "game_common.h"
#include "text_box.h"
#include "timer.h"


// colours
#define GAMEBOARD_COLOUR_WINDOW_BACKGROUND    c_black
#define GAMEBOARD_COLOUR_SNAKE_HEAD           c_orange
#define GAMEBOARD_COLOUR_SNAKE_BODY           graph_rgb(207, 169, 135)
#define GAMEBOARD_COLOUR_FOOD_GOOD            graph_rgb(188, 255, 190)
#define GAMEBOARD_COLOUR_FOOD_BAD             graph_rgb(195, 0, 0)
#define GAMEBOARD_COLOUR_WALL                 c_dgray
#define GAMEBOARD_COLOUR_ARENA                graph_rgb(0, 118, 58)
#define GAMEBOARD_COLOUR_ARENA_GRID           graph_rgb(0, 138, 58)

// Percentagem da área das células do tabuleiro que é usada para desenho
#define GAMEBOARD_USEFULL_CELL_AREA_PERCENT    0.80

#define HEADER_HEIGHT       40
#define HEADER_BORDER_X     10
#define HEADER_BORDER_Y     10
#define HEADER_FONT         SMALL_FONT
#define HEADER_BACK_COLOUR  c_black
#define HEADER_TEXT_COLOUR  c_green

#define DEAD_BACK_COLOUR    c_white
#define DEAD_TEXT_COLOUR    c_red
#define LEVELPASS_BACK_COLOUR c_yellow
#define LEVELPASS_TEXT_COLOUR c_blue


typedef struct
{
    int     board_pos_x;        // posição do tabuleiro em x
    int     board_pos_y;        // posição do tabuleiro em y
    int     cell_side;          // dimensão do lado da célula
    int     n_cell_x;           // número de células do tabuleiro em x
    int     n_cell_y;           // número de células do tabuleiro em y
    int     board_width;        // largura do tabuleiro em pixeis
    int     board_height;       // altura do tabuleiro em pixeis

    // header
    TextBox level_tb;           // nível
    TextBox snake_size_tb;      // dimensão actual da cobra
    Timer   timeout_tm;         // tempo para terminar nível
    Timer   time_tm;            // tempo total de jogo
} GameBoard;


void gameboard_init             (GameBoard *gb, int n_cells_x, int n_cells_y, int level, int size);

void gameboard_update           (GameBoard *gb, int n_cells_x, int n_cells_y);

bool gameboard_screen_to_xy     (const GameBoard *gb, int sx, int sy, int *x, int *y);

bool gameboard_xy_to_screen     (const GameBoard *gb, int x, int y, int *sx, int *sy);

void gameboard_paint_cell       (const GameBoard *gb, int x, int y, RGB colour);

void gameboard_clean_cell       (const GameBoard *gb, int x, int y);

void gameboard_draw_circle      (const GameBoard *gb, int x, int y, RGB colour);

void gameboard_draw_food        (const GameBoard *gb, Point pos, int energy);

void gameboard_draw_snake_head  (const GameBoard *gb, int x, int y, Direction dir);

void gameboard_draw_wall        (const GameBoard *gb, Point pos, int type, int size);

void gameboard_draw_grid        (const GameBoard *gb);

void gameboard_draw             (const GameBoard *gb);

void gameboard_second_passed    (GameBoard *gb);

#endif
