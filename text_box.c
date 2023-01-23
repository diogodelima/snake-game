/**
 * @file text_box.c
 * @author Nuno Oliveira (nuno.oliveira@isel.pt)
 * @brief Componente TextBox
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "text_box.h"


void textbox_init (TextBox *tb, int pos_x, int pos_y, int n_chars, TextFont font_type, int back_colour, int text_colour) 
{
    tb->font_type   = font_type;
    tb->n_chars     = n_chars;

    textbox_set_position(tb, pos_x, pos_y);
    textbox_set_border  (tb, TEXT_BOX_BORDER_X, TEXT_BOX_BORDER_Y);
    textbox_set_colours (tb, back_colour, text_colour);
    textbox_set_align   (tb, TEXT_ALIGN_LEFT);
    textbox_set_text    (tb, "");
}

void textbox_init_text (TextBox *tb, int pos_x, int pos_y, const char text [], TextFont font_type, int back_colour, int text_colour)
{
    textbox_init(tb, pos_x, pos_y, strlen(text), font_type, back_colour, text_colour);
    textbox_set_text(tb, text);
}

void textbox_set_position(TextBox *tb, int pos_x, int pos_y)
{
    tb->pos_x = pos_x;
    tb->pos_y = pos_y;
}

void textbox_set_colours (TextBox *tb, RGB back_colour, RGB text_colour)
{
    tb->back_colour = back_colour;
    tb->text_colour = text_colour;
}

void textbox_set_align (TextBox *tb, TextAlign text_align) 
{
    tb->text_align = text_align;
}

void textbox_set_border (TextBox *tb, int border_x, int border_y)
{
    if (border_x < 0) border_x = TEXT_BOX_BORDER_X;
    if (border_y < 0) border_y = TEXT_BOX_BORDER_Y;
    tb->border_x    = border_x;
    tb->border_y    = border_y;

    Size font_size  = graph_font_size(tb->font_type);
    tb->width       = 2 * tb->border_x + font_size.width * tb->n_chars;
    tb->height      = 2 * tb->border_y + font_size.height;
}


static void adjust_text_to_size (char newStr[], int max_n_chars, const char text[])
{
    int txt_sz = strlen(text);

    if (txt_sz > max_n_chars) {
        int sz = max_n_chars -3;
        strncpy(newStr, text, sz);
        newStr[sz] = '\0';
        strcat(newStr, "...");
    }
    else strcpy(newStr, text);
}

void textbox_set_text (TextBox *tb, const char text[])
{
	tb->n_chars = 30;
    adjust_text_to_size(tb->text, tb->n_chars, text);
}


// void textbox_set_text (TextBox *tb, const char text[])
// {
//     int txt_sz = strlen(text);

//     if (txt_sz > tb->n_chars) {
//         int sz = tb->n_chars -3;
//         strncpy(tb->text, text, sz);
//         tb->text[sz] = '\0';
//         strcat(tb->text, "...");
//     }
//     else strcpy(tb->text, text);
// }


Size textbox_get_size (const TextBox *tb)
{
    Size sz = { .height = tb->height, .width = tb->width};
    return sz;
}


static void textbox_put_text (const TextBox *tb, const char text[])
{
    graph_rect(tb->pos_x, tb->pos_y, tb->width, tb->height, tb->back_colour, tb->font_type);

    // calcular a linha inferior do texto
    int txt_y       = tb->pos_y + tb->height - tb->border_y - 1;
    int txt_x       = tb->border_x + tb->pos_x;
    Size font_size  = graph_font_size(tb->font_type);
    int waste_x     = tb->width - 2 * tb->border_x - strlen(text) * font_size.width;
    switch (tb->text_align) {
        case TEXT_ALIGN_LEFT:   break;
        case TEXT_ALIGN_CENTER: txt_x += waste_x / 2; break;
        case TEXT_ALIGN_RIGHT:  txt_x += waste_x;     break;
    }

    graph_text(txt_x, txt_y, tb->text_colour, text, tb->font_type);
}

void textbox_show (const TextBox *tb)
{
    textbox_put_text (tb, tb->text);
}


void textbox_show_text (const TextBox *tb, const char text[])
{
    char new_text[TEXT_BOX_MAX_STR+1];

    adjust_text_to_size(new_text, tb->n_chars, text);
    textbox_put_text(tb, new_text);
}
