/**
 * SchmartWatsch Firmware - Generic RTC Driver
 * 
 * Author: Bastian Neumann 2018
 * 
 */

#ifndef SW_RTC_H
#define SW_RTC_H

#include <stdint.h>

typedef enum
{
    ALARM_ONCE_PER_YEAR,
    ALRAM_ONCE_PER_MONTH,
    ALARM_ONCE_PER_DAY,
    ALARM_ONCE_PER_HOUR,
    ALARM_ONCE_PER_MINUTE,
    ALARM_ONCE_PER_SECOND,
} alarm_t;

int32_t rtc_localtime();

#ifndef WINVER
#include "m41t62lc6f.h"

#define rtc_init m41t62_init
#define rtc_milliseconds m41t62_milliseconds
#define rtc_seconds m41t62_seconds
#define rtc_minutes m41t62_minutes
#define rtc_hours m41t62_hours
#define rtc_day_of_the_week m41t62_day_of_the_week
#define rtc_day m41t62_day
#define rtc_month m41t62_month
#define rtc_year m41t62_year

#define rtc_set_date m41t62_set_date
#define rtc_set_alarm_periode m41t62_set_alarm_periode

#else
// Mockup variants of RTC functions
#define rtc_init mock_init
#define rtc_milliseconds mock_time60
#define rtc_seconds mock_time60
#define rtc_minutes mock_time60
#define rtc_hours mock_time24
#define rtc_day_of_the_week mock_int7
#define rtc_day mock_int30
#define rtc_month mock_int12
#define rtc_year mock_int2k

#endif

#endif //SW_RTC_H