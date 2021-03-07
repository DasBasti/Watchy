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

#define BUTTON_1 GPIO_NUM_26
#define BUTTON_2 GPIO_NUM_25
#define BUTTON_3 GPIO_NUM_32
#define BUTTON_4 GPIO_NUM_4

#define SDA GPIO_NUM_21
#define SCL GPIO_NUM_22
#define ADC_PIN GPIO_NUM_33
#define RTC_PIN GPIO_NUM_27
#define VIB_MOTOR_PIN GPIO_NUM_13
#define MENU_BTN_PIN GPIO_NUM_26
#define BACK_BTN_PIN GPIO_NUM_25
#define UP_BTN_PIN GPIO_NUM_32
#define DOWN_BTN_PIN GPIO_NUM_4
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
#define DISPLAY_BUSY GPIO_NUM_19
#define DISPLAY_RES GPIO_NUM_9
#define DISPLAY_DC GPIO_NUM_10
#define DISPLAY_CS GPIO_NUM_5
#define DISPLAY_SCLK GPIO_NUM_18
#define DISPLAY_MOSI GPIO_NUM_23

#define DISPLAY_SIZE_X 152
#define DISPLAY_SIZE_Y 152

    unsigned char DISPLAY_HRES, DISPLAY_VRES_byte1, DISPLAY_VRES_byte2;

// RTC configuration
#define RTC_SDA GPIO_NUM_21
#define RTC_SCL GPIO_NUM_22
#define RTC_INT GPIO_NUM_27

// IMU configuration
#define IMU_SDA GPIO_NUM_8
#define IMU_SCL GPIO_NUM_3
#define IMU_CS_XL GPIO_NUM_5
#define IMU_CS_MAG GPIO_NUM_6
#define IMU_INT_2_XL GPIO_NUM_31
#define IMU_INT_1_XL GPIO_NUM_2
#define IMU_INT_MAG GPIO_NUM_24
#define IMU_I2C_INSTANCE GPIO_NUM_1

// open pins
#define OPEN_1 GPIO_NUM_2
#define OPEN_2 GPIO_NUM_34
#define OPEN_3 GPIO_NUM_35

#ifdef __cplusplus
}
#endif

#endif
