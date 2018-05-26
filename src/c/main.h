#pragma once
#include "src/c/settings.h"
void reset_timer(int old_rotate);
int set_race();
void set_ship(ClaySettings settings);
void rotate(struct tm *tick_time, int min) ;
void rotate_turret(struct tm *tick_time, int min) ;
int get_random_race_int();
#define SETTINGS_KEY 1
