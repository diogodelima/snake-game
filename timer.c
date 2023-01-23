/**
 * @file timer.c
 * @author Nuno Oliveira (nuno.oliveira@isel.pt)
 * @brief Implementação das funções do componente timer
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "timer.h"



void timer_init (Timer *timer, int minutes, int seconds, int pos_x, int pos_y, 
                 const char label[], TextFont font_type, int back_colour, int text_colour)
{
    timer_set(timer, minutes, seconds);

    timer_set_label(timer, label);
    int label_sz = strlen(timer->label) + strlen("00:00");

    textbox_init(&timer->textbox, pos_x, pos_y, label_sz, font_type, back_colour, text_colour);
}


void timer_set_position (Timer *timer, int pos_x, int pos_y)
{
    textbox_set_position(&timer->textbox, pos_x, pos_y);
}

void timer_set_colours (Timer *timer, RGB back_colour, RGB text_colour)
{
    textbox_set_colours(&timer->textbox, back_colour, text_colour);
}

void timer_set_align (Timer *timer, TextAlign text_align)
{
    textbox_set_align(&timer->textbox, text_align);
}

void timer_set_border (Timer *timer, int border_x, int border_y)
{
    textbox_set_border(&timer->textbox, border_x, border_y);
}

void timer_set_label (Timer *timer, const char label[]) 
{
    int label_sz = strlen(label);
    if (label_sz > TIMER_MAX_LABEL) label_sz = TIMER_MAX_LABEL;
    strncpy(timer->label, label, label_sz);
    timer->label[label_sz] = '\0';
}



Size timer_get_size (const Timer *timer) 
{
    return textbox_get_size(&timer->textbox);
}


void timer_set (Timer *timer, int minutes, int seconds)
{
    timer->minutes = minutes;
    timer->seconds = seconds;
}

void timer_reset (Timer *timer)
{
    timer_set(timer, 0, 0);
}

void timer_up_one_second (Timer *timer) 
{
    timer->seconds = (timer->seconds + 1) % 60;
    if (timer->seconds == 0) 
        timer->minutes = (timer->minutes + 1) % 60;
}


void timer_down_one_second (Timer *timer) 
{
    if (timer->seconds > 0) {
        timer->seconds--;
    } else if (timer->minutes > 0) {
        timer->minutes--;
        timer->seconds = 59;
    }
}

bool timer_is_timeout (const Timer *timer)
{
    return timer->minutes == 0 && timer->seconds == 0;
}

int timer_get_minutes (const Timer *timer)
{
    return timer->minutes;
}

int timer_get_seconds (const Timer *timer)
{
    return timer->seconds;
}

void timer_print (const Timer *timer)
{
    printf("Timer: %02d:%02d\n", timer->minutes, timer->seconds);
}

void timer_show (const Timer *timer) 
{
    char aux[TIMER_MAX_LABEL + TIMER_MMSS_CHARS + 1];

    if (strlen(timer->label) == 0) {
        sprintf(aux, "%02d:%02d", timer->minutes, timer->seconds);
    } else {
        sprintf(aux, "%s%02d:%02d", timer->label, timer->minutes, timer->seconds);
    }
    textbox_show_text(&timer->textbox, aux);
} 