#include <pebble.h>
#include "homescreen.h"

static void init() {
  show_homescreen();
}

static void deinit() {
  hide_homescreen();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
