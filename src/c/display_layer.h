#pragma once
#include <pebble.h>

void update_captain(char *captain);
void update_insult(char *insult);
void update_time();
void destroy_display_layer();
void destroy_text_layer();
void window_load(GRect bounds, Layer *window_layer);