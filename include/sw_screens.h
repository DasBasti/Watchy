/**
 * SchmartWatsch Firmware - Screens
 * 
 * Author: Bastian Neumann 2018
 * 
 */

#include <stdbool.h>

#include <display/display.h>

void sw_splashscreen(display_t *dsp);
void sw_watch1_init(display_t *dsp, bool partial_update);
void sw_watch1();
void sw_watch1_ble_status_cb();