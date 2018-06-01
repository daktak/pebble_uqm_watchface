#pragma once
#include "src/c/settings.h"
void ship_unload();
void create_turret(GRect bounds, Layer *window_layer, int hd_gfx);
void set_ship(ClaySettings settings);
