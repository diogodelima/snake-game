/**
 * @file text_box.h
 * @author Nuno Oliveira (nuno.oliveira@isel.pt)
 * @brief Componente TextBox
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <pg/pglib.h>


#define TEXT_BOX_BORDER_X    5
#define TEXT_BOX_BORDER_Y    5
#define TEXT_BOX_MAX_STR    64


typedef enum {  
    TEXT_SMALL_FONT  = SMALL_FONT, 
    TEXT_MEDIUM_FONT = MEDIUM_FONT, 
    TEXT_LARGE_FONT  = LARGE_FONT 
} TextFont;

typedef enum { TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT} TextAlign;


typedef struct 
{
    int         pos_x;                      // x position
    int         pos_y;                      // y position
    int         border_x;                   // dimension of internal margin in x
    int         border_y;                   // dimension of internal margin in x
    int         n_chars;                    // Max number of char supported by text box
    TextFont    font_type;                  // font type used by text box
    RGB         back_colour;                // back colour of text box 
    RGB         text_colour;                // text colour
    TextAlign   text_align;                 // type of text alignment
    int         width;                      // total od text box width
    int         height;                     // total of text_box height
    char        text[TEXT_BOX_MAX_STR+1];   // text to show
} TextBox;



void textbox_init        (TextBox *tb, int pos_x, int pos_y, int n_chars,        TextFont font_type, int back_colour, int text_colour);
void textbox_init_text   (TextBox *tb, int pos_x, int pos_y, const char text [], TextFont font_type, int back_colour, int text_colour);

void textbox_set_position(TextBox *tb, int pos_x, int pos_y);
void textbox_set_colours (TextBox *tb, RGB back_colour, RGB text_colour);
void textbox_set_align   (TextBox *tb, TextAlign text_align) ;
void textbox_set_border  (TextBox *tb, int border_x, int border_y);
void textbox_set_text    (TextBox *tb, const char text[]);

Size textbox_get_size    (const TextBox *tb);

void textbox_show        (const TextBox *tb);

void textbox_show_text   (const TextBox *tb, const char text[]);

#endif