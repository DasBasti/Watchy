#include <Arduino.h>
#include <RV3028C7.h>

extern "C"
{
#include <Platinenmacher.h>
#include "boards.h"
#include "sw_screens.h"
#include "sw_rtc.h"
}

display_t *dsp;
static gdew0154z17_io_config_t io = {};
RV3028C7 rtc;
now_t *now;
bool update_screen = false;

void rtc_update()
{
}

void setup()
{
  //Serial.begin(115200);
  //Serial.println();
  // set up GPIO for Display
  io.sclk = DISPLAY_SCLK; // spi clock
  io.mosi = DISPLAY_MOSI; // spi data in
  io.busy = gpio_create(PAD_INPUT, DISPLAY_BUSY);
  pinMode(io.busy->pin, INPUT_PULLUP);
  gpio_update_mode(io.busy);
  io.res = gpio_create(PAD_OUTPUT, DISPLAY_RES);
  io.dc = gpio_create(PAD_OUTPUT, DISPLAY_DC);
  io.cs = gpio_create(PAD_OUTPUT, DISPLAY_CS);

  pinMode(RTC_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(RTC_INT), rtc_update, FALLING);
  now_t n = {};
  now = &n;
  Wire.begin();
  while (rtc.begin() == false)
  {
    //Serial.println("Failed to detect RV-3028-C7!");
    delay(5000);
  }
  rtc.disableClockOutput();
  //rtc.setDateTimeFromISO8601("2021-03-18T22:29:00"); // Hard-coded for testing
  //rtc.synchronize();                                 // Writes the new date time to RTC

  // set up SPI for display
  dsp = GDEW0154Z17_Init(DISPLAY_ROTATE_0, &io);
  //if (!dsp)
  //Serial.println("E-Ink Display init failed");
  //Serial.println("E-Ink Display initialized");

  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
  //Serial.println(wakeup_reason);
  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT1: //button Press
                              //handleButtonPress();
                              //break;
  case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
    display_fill(dsp, WHITE);
    sw_watch1_init(dsp, now);
    now->day = rtc.getDateTimeComponent(DATETIME_DAY_OF_MONTH);
    now->month = rtc.getDateTimeComponent(DATETIME_MONTH);
    now->minute = rtc.getDateTimeComponent(DATETIME_MINUTE);
    now->hour = rtc.getDateTimeComponent(DATETIME_HOUR);
    //Serial.println(rtc.getCurrentDateTime());
    sw_watch1();
    display_commit_fb(dsp);
    break;
  default: //reset
    // Refer to page 63 of Application Manual to configure timer value and timer
    // clock frequency
    rtc.setPeriodicCountdownTimer(/*timerValue=*/60, /*frequency=*/TIMER_1HZ,
                                  /*repeat=*/true);
    rtc.enableInterrupt(INTERRUPT_PERIODIC_COUNTDOWN_TIMER);
    rtc.startPeriodicCountdownTimer();
    sw_splashscreen(dsp);
    display_commit_fb(dsp);
    break;
  }
  esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_PIN, 0);                 //enable deep sleep wake on RTC interrupt
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  esp_deep_sleep_start();
}

void loop()
{
}
