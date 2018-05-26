#include <pebble.h>
#include "src/c/races.h"
#include "src/c/captain.h"
#include "src/c/main.h"
/*
 TODO
 - Remember last ship and not redraw
 - Remember last captain name
 - Black and white support
 - refactor
 - font
 */

#define SETTINGS_KEY 1

typedef struct ClaySettings {
  int ship_select;
  int ship_change;
  int ship_rotate;
  int cap_change;
  int turret_rotate;
  //int last_ship;
  //int last_race;
} ClaySettings;
static ClaySettings settings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_cap_layer;
static TextLayer *s_insult_layer;
static GBitmap *ship_image;
static RotBitmapLayer *rot;
static GBitmap *turret_image;
static RotBitmapLayer *rott;

static int races[26] = {RESOURCE_ID_ELUDER, RESOURCE_ID_GUARDIAN, RESOURCE_ID_SKIFF, RESOURCE_ID_BROODHOME, RESOURCE_ID_AVATAR, RESOURCE_ID_MAULER,
                        RESOURCE_ID_CRUISER, RESOURCE_ID_AVENGER, RESOURCE_ID_MARAUDER, RESOURCE_ID_TRADER, RESOURCE_ID_XFORM, RESOURCE_ID_PODSHIP, 
                        RESOURCE_ID_NEMESIS, RESOURCE_ID_FURY, RESOURCE_ID_SCOUT, RESOURCE_ID_PROBE, RESOURCE_ID_BLADE, RESOURCE_ID_PENETRATOR,
                        RESOURCE_ID_TORCH, RESOURCE_ID_DRONE, RESOURCE_ID_DREADNOUGHT, RESOURCE_ID_JUGGER, RESOURCE_ID_INTRUDER,
                        RESOURCE_ID_TERMINATOR, RESOURCE_ID_STINGER, RESOURCE_ID_YWING};


int random_race_int;
int ship_int;
int current_insult = 0;
Layer *window_layer;
GRect bounds;

//helper - put int in buffer and log
static void log_int(int num) {
  static char s_buffer[10];
  snprintf(s_buffer, 10, "%i", num);
  APP_LOG(APP_LOG_LEVEL_INFO, s_buffer);
}

//PKUNK set string
static void update_insult(char *insult) {
  static char s_buffer[8];
  strcpy(s_buffer, insult);
  text_layer_set_text(s_insult_layer, s_buffer);
}

//Update captain name
void update_captain(char *captain) {
  static char s_buffer[10];
  strcpy(s_buffer, captain);
  text_layer_set_text(s_cap_layer, s_buffer);
}

//Randomize the race
int set_race() {
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


//ORZ Nemesis Turret rotation
static void rotate_turret(struct tm *tick_time, int min) {
  if (random_race_int == ORZ) {
    //APP_LOG(APP_LOG_LEVEL_INFO, "rotate turret");
    //log_int(min);
    int unit = 0;
    if (min==1){
      unit = tick_time->tm_sec * TRIG_MAX_ANGLE / 60;
    } else if (min==60) {
      unit = tick_time->tm_min * TRIG_MAX_ANGLE / 60;
    } else if (min==12) {
      unit = tick_time->tm_hour%12 * TRIG_MAX_ANGLE / 12 + tick_time->tm_min * TRIG_MAX_ANGLE / (24*30); 
    }
    if (settings.turret_rotate == min) {
      rot_bitmap_layer_set_angle(rott, unit);
    }
  }
}

//Rotate the ship body
static void rotate(struct tm *tick_time, int min) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "rotate");
  //log_int(min);
  int unit = 0;
  if (min==1){
    unit = tick_time->tm_sec * TRIG_MAX_ANGLE / 60;
  } else if (min==60) {
    unit = tick_time->tm_min * TRIG_MAX_ANGLE / 60;
  } else if (min==12) {
    unit = tick_time->tm_hour%12 * TRIG_MAX_ANGLE / 12 + tick_time->tm_min * TRIG_MAX_ANGLE / (24*30); 
  }
  if ((settings.ship_rotate == min) && (settings.ship_rotate != 0)) {
    rot_bitmap_layer_set_angle(rot, unit);
  }
}

//Change the ship image and rotate to correct orientation
static void set_ship(){
  //APP_LOG(APP_LOG_LEVEL_INFO, "set_ship");
  int old_race = random_race_int;
  int old_ship = ship_int;
  ship_int = set_race();
  //ship_int = random_race_int;
  // chance mmrnhrm is ywing
  if (random_race_int == MMRNHRM) {
    if (rand() %3 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "YWing");
      ship_int = YWING;
    }
  }
  
  if (ship_int != old_ship) {
    reset_timer(settings.ship_rotate);
    layer_remove_from_parent((Layer*)rott);
    layer_remove_from_parent((Layer*)rot);
    ship_image = gbitmap_create_with_resource(races[ship_int-1]);
    rot = rot_bitmap_layer_create(ship_image);
    GRect rbounds = layer_get_bounds((Layer*)rot);
    const GPoint center = grect_center_point(&bounds);
    GRect image_frame = (GRect) { .origin = center, .size = bounds.size };
    image_frame.origin.x -= rbounds.size.w/2; 
    image_frame.origin.y -= rbounds.size.h/2 - 3;
    layer_set_frame((Layer*)rot,image_frame);
    rot_bitmap_set_compositing_mode(rot, GCompOpSet);
    //set initial angle,
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp); 
    rotate(tick_time, settings.ship_rotate);
    layer_add_child(window_layer, (Layer*)rot);
    if (ship_int == ORZ) {
      //set initial angle,
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp); 
      rotate_turret(tick_time,settings.turret_rotate);
      layer_add_child(window_layer, (Layer*)rott);
    }
  }
  
  if (old_race != random_race_int) {
    update_captain(set_captain());
  }
}

//initiate the changes
static void change(int min) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "change");
  if (settings.ship_change == min) {
    random_race_int = 0;
    set_ship();
  } else if ((random_race_int == MMRNHRM)&&(min=-1)) {
    //give a chace for the xform to switch to ywing and vica versa
    if (rand() % 20 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Mmrnhrm chance to change");
      set_ship();
    }
  } else if ((random_race_int == MMRNHRM)&&(min==1)&&
             ((settings.ship_rotate!=1)||((settings.turret_rotate!=1)&&(random_race_int=ORZ)))) {
    //give a chace for the xform to switch to ywing and vica versa
    //higher chance
    if (rand() % 2 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Mmrnhrm chance to change");
      set_ship();
    }
  }
  if (settings.cap_change == min) {
    update_captain(set_captain());
  }
  
  if (current_insult > 0) {
    current_insult = -1;
  } else if (current_insult < 0) {
    update_insult("");
    current_insult = 0;
  }
  
  if ((settings.ship_rotate == 1)&&(random_race_int == PKUNK)&&(current_insult==0)) {
    if (rand()%30==1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Pkunk Insult triggered");
      current_insult = 1;
      update_insult(get_insult());
    }
  }
}

//Time layer
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
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
  if ((settings.ship_rotate == 1)||(settings.turret_rotate == 1)) {
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  };
}

void reset_timer(int old_rotate) {
    //reset up timer service
    if ((old_rotate==1)&& ((settings.ship_rotate == 1)||(settings.turret_rotate == 1))) {
      tick_timer_service_unsubscribe();
      set_ticker();
    } else if ((old_rotate!=1)&& ((settings.ship_rotate == 1)||(settings.turret_rotate == 1))) {
      tick_timer_service_unsubscribe();
      set_ticker();
    }
}

//deint
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

//destroy
static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(ship_image);
  rot_bitmap_layer_destroy(rot);
  text_layer_destroy(s_cap_layer);
  text_layer_destroy(s_insult_layer);
  gbitmap_destroy(turret_image);
  rot_bitmap_layer_destroy(rott);
}

//start
static void main_window_load(Window *window) {
  // Get information about the Window
  window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);

  // Time
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(10, 2), bounds.size.w, 50)); //58.52
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //Insult 
  s_insult_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(50,44), bounds.size.w - PBL_IF_ROUND_ELSE(24,12), 40));
  text_layer_set_background_color(s_insult_layer, GColorBlack);
  text_layer_set_text_color(s_insult_layer, GColorWhite);
  //text_layer_set_text(s_insult_layer, "Dou-Dou");
  text_layer_set_font(s_insult_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_insult_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_insult_layer));
  
  //Captain Name
  s_cap_layer = text_layer_create(
      GRect(0, bounds.size.h - 32, bounds.size.w, 50));
  text_layer_set_background_color(s_cap_layer, GColorBlack);
  text_layer_set_text_color(s_cap_layer, GColorWhite);
  //update_captain();
  text_layer_set_font(s_cap_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_cap_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_cap_layer));
  
  //Nemesis Turret
  turret_image = gbitmap_create_with_resource(RESOURCE_ID_NEMESIS_TURRET);
  rott = rot_bitmap_layer_create(turret_image);
  GRect rbounds = layer_get_bounds((Layer*)rott);
  const GPoint center = grect_center_point(&bounds);
  GRect image_frame = (GRect) { .origin = center, .size = bounds.size };
  image_frame.origin.x -= rbounds.size.w/2; 
  image_frame.origin.y -= rbounds.size.h/2 - 3;
  layer_set_frame((Layer*)rott,image_frame);
  rot_bitmap_set_compositing_mode(rott, GCompOpSet);
  
  //SHIP
  set_ship();
}
// Initialize the default settings
static void prv_default_settings() {
  settings.ship_rotate = 60;
  settings.ship_change = 60;
  settings.cap_change = 5;
  settings.ship_select = 0;
  settings.turret_rotate = 1;
  //settings.last_ship = 0;
  //settings.last_race = 0;
}

static void prv_load_settings() {
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
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) { 
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
      set_ship();
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
    reset_timer(old_rotate);
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
    
    reset_timer(old_rotate);
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
  deinit();
}