#include <Arduino.h>
#include <lv_conf.h>
#include <lvgl.h>

#include "gui/gui.h"

void setup() {
  gui_start();
}

void loop() {
  lv_timer_handler();

  delay(5);
}

