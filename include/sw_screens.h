/**
 * SchmartWatsch Firmware - Screens
 * 
 * Author: Bastian Neumann 2018
 * 
 */

#include <stdbool.h>

#include <display/display.h>
#include "sw_rtc.h"

void sw_splashscreen(display_t *dsp, now_t *n);
void sw_watch1_init(display_t *dsp, now_t *now, uint16_t steps);
void sw_watch1();
void sw_watch1_ble_status_cb();