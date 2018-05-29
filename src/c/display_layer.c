#include <pebble.h>

#include "display_layer.h"
#include "src/c/settings.h"
#include "src/c/ship.h"

static TextLayer *s_time_layer;
static TextLayer *s_cap_layer;
static TextLayer *s_insult_layer;
bool first_position[4] = {true,true,true};
GRect gbounds;
char *s_captain;
Animation *anim[4];
char *s_insult;

void set_insult() {
  static char s_buffer[8];
  strcpy(s_buffer, s_insult);
  text_layer_set_text(s_insult_layer, s_buffer);
}
//PKUNK set string
void update_insult(char *insult) {
  s_insult = insult;
  ClaySettings settings = get_settings();
  if (settings.animations) {
    animate_layer(gbounds, s_insult_layer, true, INSULT);
  } else {
    set_insult();
  }
}

void set_captain() {
  static char s_buffer[10];
  strcpy(s_buffer, s_captain);
  text_layer_set_text(s_cap_layer, s_buffer);
}

//Update captain name
void update_captain(char *captain) {
  ClaySettings settings = get_settings();
  s_captain = captain;
  if (settings.animations) {
    animate_layer(gbounds, s_cap_layer, true, CAPTAIN);
  } else {
    set_captain();
  }
}

void anim_started_handler(Animation *animation, void *context) {
}

void set_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
           "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, s_buffer);
}


void anim_stopped_handler(Animation *animation, bool finished, void *context) {
  int layer_number = 0;
  if (anim[TIME] == animation) {
    layer_number = TIME;
  } else if (anim[CAPTAIN] == animation) {
    layer_number = CAPTAIN;
  } else if (anim[INSULT] == animation) {
    layer_number = INSULT;
  }
  
  if (first_position[layer_number]) {
    switch (layer_number) {
      case TIME:
        set_time();
        animate_layer(gbounds, s_time_layer, false, layer_number);
        break;
      case CAPTAIN:
        set_captain();
        animate_layer(gbounds, s_cap_layer, false, layer_number);
        break;
      case INSULT:
        set_insult();
        animate_layer(gbounds, s_insult_layer, false, layer_number);
      default:
        break;
    }
    
  }
}

void animate_layer(GRect bounds, TextLayer *text_layer, bool first, int layer_num) {
  GRect offscreen;
  GRect onscreen;
  PropertyAnimation *prop_anim;
  first_position[layer_num] = first;
  // Choose parameters
  int delay_ms = 0;
  int duration_ms = 500;
  
  switch (layer_num) {
    case TIME:
      offscreen = GRect(0, -50, bounds.size.w, 0);
      onscreen = GRect(0, PBL_IF_ROUND_ELSE(10, 2), bounds.size.w, 50);
      break;
    case CAPTAIN:
      onscreen = GRect(0, bounds.size.h - 32, bounds.size.w, 50);
      offscreen = GRect(0, bounds.size.h, bounds.size.w, 50);
      break;
    case INSULT:
      delay_ms = 500;
      duration_ms = 200;
      onscreen = GRect(bounds.size.w - PBL_IF_ROUND_ELSE(74,62), PBL_IF_ROUND_ELSE(50,44), bounds.size.w - PBL_IF_ROUND_ELSE(24,12), 40);
      offscreen = GRect(bounds.size.w/2, bounds.size.h/2, 0, 0);
      break;
    default:
      break;
  }
  
  if (first) {
    prop_anim = property_animation_create_layer_frame(text_layer_get_layer(text_layer), &onscreen, &offscreen);
  } else {
    prop_anim = property_animation_create_layer_frame(text_layer_get_layer(text_layer), &offscreen, &onscreen);
  }
  
  // Get the Animation
  anim[layer_num] = property_animation_get_animation(prop_anim);
  
  
  // Configure the Animation's curve, delay, and duration
  animation_set_curve(anim[layer_num], AnimationCurveEaseOut);
  animation_set_delay(anim[layer_num], delay_ms);
  animation_set_duration(anim[layer_num], duration_ms);
  
  animation_set_handlers(anim[layer_num], (AnimationHandlers) {
    .started = anim_started_handler,
    .stopped = anim_stopped_handler
  }, NULL);
  
  animation_schedule(anim[layer_num]);
}

//Time layer
void update_time(GRect bounds) {
  gbounds = bounds;
  ClaySettings settings = get_settings();
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  if ((settings.animations) &&
    (((settings.ship_change == 1) && (tick_time->tm_sec == 0))||
     ((settings.ship_change == 60) && (tick_time->tm_min == 0))||
     ((settings.ship_change == 5) && (tick_time->tm_min%5 == 0))||
     ((settings.ship_change == 10) && (tick_time->tm_min%10 == 0)))) {
      animate_layer(bounds, s_time_layer, true, TIME);
  } else {
    set_time();
  }
}


void destroy_text_layer() {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_cap_layer);
  text_layer_destroy(s_insult_layer);
}

void window_load(GRect bounds, Layer *window_layer) {
  // Time
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(10, 2), bounds.size.w, 50)); //58.52
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  //text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //Insult 
  s_insult_layer = text_layer_create(
      GRect(bounds.size.w - PBL_IF_ROUND_ELSE(74,62) , PBL_IF_ROUND_ELSE(50,44), bounds.size.w - PBL_IF_ROUND_ELSE(24,12), 40));
  text_layer_set_background_color(s_insult_layer, GColorBlack);
  text_layer_set_text_color(s_insult_layer, GColorWhite);
  //text_layer_set_text(s_insult_layer, "Dou-Dou");
  text_layer_set_font(s_insult_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_insult_layer, GTextAlignmentLeft);
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
 
  ClaySettings settings = get_settings();
  create_turret(bounds, window_layer, settings.hd_gfx);
  set_ship(settings);
}
