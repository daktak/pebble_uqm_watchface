#include <pebble.h>

#include "src/c/races.h"
#include "src/c/captain.h"
#include "src/c/main.h"
#include "src/c/settings.h"
#include "src/c/ship.h"
#include "src/c/display_layer.h"

/*
 TODO
 - Remember last ship and not redraw
 - Remember last captain name
 - Black and white support
 - improve ship resolution
 */


int random_race_int;
int current_insult = 0;
static Window *s_main_window;
Layer *window_layer;
GRect bounds;

GRect get_bounds() {
  return bounds;
}

Layer *get_window_layer() {
  return window_layer;
}

int get_random_race_int() {
  return random_race_int;
}

//Randomize the race
int set_race() {
  ClaySettings settings = get_settings();
  if (settings.ship_select == 0) {
    if ((random_race_int <= 0)||(random_race_int > 26)) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Randomizing race");
      random_race_int = rand() % 25 +1;
    }
  } else {
    random_race_int = settings.ship_select;
  }
  //settings.last_race = random_race_int;
  return random_race_int;
}


//initiate the changes
static void change(int min) {
  ClaySettings settings = get_settings();
  //APP_LOG(APP_LOG_LEVEL_INFO, "change");
  if (settings.ship_change == min) {
    random_race_int = 0;
    set_ship(settings);
  } else if ((random_race_int == MMRNHRM)&&(min=-1)) {
    //give a chace for the xform to switch to ywing and vica versa
    if (rand() % 20 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Mmrnhrm chance to change");
      set_ship(settings);
    }
  } else if ((random_race_int == MMRNHRM)&&(min==1)&&
             ((settings.ship_rotate!=1)||((settings.turret_rotate!=1)&&(random_race_int=ORZ)))) {
    //give a chace for the xform to switch to ywing and vica versa
    //higher chance
    if (rand() % 2 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Mmrnhrm chance to change");
      set_ship(settings);
    }
  }
  if (settings.cap_change == min) {
    update_captain(get_captain());
  }
  
  if (current_insult > 0) {
    current_insult = -1;
  } else if (current_insult < 0) {
    update_insult("");
    current_insult = 0;
  }
  
  if ((settings.ship_rotate == 1)&&(random_race_int == PKUNK)&&(current_insult==0)) {
    if (rand()% settings.insult_chance ==1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Pkunk Insult triggered");
      current_insult = 1;
      update_insult(get_insult());
    }
  }
}

//trigger timing updates
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  rotate(tick_time,1);
  rotate_turret(tick_time,1);
  change(-1);
  
  if(tick_time->tm_sec == 0){ // Every minute
    update_time();
    change(1);
    rotate(tick_time,60);
    rotate_turret(tick_time,60);
    if(tick_time->tm_min%5 == 0){
      change(5);
      if(tick_time->tm_min%10 == 0){ // Every ten minutes
        rotate(tick_time,12);
        rotate_turret(tick_time,12);
        change(10);
        if(tick_time->tm_min == 0){ //Every hour
          change(60);
        }
      }
    }
  }
}

static void set_ticker() {
  ClaySettings settings = get_settings();
  if ((settings.ship_rotate == 1)||(settings.turret_rotate == 1)) {
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  };
}

void reset_timer(int old_rotate, int ship_rotate, int turret_rotate) {
    //reset up timer service
    if ((old_rotate==1)&& ((ship_rotate == 1)||(turret_rotate == 1))) {
      tick_timer_service_unsubscribe();
      set_ticker();
    } else if ((old_rotate!=1)&& ((ship_rotate == 1)||(turret_rotate == 1))) {
      tick_timer_service_unsubscribe();
      set_ticker();
    }
}

//destroy
static void main_window_unload(Window *window) {
  // Destroy TextLayer
  destroy_text_layer();
  ship_unload();
}

//start
static void main_window_load(Window *window) {
  // Get information about the Window
  window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);
  window_load(bounds, window_layer);
}

static void init() {
  prv_load_settings();
  // Create main Window element and assign to pointer
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  // Register with TickTimerService
  set_ticker();
  // Make sure the time is displayed from the start
  update_time();
}

int main(void) {
  init();
  app_event_loop();
  window_destroy(s_main_window);
}