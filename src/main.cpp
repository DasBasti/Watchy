#include <Arduino.h>
#include <RV3028C7.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

extern "C"
{
#include <Platinenmacher.h>
#include "boards.h"
#include "sw_screens.h"
#include "sw_rtc.h"
}

const char *ssid = "Zuck";
const char *password = "12348765";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

display_t *dsp;
static gdew0154z17_io_config_t io = {};
RV3028C7 rtc;
now_t *now;
bool update_screen = false;
HTTPClient http;

void rtc_update()
{
}

void setRTC()
{
  char time_buf[20];
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  strftime(time_buf, sizeof(time_buf), "%FT%H:%M:%S", &timeinfo);
  rtc.setDateTimeFromISO8601(time_buf); // Hard-coded for testing
  rtc.synchronize();                    // Writes the new date time to RTC
}

void handleButtonPress()
{
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  /*
  http.begin(time_url);

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    rtc.setDateTimeFromISO8601(payload); // Hard-coded for testing
    rtc.synchronize();                   // Writes the new date time to RTC
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  */

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  setRTC();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);
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
    Serial.println("Failed to detect RV-3028-C7!");
    delay(5000);
  }
  rtc.disableClockOutput();

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
    handleButtonPress();
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
