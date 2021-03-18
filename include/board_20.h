#ifndef BOARD_H
#define BOARD_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef HW_REVISION_STR
#define HW_REVISION_STR "E2-HS"
#endif

#define BUTTONS_NUMBER 4

#define BUTTON_1 26
#define BUTTON_2 25
#define BUTTON_3 32
#define BUTTON_4 4

#define ADC_PIN 33
#define RTC_PIN 27
#define VIB_MOTOR_PIN 13
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define UP_BTN_PIN 32
#define DOWN_BTN_PIN 4
#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define UP_BTN_MASK GPIO_SEL_32
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK MENU_BTN_MASK | BACK_BTN_MASK | UP_BTN_MASK | DOWN_BTN_MASK

#define BUTTON_PULL GPIO_PULLUP_ENABLE

#define BUTTONS_ACTIVE_STATE 1

#define BUTTONS_LIST                           \
    {                                          \
        BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4 \
    }

// Display pins
#define DISPLAY_BUSY 19
#define DISPLAY_RES 9
#define DISPLAY_DC 10
#define DISPLAY_CS 5
#define DISPLAY_SCLK 18
#define DISPLAY_MOSI 23

#define DISPLAY_SIZE_X 152
#define DISPLAY_SIZE_Y 152

// RTC configuration
#define RTC_SDA 21
#define RTC_SCL 22
#define RTC_INT 27

// IMU configuration
#define IMU_SDA 8
#define IMU_SCL 3
#define IMU_CS_XL 5
#define IMU_CS_MAG 6
#define IMU_INT_2_XL 31
#define IMU_INT_1_XL 2
#define IMU_INT_MAG 24
#define IMU_I2C_INSTANCE 1

// open pins
#define OPEN_1 2
#define OPEN_2 34
#define OPEN_3 35

#ifdef __cplusplus
}
#endif

#endif
