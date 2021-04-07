/**
 * SchmartWatsch Firmware - Screens
 * 
 * Author: Bastian Neumann 2018
 * 
 */
#ifndef __SW_SCREENS_H__
#define __SW_SCREENS_H__
#include <stdbool.h>

#include <display/display.h>
#include "sw_rtc.h"

typedef struct
{
    now_t *now;
    uint32_t steps;
    uint16_t battery;
    uint8_t battery_percent;
    uint8_t ble;
} state_t;

void sw_splashscreen(display_t *dsp, state_t *state);
void sw_watch1_init(display_t *dsp, state_t *state);
void sw_watch1();
void sw_watch1_ble_status_cb();

#endif /*__SW_SCREENS_H__ */
