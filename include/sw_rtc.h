/**
 * SchmartWatsch Firmware - Generic RTC Driver
 * 
 * Author: Bastian Neumann 2018
 * 
 */

#ifndef SW_RTC_H
#define SW_RTC_H

#include <stdint.h>
typedef struct
{
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
} now_t;

#endif /* SW_RTC_H */