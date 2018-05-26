#pragma once
#include <pebble.h>

typedef struct ClaySettings {
  int ship_select;
  int ship_change;
  int ship_rotate;
  int cap_change;
  int turret_rotate;
  //int last_ship;
  //int last_race;
} ClaySettings;

void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
void prv_load_settings();
ClaySettings get_settings();