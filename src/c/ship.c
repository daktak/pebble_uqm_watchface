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

static int races[26] = {RESOURCE_ID_ELUDER, RESOURCE_ID_GUARDIAN, RESOURCE_ID_SKIFF, RESOURCE_ID_BROODHOME, RESOURCE_ID_AVATAR, RESOURCE_ID_MAULER,
                        RESOURCE_ID_CRUISER, RESOURCE_ID_AVENGER, RESOURCE_ID_MARAUDER, RESOURCE_ID_TRADER, RESOURCE_ID_XFORM, RESOURCE_ID_PODSHIP, 
                        RESOURCE_ID_NEMESIS, RESOURCE_ID_FURY, RESOURCE_ID_SCOUT, RESOURCE_ID_PROBE, RESOURCE_ID_BLADE, RESOURCE_ID_PENETRATOR,
                        RESOURCE_ID_TORCH, RESOURCE_ID_DRONE, RESOURCE_ID_DREADNOUGHT, RESOURCE_ID_JUGGER, RESOURCE_ID_INTRUDER,
                        RESOURCE_ID_TERMINATOR, RESOURCE_ID_STINGER, RESOURCE_ID_YWING};

int ship_int;

/*
static void anim_started_handler(Animation *animation, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Animation started!");
}

static void anim_stopped_handler(Animation *animation, bool finished, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Animation stopped!");
}

void animate_ship() {
    GRect bounds = get_bounds();
    GRect onscreen = layer_get_frame((Layer*)rot);
    GRect offscreen = GRect(bounds.size.w/2-15, bounds.size.h, bounds.size.w, bounds.size.h);
    PropertyAnimation *prop_anim = property_animation_create_layer_frame((Layer*)rot, &offscreen, &onscreen);
      // Get the Animation
    Animation *anim = property_animation_get_animation(prop_anim);
    
    // Choose parameters
    const int delay_ms = 1000;
    const int duration_ms = 500;
    
    // Configure the Animation's curve, delay, and duration
    animation_set_curve(anim, AnimationCurveEaseOut);
    animation_set_delay(anim, delay_ms);
    animation_set_duration(anim, duration_ms);
    // Set some handlers
    animation_set_handlers(anim, (AnimationHandlers) {
      .started = anim_started_handler,
      .stopped = anim_stopped_handler
    }, NULL);
    // Play the animation
    animation_schedule(anim);
}
*/
//ORZ Nemesis Turret rotation
void rotate_turret(struct tm *tick_time, int min) {
  ClaySettings settings = get_settings();
  int random_race_int = get_random_race_int();
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
void rotate(struct tm *tick_time, int min) {
  ClaySettings settings = get_settings();
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
void set_ship(ClaySettings settings){
  int random_race_int = get_random_race_int();
  //APP_LOG(APP_LOG_LEVEL_INFO, "set_ship");
  int old_race = random_race_int;
  int old_ship = ship_int;
  random_race_int = set_race();
  ship_int = random_race_int;
  //ship_int = random_race_int;
  // chance mmrnhrm is ywing
  if (random_race_int == MMRNHRM) {
    if (rand() % settings.ywing_chance == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "YWing");
      ship_int = YWING;
    }
  }
  
  if (ship_int != old_ship) {
    reset_timer(settings.ship_rotate, settings.ship_rotate, settings.turret_rotate);
    layer_remove_from_parent((Layer*)rott);
    layer_remove_from_parent((Layer*)rot);
    ship_image = gbitmap_create_with_resource(races[ship_int-1]);
    rot = rot_bitmap_layer_create(ship_image);
    GRect rbounds = layer_get_bounds((Layer*)rot);
    GRect bounds = get_bounds();
    const GPoint center = grect_center_point(&bounds);
    GRect image_frame = (GRect) { .origin = center, .size = bounds.size };
    image_frame.origin.x -= rbounds.size.w/2; 
    image_frame.origin.y -= rbounds.size.h/2 - 3;
    layer_set_frame((Layer*)rot,image_frame);
    
    rot_bitmap_set_compositing_mode(rot, PBL_IF_COLOR_ELSE(GCompOpSet,GCompOpOr));
    //set initial angle,
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp); 
    rotate(tick_time, settings.ship_rotate);
    Layer *window_layer = get_window_layer();
    layer_add_child(window_layer, (Layer*)rot);
    //animate_ship();
    if (ship_int == ORZ) {
      //set initial angle,
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp); 
      rotate_turret(tick_time,settings.turret_rotate);
      layer_add_child(window_layer, (Layer*)rott);
    }
  }
  if ((old_race != random_race_int)||((old_race == 0)&&(random_race_int==0))) {
    update_captain(get_captain(random_race_int));
  }
}

void create_turret(GRect bounds, Layer *window_layer) {
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
}

void ship_unload() {
  gbitmap_destroy(ship_image);
  rot_bitmap_layer_destroy(rot);
  gbitmap_destroy(turret_image);
  rot_bitmap_layer_destroy(rott);
}
