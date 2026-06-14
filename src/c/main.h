#pragma once
#include "src/c/settings.h"
void reset_timer(int ship_rotate, int turret_rotate);
int set_race();
void set_ship(ClaySettings settings, bool force);
void rotate(struct tm *tick_time, int min) ;
void rotate_turret(struct tm *tick_time, int min) ;
int get_random_race_int();
GRect get_bounds();
Layer *get_window_layer();
