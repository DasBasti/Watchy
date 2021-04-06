/**
 * SchmartWatsch Firmware - Watch 1 Screen (7-Segment Circuit Design)
 * 
 * Author: Bastian Neumann 2018
 * 
 */
#include <stdio.h>

#include <Platinenmacher.h>
#include "boards.h"
#include "sw_screens.h"
#include "images/sm_seven_segment.h"
#include "icons/sm_bluetooth.h"
#include "icons/sm_bluetooth_adv.h"
#include "sw_rtc.h"

static display_t *dsp;
now_t *now;
static uint8_t cycles = 0;
static int8_t old_hours = -11, old_minutes = -11, old_day = -1;
uint32_t steps = 0;
static void draw_vertical_bar(uint8_t x, uint8_t y)
{
    // draw large box
    display_rect_fill(dsp, x, y + 1, 5, 18, BLACK);
    // draw two small lines at top and bottom
    display_line_draw(dsp, x + 1, y, x + 3, y, BLACK);
    display_line_draw(dsp, x + 1, y + 19, x + 3, y + 19, BLACK);
}
static void draw_horizontal_bar(uint8_t x, uint8_t y)
{
    // draw large box
    display_rect_fill(dsp, x + 1, y, 18, 5, BLACK);
    // draw two small lines at left and right
    display_line_draw(dsp, x, y + 1, x, y + 3, BLACK);
    display_line_draw(dsp, x + 19, y + 1, x + 19, y + 3, BLACK);
}

#define top_horizontal draw_horizontal_bar(x + 4, y + 0)
#define top_left_vertical draw_vertical_bar(x + 0, y + 4)
#define top_right_vertical draw_vertical_bar(x + 23, y + 4)
#define middle_horizontal draw_horizontal_bar(x + 4, y + 23)
#define bottom_left_vertical draw_vertical_bar(x + 0, y + 27)
#define bottom_right_vertical draw_vertical_bar(x + 23, y + 27)
#define bottom_horizontal draw_horizontal_bar(x + 4, y + 46)

static void draw_digit(uint8_t position, uint8_t number)
{
    if (number > 9)
    {
        return;
    }

    uint8_t y = 56;

    uint8_t x = 0;
    switch (position)
    {
    case 1:
        x = 8;
        break;
    case 2:
        x = 39;
        break;
    case 3:
        x = 83;
        break;
    case 4:
        x = 114;
        break;
    }
#if 0
    if (dsp->partial_window.ys > y) // if startpoint of y is above this drawen object we want to start here
    {
        dsp->partial_window.ys = y;
    }
    if (dsp->partial_window.ye < y + 51) // same for endpoint
    {
        dsp->partial_window.ye = y + 51;
    }

    if (dsp->partial_window.xs > x) // if x is right from this point set this point to the left most point
    {
        dsp->partial_window.xs = x;
    }
    if (dsp->partial_window.xe < x + 28) // same for endpoint
    {
        dsp->partial_window.xe = x + 28;
    }
#endif
    switch (number)
    {
    case 0:
        top_horizontal;
        top_left_vertical;
        top_right_vertical;
        bottom_left_vertical;
        bottom_right_vertical;
        bottom_horizontal;
        break;
    case 1:
        top_right_vertical;
        bottom_right_vertical;
        break;
    case 2:
        top_horizontal;
        top_right_vertical;
        middle_horizontal;
        bottom_left_vertical;
        bottom_horizontal;
        break;
    case 3:
        top_horizontal;
        top_right_vertical;
        middle_horizontal;
        bottom_right_vertical;
        bottom_horizontal;
        break;
    case 4:
        top_left_vertical;
        top_right_vertical;
        middle_horizontal;
        bottom_right_vertical;
        break;
    case 5:
        top_horizontal;
        top_left_vertical;
        middle_horizontal;
        bottom_right_vertical;
        bottom_horizontal;
        break;
    case 6:
        top_horizontal;
        top_left_vertical;
        middle_horizontal;
        bottom_left_vertical;
        bottom_right_vertical;
        bottom_horizontal;
        break;
    case 7:
        top_horizontal;
        top_right_vertical;
        bottom_right_vertical;
        break;
    case 8:
        top_horizontal;
        top_right_vertical;
        top_left_vertical;
        middle_horizontal;
        bottom_left_vertical;
        bottom_right_vertical;
        bottom_horizontal;
        break;
    case 9:
        top_horizontal;
        top_right_vertical;
        top_left_vertical;
        middle_horizontal;
        bottom_right_vertical;
        bottom_horizontal;
        break;
    }
}

static void nerd_steps(char *number, uint32_t steps)
{
    if (steps < 1000)
    {
        sprintf(number, "%03d", steps);
    }
    else if (steps < 10000)
    {
        // 1K0 9K9
        uint8_t kilo = steps / 1000;
        uint8_t cents = (steps % 1000) / 100;
        sprintf(number, "%01dK%01d", kilo, cents);
    }
    else if (steps < 100000)
    {
        // 10K 99K
        uint8_t kilo = steps / 1000;
        sprintf(number, "%02dK", kilo);
    }
    else if (steps < 10000000)
    {
        // 0M1 9M9
        uint8_t kilo = steps / 1000000;
        uint8_t cents = (steps % 1000000) / 100000;
        sprintf(number, "%01dM%01d", kilo, cents);
    }
    else
    {
        // oo
        sprintf(number, "inf");
    }
}

static void full_screen()
{
    font_t fnt8x8;
    font_load_from_array(&fnt8x8, font8x8, font8x8_name);
    //display_clear(dsp, WHITE);

    //DOT 1
    display_rect_fill(dsp, 71, 80, 6, 4, BLACK);
    display_rect_fill(dsp, 72, 79, 4, 6, BLACK);

    //DOT2
    display_rect_fill(dsp, 71, 98, 6, 4, BLACK);
    display_rect_fill(dsp, 72, 97, 4, 6, BLACK);

    uint8_t hours = now->hour;
    uint8_t minutes = now->minute; //rtc_minutes();
    if (hours % 10 != old_hours % 10)
    {
        draw_digit(2, hours % 10);
        //NRF_LOG_INFO("Watch1: hour %% 10: %d", hours % 10);
        if (hours / 10 != old_hours / 10)
        {
            draw_digit(1, hours / 10);
            //NRF_LOG_INFO("Watch1: hour / 10: %d", hours / 10);
        }
        old_hours = hours;
    }

    if (minutes % 10 != old_minutes % 10)
    {
        draw_digit(4, minutes % 10);
        //NRF_LOG_INFO("Watch1: minutes %% 10: %d", minutes % 10);
        if (minutes / 10 != old_minutes / 10)
        {
            draw_digit(3, minutes / 10);
            //NRF_LOG_INFO("Watch1: minutes / 10: %d", minutes / 10);
        }
        old_minutes = minutes;
    }
    //if (!dsp->partial_update)
    //{
    display_draw_image_colored(dsp, (uint8_t *)sm_seven_segment_bot, 0, 107, BLACK);
    //}

    // draw white text above black background of chips

    uint8_t day = now->day; //rtc_day();
    if (old_day != day)
    { //draw the top area again if we update day and month
        display_draw_image_colored(dsp, (uint8_t *)sm_seven_segment_top, 0, 0, BLACK);
        char h[3];
        sprintf(h, "%02d", day);
        display_text_draw(dsp, &fnt8x8, 38, 14, h, WHITE);
        sprintf(h, "%02d", now->month);
        display_text_draw(dsp, &fnt8x8, 38, 24, h, WHITE);
#if 0
        if (dsp->partial_window.ys > 10) // if startpoint of y is above this drawen object we want to start here
        {
            dsp->partial_window.ys = 10;
        }
        if (dsp->partial_window.ye < 35) // same for endpoint
        {
            dsp->partial_window.ye = 35;
        }

        if (dsp->partial_window.xs > 31) // if x is right from this point set this point to the left most point
        {
            dsp->partial_window.xs = 31;
        }
        if (dsp->partial_window.xe < 59) // same for endpoint
        {
            dsp->partial_window.xe = 59;
        }
#endif
        old_day = day;
    }
    char h[4];
    nerd_steps(h, steps);
    display_text_draw(dsp, &fnt8x8, 104, 12, h, WHITE);

    //dsp->update_pending = 1;
}

#if 0
void sw_watch1_ble_status_cb()
{
    if (ble_connection_status())
    {
        display_draw_image_colored(dsp, sm_bluetooth, 113, 14, COLOR_WHITE, COLOR_BLACK);
    }

    if (ble_advertisement_status())
    {
        //char h[] = "A";
        //display_text_draw(dsp, &fnt8x8, 108, 22, h, COLOR_WHITE);
        display_draw_image_colored(dsp, sm_bluetooth, 113, 14, COLOR_WHITE, COLOR_BLACK);
    }
}
#endif

/**
 * @brief initailizes the display and does a full update in blocking mode
 */
void sw_watch1_init(display_t *d, now_t *n, uint32_t s)
{
    dsp = d;
    now = n;
    steps = s;
    //dsp->partial_update = partial_update;
    //NRF_LOG_INFO("Update: %d", partial_update);
    //sw_watch1(now);
    //display_render(dsp);
    //dsp->partial_update = true;
}

void sw_watch1()
{
    // first we draw a complete image after that ist is partial for cycles times
    /*cycles++;
    if (cycles == 0) {
      dsp->partial_update = false;
    } else {
      dsp->partial_update = true;
    */
#if 0
    dsp->partial_window.xs = DISPLAY_SIZE_X - 1;
    dsp->partial_window.xe = 0;
    dsp->partial_window.ys = DISPLAY_SIZE_Y - 1;
    dsp->partial_window.ye = 0;
    //}
#endif
    //NRF_LOG_INFO("Screen cycle: %d", cycles);
    full_screen();
}
