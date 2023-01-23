/**
 * @file test_header.c
 * @author Nuno Oliveira (nuno.oliveira@isel.pt)
 * @brief 
 *      Código exemplificativo da utilização dos componentes:
 *          - TextBox
 *          - Timer
 *      Exemplo da definição do header a ser utilizado no jogo da cobra do 
 *      3º trabalho prático
 *      compile: gcc -Wall -o test_header test_header.c text_box.c timer.c  -lpg
 *
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "text_box.h"
#include "timer.h"


// ----------------------------------------------------
// variáveis Globais
//   usadas pelo timerHandle
//   no contexto do terceiro trabalho associar estas variáveis que 
//   suportam o header ao GameBoard
TextBox level_tb;
TextBox snake_size_tb;
Timer   timeout_tm;
Timer   time_tm;



void timerHandler ()
{
    // printf("TIMER HANDLE\n");
    timer_up_one_second(&time_tm);
    timer_down_one_second(&timeout_tm);

    if (timer_is_timeout(&timeout_tm)) {
        timer_set_colours(&timeout_tm, c_black, c_red);
    }

    timer_show(&time_tm);
    timer_show(&timeout_tm);
}


#define HEADER_BORDER_X     10
#define HEADER_BORDER_Y     10

/**
 * @brief Create a header snake game object
 *      Ilustra como pode ser criado o header pedido no terceiro trabalho prático
 * @param font          tipo de fonte a ser usada
 * @param header_height Altura do header
 * @param back_colour   cor do fundo
 * @param text_colour   cor do texto
 */
void  create_header_snake_game (TextFont font, int header_height, RGB back_colour, RGB text_colour)
{
    int win_width  = graph_get_width();

    graph_rect(0, 0, win_width, header_height, back_colour, true);

    textbox_init_text(&level_tb,      0, 0, "Level: 000", font, back_colour, text_colour);
    textbox_init_text(&snake_size_tb, 0, 0, "Size: 000",  font, back_colour, text_colour);

    timer_init(&timeout_tm, 2, 30, 0, 0, "Timeout: ", font, back_colour, text_colour);
    timer_init(&time_tm,    0,  0, 0, 0, "Time: ",    font, back_colour, text_colour);

    Size level_sz       = textbox_get_size(&level_tb);
    Size snake_size_sz  = textbox_get_size(&snake_size_tb);
    Size timeout_sz     = timer_get_size(&timeout_tm);
    Size time_sz        = timer_get_size(&time_tm);

    int waste_y = header_height - level_sz.height - 2 * HEADER_BORDER_Y; //*******
    int pos_y = waste_y / 2 + HEADER_BORDER_Y; //********

    int waste_x = win_width - level_sz.width - snake_size_sz.width - timeout_sz.width - time_sz.width - 20;
    int gap_x     = waste_x / 3;

    int pos_x = HEADER_BORDER_X;///********
    textbox_set_position(&level_tb, pos_x, pos_y);

    pos_x += level_sz.width + gap_x;
    textbox_set_position(&snake_size_tb, pos_x, pos_y);

    pos_x += snake_size_sz.width + gap_x;
    timer_set_position(&timeout_tm, pos_x, pos_y);

    pos_x += timeout_sz.width + gap_x;
    timer_set_position(&time_tm, pos_x, pos_y);

    textbox_show(&level_tb);
    textbox_show(&snake_size_tb);
    timer_show(&timeout_tm);
    timer_show(&time_tm);

}



void test_text_box (int pos_x, int pos_y, int msg_sz, const char msg[], int font_type, int back_colour, int text_colour)
{
    TextBox tb1;
    TextBox tb2;
    TextBox tb3;

    textbox_init(&tb1,   pos_x, pos_y, msg_sz, font_type,  back_colour, text_colour); 
    pos_x += textbox_get_size(&tb1).width + 5;
    textbox_init(&tb2,   pos_x, pos_y, msg_sz, font_type,  back_colour, text_colour); 
    pos_x += textbox_get_size(&tb2).width + 5;
    textbox_init(&tb3,   pos_x, pos_y, msg_sz, font_type,  back_colour, text_colour); 

    // textbox_set_border(&tb1, 20, 20);
    // textbox_set_border(&tb2, 20, 20);
    // textbox_set_border(&tb3, 20, 20);

    textbox_set_align(&tb1, TEXT_ALIGN_LEFT);
    textbox_set_align(&tb2, TEXT_ALIGN_CENTER);
    textbox_set_align(&tb3, TEXT_ALIGN_RIGHT);

    textbox_show_text(&tb1,  msg);  
    textbox_show_text(&tb2,  msg);  
    textbox_show_text(&tb3,  msg);
}



#define WINDOW_WIDTH    1000
#define WINDOW_HEIGHT    400

#define TIMER_PERIOD    1000

int main ()
{
    graph_init2("Snake Game header test...", WINDOW_WIDTH, WINDOW_HEIGHT);

    printf("small font height %d,  width = %d\n", graph_font_size(SMALL_FONT).height, graph_font_size(SMALL_FONT).width);
    printf("medium font height %d, width = %d\n", graph_font_size(MEDIUM_FONT).height, graph_font_size(MEDIUM_FONT).width);
    printf("large font height %d,  width = %d\n", graph_font_size(LARGE_FONT).height, graph_font_size(LARGE_FONT).width);


    // char *msg = "ola pfzyYPqtJjabcdefghirjklmno";
    // int msg_sz = strlen(msg);
    int msg_sz = 16;
    char *msg = "TextBoard";

    test_text_box(10, 100, msg_sz, msg, TEXT_SMALL_FONT,  c_black, c_green);
    test_text_box(10, 140, msg_sz, msg, TEXT_MEDIUM_FONT, c_black, c_green);
    test_text_box(10, 180, msg_sz, msg, TEXT_LARGE_FONT,  c_black, c_green);

    create_header_snake_game(TEXT_MEDIUM_FONT, 40, c_black, c_green);


    // graph_regist_mouse_handler(mouseHandler);
    // graph_regist_key_handler(keyHandler);
    graph_regist_timer_handler(timerHandler, TIMER_PERIOD);    

      
    graph_start();
}