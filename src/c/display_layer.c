#include <pebble.h>

#include "display_layer.h"
#include "src/c/settings.h"
#include "src/c/ship.h"

static TextLayer *s_time_layer;
static TextLayer *s_cap_layer;
static TextLayer *s_insult_layer;

//PKUNK set string
void update_insult(char *insult) {
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

//Time layer
void update_time() {
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
  
  create_turret(bounds, window_layer);
  
  //SHIP
  ClaySettings settings = get_settings();
  set_ship(settings);
}
