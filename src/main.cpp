#include <Arduino.h>
#include <RV3028C7.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LSM6DSLSensor.h>
#include "time.h"
#include "BLE.h"

extern "C"
{
#include <Platinenmacher.h>
#include "boards.h"
#include "sw_screens.h"
#include "sw_rtc.h"
}

#ifdef DEBUG
#define PRINT Serial.print
#define PRINTLN Serial.println
#else
#define PRINT(a)
#define PRINTLN(a)
#endif
const char *ssid = "Zuck";
const char *password = "12348765";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

void handleButtonPress();

display_t *dsp;
static gdew0154z17_io_config_t io = {};
RV3028C7 rtc;
LSM6DSLSensor AccGyr(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW);
now_t *now;
bool update_screen = false;
//HTTPClient http;
uint32_t step_counts;
BLE ble;
bool stay_active = false;
bool update_needed = false;

void handle_wakeup(esp_sleep_wakeup_cause_t);

void go_to_sleep()
{
  if (!stay_active)
  {
    uint8_t sleep_time = 60 - rtc.getDateTimeComponent(DATETIME_SECOND);
    if (sleep_time == 0) // if zero seconds to next update. Do it now.
    {
      handle_wakeup(ESP_SLEEP_WAKEUP_EXT0);
    }

    rtc.setPeriodicCountdownTimer(/*timerValue=*/sleep_time, /*frequency=*/TIMER_1HZ,
                                  /*repeat=*/false);
    // Refer to page 63 of Application Manual to configure timer value and timer
    // clock frequency
    rtc.enableInterrupt(INTERRUPT_PERIODIC_COUNTDOWN_TIMER);
    rtc.startPeriodicCountdownTimer();

    PRINTLN("Go to sleep");
    Serial.flush();
    esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_PIN, 0);                 //enable deep sleep wake on RTC interrupt
    esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
    esp_deep_sleep_start();
  }
}

void handle_wakeup(esp_sleep_wakeup_cause_t wakeup_reason)
{
  now->day = rtc.getDateTimeComponent(DATETIME_DAY_OF_MONTH);
  now->month = rtc.getDateTimeComponent(DATETIME_MONTH);
  now->minute = rtc.getDateTimeComponent(DATETIME_MINUTE);
  now->hour = rtc.getDateTimeComponent(DATETIME_HOUR);
  PRINTLN(rtc.getCurrentDateTime());

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT1: //button Press
    handleButtonPress();
    break;
  case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
    update_needed = false;
    display_fill(dsp, WHITE);
    sw_watch1_init(dsp, now, step_counts);
    sw_watch1();
    display_commit_fb(dsp);
    PRINTLN("display");
    break;
  default: // Power On Reset
    sw_splashscreen(dsp, now);
    AccGyr.begin();
    AccGyr.Enable_Pedometer();
    display_commit_fb(dsp);
    break;
  }
}

void updateRTC(char *time)
{
  //2021-03-31T21:20:30
  rtc.setDateTimeFromISO8601(time);
  rtc.synchronize(); // Writes the new date time to RTC
  update_needed = true;
}

void handleButtonPress()
{
  PRINTLN("BLE on");
  ble.setCtsUpdateCb(updateRTC);
  ble.begin("Schmartwatch");
  stay_active = true;
}

void setup()
{
  Serial.begin(115200);
  PRINTLN("");
  // set up GPIO for Display
  io.sclk = DISPLAY_SCLK; // spi clock
  io.mosi = DISPLAY_MOSI; // spi data in
  io.busy = gpio_create(PAD_INPUT, DISPLAY_BUSY);
  pinMode(io.busy->pin, INPUT_PULLUP);
  gpio_update_mode(io.busy);
  io.res = gpio_create(PAD_OUTPUT, DISPLAY_RES);
  io.dc = gpio_create(PAD_OUTPUT, DISPLAY_DC);
  io.cs = gpio_create(PAD_OUTPUT, DISPLAY_CS);

  now_t n = {};
  now = &n;
  Wire.begin();
  while (rtc.begin() == false)
  {
    PRINTLN("Failed to detect RV-3028-C7!");
    delay(5000);
  }
  rtc.disableClockOutput();

  uint16_t steps;
  AccGyr.Get_Step_Counter(&steps);
  step_counts = steps;

  // set up SPI for display
  dsp = GDEW0154Z17_Init(DISPLAY_ROTATE_0, &io);
  if (!dsp)
    PRINTLN("E-Ink Display init failed");

  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
  //Serial.println(wakeup_reason);
  handle_wakeup(wakeup_reason);
}

void loop()
{
  go_to_sleep();
  if (!digitalRead(RTC_PIN) | update_needed)
    handle_wakeup(ESP_SLEEP_WAKEUP_EXT0);
  if (digitalRead(UP_BTN_PIN))
    handle_wakeup(ESP_SLEEP_WAKEUP_EXT1);
  if (digitalRead(DOWN_BTN_PIN))
    handle_wakeup(ESP_SLEEP_WAKEUP_EXT1);
  if (digitalRead(MENU_BTN_PIN))
    handle_wakeup(ESP_SLEEP_WAKEUP_EXT1);
  if (digitalRead(BACK_BTN_PIN))
    handle_wakeup(ESP_SLEEP_WAKEUP_EXT1);

  if (ble.updateStatus() == 4)
  {
    PRINTLN("Disable BLE");
    stay_active = false;
  }
}
