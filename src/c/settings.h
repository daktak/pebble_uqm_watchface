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
  bool quiet_time;
  int quiet_start;
  int quiet_stop;
  int last_ship;
  int last_race;
  int last_cap;
} ClaySettings;

void inbox_received_handler(DictionaryIterator *iter, void *context);
void load_settings();
ClaySettings get_settings();
void save_ship_state(int ship, int race, int cap);
