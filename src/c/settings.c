#include <pebble.h>

#include "settings.h"
#include "src/c/main.h"
#include "src/c/races.h"

static ClaySettings settings;

/* helper - put int in buffer and log
void log_int(int num) {
  static char s_buffer[10];
  snprintf(s_buffer, 10, "%i", num);
  APP_LOG(APP_LOG_LEVEL_INFO, s_buffer);
}*/

ClaySettings get_settings() {
  return settings;
}

// Initialize the default settings
static void prv_default_settings() {
  settings.ship_rotate = 60;
  settings.ship_change = 60;
  settings.cap_change = 5;
  settings.ship_select = 0;
  settings.turret_rotate = 1;
  settings.insult_chance = 25;
  settings.ywing_chance = 3;
  settings.animations = true;
  //settings.last_ship = 0;
  //settings.last_race = 0;
}

void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
  //ship_int = settings.last_ship;
  //random_race_int = settings.last_race;
}

static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

//inbox
void prv_inbox_received_handler(DictionaryIterator *iter, void *context) { 
  int random_race_int = get_random_race_int();
  Tuple *ship_select_t = dict_find(iter, MESSAGE_KEY_ShipSelection);
  Tuple *ship_change_t = dict_find(iter, MESSAGE_KEY_ShipChange);
  Tuple *ship_rotate_t = dict_find(iter, MESSAGE_KEY_ShipRotate);
  Tuple *cap_change_t = dict_find(iter, MESSAGE_KEY_CapChange);
  Tuple *turret_rotate_t = dict_find(iter, MESSAGE_KEY_TurretRotate);
  if (ship_select_t) {
    int old_ship = settings.ship_select;
    settings.ship_select = atoi(ship_select_t->value->cstring);
    //log_int(settings.ship_select);
    if (old_ship != settings.ship_select) {
      set_ship(settings);
    }
  }
  if (turret_rotate_t) {
    int old_rotate = settings.turret_rotate;
    settings.turret_rotate = atoi(turret_rotate_t->value->cstring);
    //log_int(settings.turret_rotate);
    if ((old_rotate != settings.turret_rotate)&&(random_race_int == ORZ)) {
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp); 
      rotate_turret(tick_time, settings.turret_rotate);
    }
    reset_timer(old_rotate, settings.ship_rotate, settings.turret_rotate);
  }
  if (ship_rotate_t) {
    int old_rotate  = settings.ship_rotate;
    settings.ship_rotate = atoi(ship_rotate_t->value->cstring);
    //log_int(settings.ship_rotate);
    
    //redraw ship rotation
    if (old_rotate != settings.ship_rotate) {
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp); 
      rotate(tick_time, settings.ship_rotate);
    }
    
    reset_timer(old_rotate, settings.ship_rotate, settings.turret_rotate);
  }
  if (ship_change_t) {
    settings.ship_change = atoi(ship_change_t->value->cstring);
    //log_int(settings.ship_change);
  }
  if (cap_change_t) {
    settings.cap_change = atoi(cap_change_t->value->cstring);
    //log_int(settings.cap_change);
  }
  prv_save_settings();
}