#include <pebble.h>

#include "display_layer.h"
#include "src/c/settings.h"
#include "src/c/ship.h"

// Platform-specific font sizes and layout constants
#if defined(PBL_PLATFORM_GABBRO)
  #define TIME_FONT FONT_KEY_LECO_42_NUMBERS
  #define CAP_FONT FONT_KEY_GOTHIC_28
  #define INSULT_FONT FONT_KEY_GOTHIC_24
  #define TIME_Y 12
  #define TIME_H 70
  #define CAP_BOTTOM 42
  #define CAP_H 55
  #define INSULT_X_OFF 100
  #define INSULT_Y 55
  #define INSULT_W_OFF 30
  #define INSULT_H 40
#elif defined(PBL_PLATFORM_EMERY)
  #define TIME_FONT FONT_KEY_LECO_36_BOLD_NUMBERS
  #define CAP_FONT FONT_KEY_GOTHIC_24
  #define INSULT_FONT FONT_KEY_GOTHIC_18
  #define TIME_Y 5
  #define TIME_H 60
  #define CAP_BOTTOM 38
  #define CAP_H 50
  #define INSULT_X_OFF 80
  #define INSULT_Y 50
  #define INSULT_W_OFF 20
  #define INSULT_H 40
#elif defined(PBL_PLATFORM_CHALK)
  #define TIME_FONT FONT_KEY_LECO_32_BOLD_NUMBERS
  #define CAP_FONT FONT_KEY_GOTHIC_18
  #define INSULT_FONT FONT_KEY_GOTHIC_14
  #define TIME_Y 10
  #define TIME_H 55
  #define CAP_BOTTOM 32
  #define CAP_H 50
  #define INSULT_X_OFF 74
  #define INSULT_Y 50
  #define INSULT_W_OFF 24
  #define INSULT_H 40
#else
  #define TIME_FONT FONT_KEY_LECO_28_LIGHT_NUMBERS
  #define CAP_FONT FONT_KEY_GOTHIC_18
  #define INSULT_FONT FONT_KEY_GOTHIC_14
  #define TIME_Y 2
  #define TIME_H 50
  #define CAP_BOTTOM 32
  #define CAP_H 50
  #define INSULT_X_OFF 62
  #define INSULT_Y 44
  #define INSULT_W_OFF 12
  #define INSULT_H 40
#endif

static TextLayer *s_time_layer;
static TextLayer *s_cap_layer;
static TextLayer *s_insult_layer;
static bool first_position[4] = {true,true,true};
static GRect gbounds;
static char *s_captain;
static Animation *anim[4];
static char *s_insult;

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
  if (s_captain != NULL && strcmp(s_captain, captain) == 0) {
    set_captain();
    return;
  }
  bool first = (s_captain == NULL);
  ClaySettings settings = get_settings();
  s_captain = captain;
  if (settings.animations && !first) {
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
      offscreen = GRect(0, -(TIME_H + 10), bounds.size.w, 0);
      onscreen = GRect(0, TIME_Y, bounds.size.w, TIME_H);
      break;
    case CAPTAIN:
      onscreen = GRect(0, bounds.size.h - CAP_BOTTOM, bounds.size.w, CAP_H);
      offscreen = GRect(0, bounds.size.h, bounds.size.w, CAP_H);
      break;
    case INSULT:
      delay_ms = 500;
      duration_ms = 200;
      onscreen = GRect(bounds.size.w - INSULT_X_OFF, INSULT_Y, bounds.size.w - INSULT_W_OFF, INSULT_H);
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
  gbounds = bounds;
  // Time
  s_time_layer = text_layer_create(
      GRect(0, TIME_Y, bounds.size.w, TIME_H));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(TIME_FONT));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  //Insult
  s_insult_layer = text_layer_create(
      GRect(bounds.size.w - INSULT_X_OFF, INSULT_Y, bounds.size.w - INSULT_W_OFF, INSULT_H));
  text_layer_set_background_color(s_insult_layer, GColorBlack);
  text_layer_set_text_color(s_insult_layer, GColorWhite);
  text_layer_set_font(s_insult_layer, fonts_get_system_font(INSULT_FONT));
  text_layer_set_text_alignment(s_insult_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(s_insult_layer));

  //Captain Name
  s_cap_layer = text_layer_create(
      GRect(0, bounds.size.h - CAP_BOTTOM, bounds.size.w, CAP_H));
  text_layer_set_background_color(s_cap_layer, GColorBlack);
  text_layer_set_text_color(s_cap_layer, GColorWhite);
  text_layer_set_font(s_cap_layer, fonts_get_system_font(CAP_FONT));
  text_layer_set_text_alignment(s_cap_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_cap_layer));

  ClaySettings settings = get_settings();
  create_turret(bounds, window_layer, settings.hd_gfx);
  set_ship(settings, true);
}
