/**
 * SchmartWatch Firmware - Splashscreen
 * 
 * Author: Bastian Neumann 2018
 * 
 */
#include <stdio.h>

#include <Platinenmacher.h>

#include "version.h"
#include "boards.h"

#include "sw_screens.h"
#include "icons/sm_mono.h"
#include "font/font6x8.h"
#include "sw_rtc.h"

void sw_splashscreen(display_t *dsp, state_t *state)
{
    font_t fnt6x8;
    font_load_from_array(&fnt6x8, font6x8, font6x8_name);

    // draw black and red rectangle around screen
    display_rect_draw(dsp, 1, 1, 149, 149, BLACK);
    display_rect_draw(dsp, 2, 2, 147, 147, RED);
    display_rect_draw(dsp, 3, 3, 145, 145, BLACK);

    display_rect_fill(dsp, 10, 10, 132, 132, RED);

    char h[50];
    //sprintf(h, "%02d:%02d:%02d", (rand() % 24), (rand() % 24), (rand() % 24));
    sprintf(h, "%s", HW_REVISION_STR);

    //    display_text_draw(&dsp, &x8, (rand() % 154), (rand() % 154), h, COLOR_BLACK);
    display_text_draw(dsp, &fnt6x8, 57, 98, h, BLACK);

    //    sprintf(h, "%s", build_date);
    //    display_text_draw(dsp, &fnt6x8, 15, 125, h, COLOR_BLACK);

    sprintf(h, "%.5s", build_git_sha);
    display_text_draw(dsp, &fnt6x8, 13, 133, h, BLACK);

    sprintf(h, "%02d", state->now->minute);
    display_text_draw(dsp, &fnt6x8, 126, 133, h, WHITE);

    sprintf(h, "%02d:", state->now->hour);
    display_text_draw(dsp, &fnt6x8, 102, 133, h, WHITE);
    /*    sprintf(h, "%02d:%02d:%02d", (12), (34), (56));

    display_text_draw(dsp, xfnt[1], 10, 20, h, COLOR_BLACK);
*/
    // draw filled rect
    //    display_rect_fill(&dsp, (rand() % 154), (rand() % 154), (rand() % 154), (rand() % 154), COLOR_RED);

    // draw image
    //    display_image_draw_transparent(&dsp, sm_mono, (rand() % 154), (rand() % 154), COLOR_BLACK);
    display_draw_image_colored(dsp, sm_mono, 40, 9, BLACK);
}