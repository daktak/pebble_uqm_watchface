#pragma once
#include <pebble.h>

#define TIME 1
#define CAPTAIN 2
#define INSULT 3

void update_captain(char *captain);
void update_insult(char *insult);
void update_time(GRect bounds);
void destroy_display_layer();
void destroy_text_layer();
void window_load(GRect bounds, Layer *window_layer);
void animate_layer(GRect bounds, TextLayer *text_layer, bool first, int layer_num);