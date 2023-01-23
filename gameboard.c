#include "game.h"
#include "gameboard.h"

#include <stdbool.h>




/**
 * @brief Create a header snake game object
 *      Ilustra como pode ser criado o header pedido no terceiro trabalho prático
 * @param font          tipo de fonte a ser usada
 * @param header_height Altura do header
 * @param back_colour   cor do fundo
 * @param text_colour   cor do texto
 */
void  gameboard_header_init (GameBoard *gb, TextFont font, int header_height, RGB back_colour, RGB text_colour, int level, int size)
{
    int win_width  = graph_get_width();

    graph_rect(0, 0, win_width, header_height, back_colour, true);
    
	char str_level[] = "Level: ";
	char str_level1[20];
	snprintf (str_level1, 20, "%s%d", str_level, level );
	
	char str_size[] = "Size: ";
	char str_size1[20];
	snprintf (str_size1, 20, "%s%d", str_size, size );
	
	gb -> snake_size_tb.n_chars = 15;

    textbox_init_text(&gb->level_tb,      0, 0, str_level1, font, back_colour, text_colour);
    textbox_init_text(&gb->snake_size_tb, 0, 0, str_size1,  font, back_colour, text_colour);

    timer_init(&gb->timeout_tm, 2, 30, 0, 0, "Timeout: ", font, back_colour, text_colour);
    timer_init(&gb->time_tm,    0,  0, 0, 0, "Time: ",    font, back_colour, text_colour);

    Size level_sz       = textbox_get_size(&gb->level_tb);
    Size snake_size_sz  = textbox_get_size(&gb->snake_size_tb);
    Size timeout_sz     = timer_get_size(&gb->timeout_tm);
    Size time_sz        = timer_get_size(&gb->time_tm);

    int waste_y = header_height - level_sz.height - 2 * HEADER_BORDER_Y;
    int pos_y = waste_y / 2 + HEADER_BORDER_Y;

    int waste_x = win_width - level_sz.width - snake_size_sz.width - timeout_sz.width - time_sz.width - 20;
    int gap_x     = waste_x / 3;

    int pos_x = HEADER_BORDER_X;
    textbox_set_position(&gb->level_tb, pos_x, pos_y);

    pos_x += level_sz.width + gap_x;
    textbox_set_position(&gb->snake_size_tb, pos_x, pos_y);

    pos_x += snake_size_sz.width + gap_x;
    timer_set_position(&gb->timeout_tm, pos_x, pos_y);

    pos_x += timeout_sz.width + gap_x;
    timer_set_position(&gb->time_tm, pos_x, pos_y);
    
}

void gameboard_header_draw (const GameBoard *gb)
{
    int win_width  = graph_get_width();

    // draw header area
    graph_rect(0, 0, win_width, HEADER_HEIGHT, HEADER_BACK_COLOUR, true);
    
    textbox_show(&gb->level_tb);
    textbox_show(&gb->snake_size_tb);
    timer_show  (&gb->timeout_tm);
    timer_show  (&gb->time_tm);
}



void gameboard_update (GameBoard *gb, int n_cells_x, int n_cells_y)
{
    int win_width  = graph_get_width() - 1;
    int win_height = graph_get_height() -1 - HEADER_HEIGHT;

    int dim_cell = win_width / n_cells_x;

    int board_width = dim_cell * n_cells_x;
    int board_height = dim_cell * n_cells_y;

    int waste_x = win_width - board_width;
    int waste_y = win_height - board_height;

    gb->board_pos_x = waste_x / 2;
    gb->board_pos_y = waste_y / 2 + HEADER_HEIGHT;
    gb->n_cell_x = n_cells_x;
    gb->n_cell_y = n_cells_y;
    gb->cell_side = dim_cell;
    gb->board_width = board_width;
    gb->board_height  = board_height;
    gameboard_header_draw(gb);
}

void gameboard_init (GameBoard *gb, int n_cells_x, int n_cells_y, int level, int size)
{
   gameboard_header_init(gb, TEXT_MEDIUM_FONT, HEADER_HEIGHT, HEADER_BACK_COLOUR, HEADER_TEXT_COLOUR, level, size);
   gameboard_update(gb, n_cells_x, n_cells_y);
}

bool gameboard_screen_to_xy(const GameBoard *gb, 
                            int sx, int sy, int *x, int *y)
{
    sx = sx - gb->board_pos_x;
    sy = sy - gb->board_pos_y;

    if (sx < 0 || sx >= gb->board_width || sy < 0 || sy >= gb->board_height)
        return false;
    *x = sx / gb->cell_side;
    *y = sy / gb->cell_side;
    return true;
}

bool gameboard_xy_to_screen (const GameBoard *gb, 
                             int x, int y, int *sx, int *sy)
{
    if (x < 0 || x >= gb->n_cell_x|| y < 0 || y >= gb->n_cell_y)
        return false; 
    *sx = x * gb->cell_side + gb->board_pos_x;
    *sy = y * gb->cell_side + gb->board_pos_y;
    return true;
}

// obter as coordenadas para não incluir as linhas de divisão da célula
void gameboard_xy_to_screen_inner_cell (const GameBoard *gb, int x, int y, 
                                        int *sx, int *sy, int *side_dim)
{
    int aux_sx;
    int aux_sy;
    gameboard_xy_to_screen(gb, x, y, &aux_sx, &aux_sy);

    *sx       = aux_sx + 1;
    *sy       = aux_sy + 1;
    *side_dim = gb->cell_side - 1;
}

void gameboard_paint_cell (const GameBoard *gb, int x, int y, RGB colour)
{
    int sx;
    int sy;
    int side;
    gameboard_xy_to_screen_inner_cell(gb, x, y, &sx, &sy, &side);

    graph_rect(sx, sy, side, side, colour, true);
}



void gameboard_clean_cell (const GameBoard *gb, int x, int y)
{
    gameboard_paint_cell(gb, x, y, GAMEBOARD_COLOUR_ARENA);
}


void gameboard_draw_circle (const GameBoard *gb, int x, int y, RGB colour)
{
    int sx;
    int sy;
    int side;
    gameboard_xy_to_screen_inner_cell(gb, x, y, &sx, &sy, &side);

    // calculates the center and rounding
    int cx     = sx + (side * 0.5) + 0.5;
    int cy     = sy + (side * 0.5) + 0.5;
    int radius = (side * 0.5) * GAMEBOARD_USEFULL_CELL_AREA_PERCENT;

    graph_circle(cx, cy, radius, colour, true);
}

//-----------------------------------------------------------------------------
//    Food
//-----------------------------------------------------------------------------
void gameboard_draw_food (const GameBoard *gb, Point pos, int energy)
{
    RGB cfood  = energy < 0 ? GAMEBOARD_COLOUR_FOOD_BAD: GAMEBOARD_COLOUR_FOOD_GOOD;
    gameboard_draw_circle(gb, pos.x, pos.y, cfood);
}




//-----------------------------------------------------------------------------
//    Snake
//-----------------------------------------------------------------------------
void gameboard_draw_snake_head (const GameBoard *gb, int x, int y, Direction dir)
{
    int sx;
    int sy;
    int side;
    gameboard_xy_to_screen_inner_cell(gb, x, y, &sx, &sy, &side);

    // calculates the center and rounding
    int cx           = sx + (side * 0.5) + 0.5;
    int cy           = sy + (side * 0.5) + 0.5;
    int radius       = (side * 0.5) * GAMEBOARD_USEFULL_CELL_AREA_PERCENT;
    int start_angle  = 30;
    int end_angle    = 330;
    switch (dir) {
        case LEFT:  start_angle = 210; end_angle = 150; break;
        case RIGHT: start_angle =  30; end_angle = 330; break;
        case UP:    start_angle = 300; end_angle = 240; break;
        case DOWN:  start_angle = 120; end_angle =  60; break;
    }
    // gameboard_paint_cell_xy(gb, x, y, GAMEBOARD_COLOUR_SNAKE_BODY);
    graph_pie(cx, cy, radius, start_angle, end_angle, GAMEBOARD_COLOUR_SNAKE_HEAD, true);
}


//-----------------------------------------------------------------------------
//    Wall
//-----------------------------------------------------------------------------
void gameboard_draw_horizontal_wall (const GameBoard *gb, int x, int y, int size)
{
    int sx;
    int sy;
    gameboard_xy_to_screen(gb, x, y, &sx, &sy); 

    for (int i = 0; i < size; i++) {
        gameboard_paint_cell(gb, x + i, y, GAMEBOARD_COLOUR_WALL);
    }
}

void gameboard_draw_vertical_wall (const GameBoard *gb, int x, int y, int size)
{
    int sx;
    int sy;
    gameboard_xy_to_screen(gb, x, y, &sx, &sy); 

    for (int i = 0; i < size; i++) {
        gameboard_paint_cell(gb, x, y + i, GAMEBOARD_COLOUR_WALL);
    }
}

void gameboard_draw_wall_xy (const GameBoard *gb, int x, int y, int type, int size)
{
    if (type == HORIZONTAL) {
        gameboard_draw_horizontal_wall(gb, x, y, size);
    } else {
        gameboard_draw_vertical_wall(gb, x, y, size);
    }
}

void gameboard_draw_wall (const GameBoard *gb, Point pos, int type, int size)
{
    gameboard_draw_wall_xy(gb, pos.x, pos.y, type, size);
}


//-----------------------------------------------------------------------------
//    Board
//-----------------------------------------------------------------------------
void gameboard_draw_grid (const GameBoard *gb)
{
    // desenha linhas horizontais
    for (int i = 0; i < gb->n_cell_x + 1; i++) {
        int pos_x      = gb->board_pos_x + (i * gb->cell_side);
        int pos_y_end = gb->board_pos_y + gb->board_height-1;
        graph_line(pos_x, gb->board_pos_y, pos_x, pos_y_end, GAMEBOARD_COLOUR_ARENA_GRID);
    }
    // desenha linhas verticais
    for (int i = 0; i < gb->n_cell_y + 1; i++) {
        int pos_y      = gb->board_pos_y + (i * gb->cell_side);
        int pos_x_end = gb->board_pos_x + gb->board_width-1;
        graph_line(gb->board_pos_x, pos_y, pos_x_end, pos_y, GAMEBOARD_COLOUR_ARENA_GRID);
    }
}

void gameboard_draw (const GameBoard * gb)
{
    // clean background window
    graph_rect(0, 0, graph_get_width(), graph_get_height(), 
               GAMEBOARD_COLOUR_WINDOW_BACKGROUND, true);

    // draw header
    gameboard_header_draw(gb);

    // draw game board area
    graph_rect(gb->board_pos_x, gb->board_pos_y, 
               gb->board_width, gb->board_height, GAMEBOARD_COLOUR_ARENA, true);

    // draw board grid lines
    gameboard_draw_grid(gb);
}

void gameboard_second_passed (GameBoard *gb)
{
    timer_up_one_second(&gb->time_tm);
    timer_down_one_second (&gb->timeout_tm);
    timer_show(&gb->timeout_tm); 
    timer_show(&gb->time_tm);
}
