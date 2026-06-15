#include <pebble.h>

#include "ship.h"
#include "src/c/races.h"
#include "src/c/settings.h"
#include "src/c/main.h"
#include "src/c/captain.h"
#include "src/c/display_layer.h"

static GBitmap *ship_image;
static RotBitmapLayer *rot;
static GBitmap *turret_image;
static RotBitmapLayer *rott;

static int ship_int;

typedef struct {
  int res;
  int res_hd;
} ShipRes;

static ShipRes ships[YWING + 1] = {
  [SPATHI]     = { RESOURCE_ID_ELUDER,       RESOURCE_ID_ELUDER_HD2X },
  [ANDROSYNTH] = { RESOURCE_ID_GUARDIAN,     RESOURCE_ID_GUARDIAN_HD2X },
  [ARILOU]     = { RESOURCE_ID_SKIFF,        RESOURCE_ID_SKIFF_HD2X },
  [CHENJESU]   = { RESOURCE_ID_BROODHOME,    RESOURCE_ID_BROODHOME_HD2X },
  [CHMMR]      = { RESOURCE_ID_AVATAR,       RESOURCE_ID_AVATAR_HD2X },
  [DRUUGE]     = { RESOURCE_ID_MAULER,       RESOURCE_ID_MAULER_HD2X },
  [HUMAN]      = { RESOURCE_ID_CRUISER,      RESOURCE_ID_CRUISER_HD2X },
  [ILWRATH]    = { RESOURCE_ID_AVENGER,      RESOURCE_ID_AVENGER_HD2X },
  [KOHRAH]     = { RESOURCE_ID_MARAUDER,     RESOURCE_ID_MARAUDER_HD2X },
  [MELNORME]   = { RESOURCE_ID_TRADER,       RESOURCE_ID_TRADER_HD2X },
  [MMRNHRM]    = { RESOURCE_ID_XFORM,        RESOURCE_ID_XFORM_HD2X },
  [MYCON]      = { RESOURCE_ID_PODSHIP,      RESOURCE_ID_PODSHIP_HD2X },
  [ORZ]        = { RESOURCE_ID_NEMESIS,      RESOURCE_ID_NEMESIS_HD2X },
  [PKUNK]      = { RESOURCE_ID_FURY,         RESOURCE_ID_FURY_HD2X },
  [SHOFIXTI]   = { RESOURCE_ID_SCOUT,        RESOURCE_ID_SCOUT_HD2X },
  [SLYLANDRO]  = { RESOURCE_ID_PROBE,        RESOURCE_ID_PROBE_HD2X },
  [SUPOX]      = { RESOURCE_ID_BLADE,        RESOURCE_ID_BLADE_HD2X },
  [SYREEN]     = { RESOURCE_ID_PENETRATOR,   RESOURCE_ID_PENETRATOR_HD2X },
  [THRADDASH]  = { RESOURCE_ID_TORCH,        RESOURCE_ID_TORCH_HD2X },
  [UMGAH]      = { RESOURCE_ID_DRONE,        RESOURCE_ID_DRONE_HD2X },
  [URQUAN]     = { RESOURCE_ID_DREADNOUGHT,  RESOURCE_ID_DREADNOUGHT_HD2X },
  [UTWIG]      = { RESOURCE_ID_JUGGER,       RESOURCE_ID_JUGGER_HD2X },
  [VUX]        = { RESOURCE_ID_INTRUDER,     RESOURCE_ID_INTRUDER_HD2X },
  [YEHAT]      = { RESOURCE_ID_TERMINATOR,   RESOURCE_ID_TERMINATOR_HD2X },
  [ZOQFOTPIK]  = { RESOURCE_ID_STINGER,      RESOURCE_ID_STINGER_HD2X },
  [YWING]      = { RESOURCE_ID_YWING,        RESOURCE_ID_YWING_HD2X },
};

void restore_ship_int(int last_ship) {
  ship_int = last_ship;
}

static void rotate_layer(RotBitmapLayer *layer, struct tm *tick_time, int min, int interval) {
  if (interval != min) return;
  int unit = 0;
  if (min == 1)
    unit = tick_time->tm_sec * TRIG_MAX_ANGLE / 60;
  else if (min == 60)
    unit = tick_time->tm_min * TRIG_MAX_ANGLE / 60;
  else if (min == 12)
    unit = tick_time->tm_hour % 12 * TRIG_MAX_ANGLE / 12 + tick_time->tm_min * TRIG_MAX_ANGLE / (24 * 30);
  rot_bitmap_layer_set_angle(layer, unit);
}

void rotate(struct tm *tick_time, int min) {
  ClaySettings settings = get_settings();
  rotate_layer(rot, tick_time, min, settings.ship_rotate);
}

void rotate_turret(struct tm *tick_time, int min) {
  if (get_random_race_int() != ORZ) return;
  ClaySettings settings = get_settings();
  rotate_layer(rott, tick_time, min, settings.turret_rotate);
}

static void center_layer(RotBitmapLayer *layer, GRect bounds, int y_off) {
  GRect rb = layer_get_bounds((Layer*)layer);
  GPoint c = grect_center_point(&bounds);
  GRect f = { .origin = c, .size = bounds.size };
  f.origin.x -= rb.size.w / 2;
  f.origin.y -= rb.size.h / 2 + y_off;
  layer_set_frame((Layer*)layer, f);
}

void set_ship(ClaySettings settings, bool force){
  int random_race_int = get_random_race_int();
  int old_race = random_race_int;
  int old_ship = ship_int;
  random_race_int = set_race();
  ship_int = random_race_int;
  if (random_race_int == MMRNHRM) {
    if (force && old_ship > 0) {
      ship_int = old_ship;
    } else if (rand() % settings.ywing_chance == 1) {
      ship_int = YWING;
    }
  }

  if ((ship_int != old_ship) || force) {
    reset_timer(settings.ship_rotate, settings.turret_rotate);
    layer_remove_from_parent((Layer*)rott);
    layer_remove_from_parent((Layer*)rot);
    ShipRes *s = &ships[ship_int];
    int r = s->res;
    if (settings.hd_gfx && PBL_IF_COLOR_ELSE(true, false))
      r = s->res_hd;
    ship_image = gbitmap_create_with_resource(r);
    rot = rot_bitmap_layer_create(ship_image);
    GRect bounds = get_bounds();
    center_layer(rot, bounds, -3);
    rot_bitmap_set_compositing_mode(rot, PBL_IF_COLOR_ELSE(GCompOpSet, GCompOpAssign));
    if (settings.ship_rotate != 0) {
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp);
      rotate(tick_time, settings.ship_rotate);
    }
    Layer *window_layer = get_window_layer();
    layer_add_child(window_layer, (Layer*)rot);
    if (ship_int == ORZ) {
      if (settings.ship_rotate != 0) {
        time_t temp = time(NULL);
        struct tm *tick_time = localtime(&temp);
        rotate_turret(tick_time, settings.turret_rotate);
      }
      layer_add_child(window_layer, (Layer*)rott);
    }
  }
  if ((old_race != random_race_int) || force) {
    if (old_race != random_race_int)
      update_captain(get_captain(random_race_int));
    else
      update_captain(get_captain_by_index(random_race_int, settings.last_cap));
  }
  save_ship_state(ship_int, random_race_int, get_last_cap_index());
}

void create_turret(GRect bounds, Layer *window_layer, bool hd_gfx) {
  if (turret_image != NULL) gbitmap_destroy(turret_image);
  if (rott != NULL) rot_bitmap_layer_destroy(rott);
  int r = RESOURCE_ID_NEMESIS_TURRET;
  if (hd_gfx && PBL_IF_COLOR_ELSE(true, false))
    r = RESOURCE_ID_NEMESIS_TURRET_HD2X;
  turret_image = gbitmap_create_with_resource(r);
  rott = rot_bitmap_layer_create(turret_image);
  center_layer(rott, bounds, -3);
  rot_bitmap_set_compositing_mode(rott, PBL_IF_COLOR_ELSE(GCompOpSet, GCompOpOr));
}

void ship_unload() {
  gbitmap_destroy(ship_image);
  rot_bitmap_layer_destroy(rot);
  gbitmap_destroy(turret_image);
  rot_bitmap_layer_destroy(rott);
}
