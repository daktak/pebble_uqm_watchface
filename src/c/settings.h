#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

typedef struct ClaySettings {
  int ship_select;
  int ship_change;
  int ship_rotate;
  int cap_change;
  int turret_rotate;
  int insult_chance;
  int ywing_chance;
  bool animations;
  bool hd_gfx;
  //int last_ship;
  //int last_race;
} ClaySettings;

void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
void prv_load_settings();
ClaySettings get_settings();
