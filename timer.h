/**
 * @file timer.h
 * @author Nuno Oliveira (nuno.oliveira@isel.pt)
 * @brief Componte Timer
 * @version 0.1
 * @date 2022-12-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdbool.h>
#include "text_box.h"


#define TIMER_MAX_LABEL     32
#define TIMER_MMSS_CHARS     8



typedef struct
{
    int     minutes;
    int     seconds;
    char    label[TIMER_MAX_LABEL+1];
    TextBox textbox;
} Timer;




void timer_init             (Timer *timer, int minutes, int seconds, int pos_x, int pos_y, 
                             const char label[], TextFont font_type, int back_colour, int text_colour);

void timer_set_position     (Timer *timer, int pos_x, int pos_y);
void timer_set_colours      (Timer *timer, RGB back_colour, RGB text_colour);
void timer_set_align        (Timer *timer, TextAlign text_align);
void timer_set_border       (Timer *timer, int border_x, int border_y);
void timer_set_label        (Timer *timer, const char text[]);

Size timer_get_size         (const Timer *timer);

void timer_set              (Timer *timer, int minutes, int seconds);

void timer_reset            (Timer *timer);

void timer_up_one_second    (Timer *timer);

void timer_down_one_second  (Timer *timer);

bool timer_is_timeout       (const Timer *timer);

int timer_get_minutes       (const Timer *timer);

int timer_get_seconds       (const Timer *timer);


void timer_print            (const Timer *timer);

void timer_show             (const Timer *timer);

