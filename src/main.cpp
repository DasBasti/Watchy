#include <Arduino.h>

extern "C"
{
#include <Platinenmacher.h>
#include "boards.h"
#include "sw_screens.h"
}

display_t *dsp;
static gdew0154z17_io_config_t io = {};

void setup()
{
  Serial.begin(115200);
  Serial.println();
  // set up GPIO for Display
  io.sclk = DISPLAY_SCLK; // spi clock
  io.mosi = DISPLAY_MOSI; // spi data in
  io.busy = gpio_create(PAD_INPUT, DISPLAY_BUSY);
  pinMode(io.busy->pin, INPUT_PULLUP);
  gpio_update_mode(io.busy);
  io.res = gpio_create(PAD_OUTPUT, DISPLAY_RES);
  io.dc = gpio_create(PAD_OUTPUT, DISPLAY_DC);
  io.cs = gpio_create(PAD_OUTPUT, DISPLAY_CS);

  delay(1000);
  Serial.println(">>> Go <<<");
  // set up SPI for display
  dsp = GDEW0154Z17_Init(DISPLAY_ROTATE_0, &io);
  if (!dsp)
    Serial.println("E-Ink Display init failed");
  Serial.println("E-Ink Display initialized");

  // RTC aktivieren: Wakeup trigger konfigurieren

  // Beschleunigungssensor aktivieren / tap trigger

  // Watchface starten / antippen
  sw_splashscreen(dsp);
  display_commit_fb(dsp);
}

void loop()
{

  Serial.println("loop.");
  delay(10000);
}