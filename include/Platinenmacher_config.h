/*
 * Platinenmacher_config.h
 *
 *  Created on: 03.01.2021
 *      Author: bastian
 */

#ifndef __PLATINENMACHER_CONFIG_H__
#define __PLATINENMACHER_CONFIG_H__

#include <stdio.h>
#define __weak __attribute__((weak))
#define EINK_GDEW0154Z17

#include "hw/gpio.h"

/* 3 color ePaper display */
#include "display/display.h"
#include "display/eink/gdew0154z17.h"
#include "display/colors.h"

/* GUI drawing */
#include "display/font.h"
#include "display/font/font8x8.h"
#include "display/font/font8x16.h"
font_t f8x8, f8x16;
#include "display/GUI/image.h"
#include "display/GUI/label.h"

#endif /* __PLATINENMACHER_CONFIG_H__ */
