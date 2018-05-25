#include <pebble.h>
/*
 TODO
 - Remember last ship and not redraw
 - Remember last captain name
 - Black and white support
 - refactor
 - font
 - round bring time down a tad
 */
#define SPATHI 1
#define ANDROSYNTH 2
#define ARILOU 3
#define CHENJESU 4
#define CHMMR 5
#define DRUUGE 6
#define HUMAN 7
#define ILWRATH 8
#define KOHRAH 9
#define MELNORME 10
#define MMRNHRM 11
#define MYCON 12
#define ORZ 13
#define PKUNK 14
#define SHOFIXTI 15
#define SLYLANDRO 16
#define SUPOX 17
#define SYREEN 18
#define THRADDASH 19
#define UMGAH 20
#define URQUAN 21
#define UTWIG 22
#define VUX 23
#define YEHAT 24
#define ZOQFOTPIK 25
#define YWING 26

#define SETTINGS_KEY 1

typedef struct ClaySettings {
  int ship_select;
  int ship_change;
  int ship_rotate;
  int cap_change;
  int turret_rotate;
  //int last_ship;
  //int last_race;
} ClaySettings;
static ClaySettings settings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_cap_layer;
static TextLayer *s_insult_layer;
static GBitmap *ship_image;
static RotBitmapLayer *rot;
static GBitmap *turret_image;
static RotBitmapLayer *rott;
//spathi
static char eluder_cap[17][10]={"Thwil","Pwappy","Phwiff","Wiffy","Plibnik","Snurfel","Kwimp","Pkunky","Jinkeze","Thintho","Rupatup","Nargle","Phlendo","Snelopy","Bwinkin","Whuff","Fwiffo"};
//androsynth
static char guardian_cap[16][10]={"BOOJI-1","DORN-3","BIM-XT","JOR-15","976-KILL","KORB-7B","XR4-TI","CRC-16","BHS-79","DOS-1.0","ME-262","AK-47","1040-EZ","NECRO-99","HAL-2001","SR-71"};
//ariloulaleelay
static char skiff_cap[16][10]={"Fefaloo","Bezabu","Tiptushi","Marypup","Tinkafo","Patooti","Tifiwilo","Loleelu","Louifoui","Pinywiny","Oowbabe","Dingdup","Wewalia","Yipyapi","Ropilup","Wolwali"};
//chenjesu
static char broodhome_cap[16][10]={"Kzzakk","Tzrrow","Zzmzmm","Vziziz","Hmmhmm","Bzrak","Krrtzz","Zzzzz","Zxzakz","Brrzap","Tzaprak","Pzkrakz","Fzzzz","Vrroww","Zznaz","Zzzhmm"};
//chmmr
static char avatar_cap[16][10]={"Mnzgk","Chzrmn","Bzztrm","Zrnzrk","Tzzqrn","Kzzrn","Vzrzn","Qrntz","Rmnzk","Szmrnz","Zbzzn","Frnkzk","Prmtzz","Tzrtzn","Kztztz","Mrnkzt"};
//druuge
static char mauler_cap[16][10]={"Tuuga","Siinur","Kaapo","Juugl","Paato","Feezo","Maad","Moola","Kooli","Faazur","Zooto","Biitur","Duulard","Piini","Soopi","Peeru"};
//human
static char cruiser_cap[16][10]={"Decker","Trent","Adama","Spiff","Graeme","Kirk","Pike","Halleck","Tuf","Pirx","Wu","VanRijn","Ender","Buck","Solo","Belt"};
//ilwrath
static char avenger_cap[16][10]={"Gorgon","Taragon","Kalgon","Borgo","Dirga","Slygor","Rogash","Argarak","Kayzar","Baylor","Zoggak","Targa","Vogar","Lurgo","Regorjo","Manglor"};
//kohrah
static char marauder_cap[16][10]={"Death 11","Death 17","Death 37","Death 23","Death 7","Death 13","Death 19","Death 29","Death 31","Death 41","Death 43","Death 3","Death 5","Death 47","Death 53","Death 83"};
//melnorme
static char trader_cap[16][10]={"Reddish","Orangy","Aqua","Crimson","Magenta","Cheruse","Beige","Fuchsia","Umber","Cerise","Mauve","Grayish","Yellow","Black","Bluish","Purple"};
//mmrnhrm
static char xform_cap[16][10]={"Qir-nha","Jhe-qir","Qua-rhna","Mn-quah","Nrna-mha","Um-hrh","Hm-nhuh","Rrma-hrn","Jra-nr","Ur-mfrs","Qua-qir","Mrm-na","Jhe-mhr","Hmr-hun","Nhuh-na","Hrnm-hm"};
//mycon
static char podship_cap[16][10]={"Blort","Chupp","Floos","Glish","Glob","Glush","Plork","Shlish","Shlupp","Slingy","Sploozo","Spork","Uffo","Yush","Zaloop","Znuff"};
//orz
static char nemesis_cap[16][10] = {"*Wet*","*Happy*","*Frumple*","*Camper*","*Loner*","*Dancer*","*Singer*","*Heavy*","*NewBoy*","*FatFun*","*Pepper*","*Hungry*","*Deep*","*Smell*","*Juice*","*Squirt*"};
//pkunk
static char fury_cap[16][10] = {"Awwky","Tweety","WudStok","Poppy","Brakky","Hooter","Buzzard","Polly","Ernie","Yompin","Fuzzy","Raven","Crow","Jay","Screech","Twitter"};
static char pkunk_insult[14][8] = {"Baby","Dou-Dou","Fool","Idiot","Jerk","Looser","Moron","Nerd","Nitwit","Stupid","Twig","Whimp","Worm","Dummy",};
//shofixti
static char scout_cap[18][10] = {"Hiyata","Wasabe","Kudzu","Ichiban","Bonsai!","Genjiro","Ginzu","Busu","Gaijin","Daikon","Sushi","Naninani","Chimchim","Tora-3","Tofu","Kimba","Tanaka","Katana"};
//slylandro
static char probe_cap[1][10] = {"2418-B"};
//supox
static char blade_cap[16][10] = {"Trifid","Crinoid","FlyTrap","Thistle","Ivy","Sprout","Twig","Root","Branch","Thorn","Bark","Bud","Nut","Stem","Bramble","Thicket"};
//syreen
static char penetrator_cap[16][10] = {"Teela","Dejah","Penny","Alia","Be'lit","Ripley","Yarr","Ardala","Sparta","Munro","Danning","Brawne","Maya","Aelita","Alura","Dale"};
//thraddash
static char torch_cap[16][10] = {"Dthunk","Bardat","Znonk","Mnump","Bronk","Smup","Grulk","Hornk","Knarg","Drulg","Dgako","Znork","Kwamp","Fkank","Pdump","Whumps"};
//umgah
static char drone_cap[16][10] = {"Julg'ka","Gibj'o","Baguk'i","O'guk'e","Gwap'he","Chez'ef","Znork'i","Bob","Kwik'ow","Ei'Ei'o","Brewz'k","Pruk'u","O'bargy","Kterbi'a","Chup'he","I'buba"};
//urquan
static char dreadnought_cap[16][10] = {"Lord 999","Lord 342","Lord 88","Lord 156","Lord 43","Lord 412","Lord 666","Lord 18","Lord 237","Lord 89","Lord 3","Lord 476","Lord 103","Lord 783","Lord 52","Lord 21"};
//utwig
static char jugger_cap[16][10] = {"Endo","Vermi","Manny","Uuter","Nergo","Sami","Duna","Frann","Krisk","Lololo","Snoon","Nestor","Lurg","Thory","Jujuby","Erog"};
//vux
static char intruder_cap[16][4] = {"ZIK","PUZ","ZUK","VIP","ZIT","YUK","DAK","ZRN","PIF","FIZ","FUP","ZUP","NRF","ZOG","ORZ","ZEK"};
//yehat
static char terminator_cap[16][10] = {"Heep-eep","Feep-eep","Reep-eep","Yeep-eep","Beep-eep","Eeep-eep","Meep-eep","Teep-eep","Jeep-eep","Leep-eep","Peep-eep","Weep-eep","Veep-eep","Geep-eep","Zeep-eep","Neep-eep"};
//zoqfotpik
static char stinger_cap[16][8] = {"NikNak","FipPat","DipPak","FatPot","ZikFat","PukYor","TopNik","PorKoo","TikTak","RinTin","FitFap","TotToe","ZipZak","TikTok","MikMok","SikSok"};

static int races[26] = {RESOURCE_ID_ELUDER, RESOURCE_ID_GUARDIAN, RESOURCE_ID_SKIFF, RESOURCE_ID_BROODHOME, RESOURCE_ID_AVATAR, RESOURCE_ID_MAULER,
                        RESOURCE_ID_CRUISER, RESOURCE_ID_AVENGER, RESOURCE_ID_MARAUDER, RESOURCE_ID_TRADER, RESOURCE_ID_XFORM, RESOURCE_ID_PODSHIP, 
                        RESOURCE_ID_NEMESIS, RESOURCE_ID_FURY, RESOURCE_ID_SCOUT, RESOURCE_ID_PROBE, RESOURCE_ID_BLADE, RESOURCE_ID_PENETRATOR,
                        RESOURCE_ID_TORCH, RESOURCE_ID_DRONE, RESOURCE_ID_DREADNOUGHT, RESOURCE_ID_JUGGER, RESOURCE_ID_INTRUDER,
                        RESOURCE_ID_TERMINATOR, RESOURCE_ID_STINGER, RESOURCE_ID_YWING};

int random_race_int;
int ship_int;
int current_insult = 0;
Layer *window_layer;
GRect bounds;

//helper - put int in buffer and log
static void log_int(int num) {
  static char s_buffer[10];
  snprintf(s_buffer, 10, "%i", num);
  APP_LOG(APP_LOG_LEVEL_INFO, s_buffer);
}

//PKUNK set string
static void update_insult(char *insult) {
  static char s_buffer[8];
  strcpy(s_buffer, insult);
  text_layer_set_text(s_insult_layer, s_buffer);
}

//Update captain name
static void update_captain(char *captain) {
  static char s_buffer[10];
  strcpy(s_buffer, captain);
  text_layer_set_text(s_cap_layer, s_buffer);
}

//Randomize the race
static void set_race() {
  if (settings.ship_select == 0) {
    if ((random_race_int <= 0)||(random_race_int > 26)) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Randomizing race");
      random_race_int = rand() % 25 +1;
    }
  } else {
    random_race_int = settings.ship_select;
  }
  //settings.last_race = random_race_int;
}

//Randomly select the captains name
static void set_captain() {
  //APP_LOG(APP_LOG_LEVEL_INFO, "set_captain");
  char *current_cap;
  set_race();
  int def_rand = rand() % 16;
  switch (random_race_int) {
    case SPATHI:
      current_cap = eluder_cap[rand() % 17];
      break;
    case ANDROSYNTH:
      current_cap = guardian_cap[def_rand];
      break;
    case ARILOU:
      current_cap = skiff_cap[def_rand];
      break;
    case CHENJESU:
      current_cap = broodhome_cap[def_rand];
      break;
    case CHMMR:
      current_cap = avatar_cap[def_rand];
      break;
    case DRUUGE:
      current_cap = mauler_cap[def_rand];
      break;
    case HUMAN:
      current_cap = cruiser_cap[def_rand];
      break;
    case ILWRATH:
      current_cap = avenger_cap[def_rand];
      break;
    case KOHRAH:
      current_cap = marauder_cap[def_rand];
      break;
    case MELNORME:
      current_cap = trader_cap[def_rand];
      break;
    case MMRNHRM:
      current_cap = xform_cap[def_rand];
      break;
    case MYCON:
      current_cap = podship_cap[def_rand];
      break;
    case ORZ:
      current_cap = nemesis_cap[def_rand];
      break;
    case PKUNK:
      current_cap = fury_cap[def_rand];
      break;
    case SHOFIXTI:
      current_cap = scout_cap[def_rand];
      break;
    case SLYLANDRO:
      current_cap = probe_cap[0];
      break;
    case SUPOX:
      current_cap = blade_cap[def_rand];
      break;
    case SYREEN:
      current_cap = penetrator_cap[def_rand];
      break;
    case THRADDASH:
      current_cap = torch_cap[def_rand];
      break;
    case UMGAH:
      current_cap = drone_cap[def_rand];
      break;
    case URQUAN:
      current_cap = dreadnought_cap[def_rand];
      break;
    case UTWIG:
      current_cap = jugger_cap[def_rand];
      break;
    case VUX:
      current_cap = intruder_cap[def_rand];
      break;
    case YEHAT:
      current_cap = terminator_cap[def_rand];
      break;
    case ZOQFOTPIK:
      current_cap = stinger_cap[def_rand];
      break;
    default:
      current_cap = "daktak";
  }
  update_captain(current_cap);
  
}

//ORZ Nemesis Turret rotation
static void rotate_turret(struct tm *tick_time, int min) {
  if (random_race_int == ORZ) {
    APP_LOG(APP_LOG_LEVEL_INFO, "rotate turret");
    log_int(min);
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
static void rotate(struct tm *tick_time, int min) {
  APP_LOG(APP_LOG_LEVEL_INFO, "rotate");
  log_int(min);
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
static void set_ship(){
  APP_LOG(APP_LOG_LEVEL_INFO, "set_ship");
  int old_race = random_race_int;
  int old_ship = ship_int;
  set_race();
  ship_int = random_race_int;
  // chance mmrnhrm is ywing
  if (random_race_int == MMRNHRM) {
    if (rand() %3 == 1) {
      APP_LOG(APP_LOG_LEVEL_INFO, "YWing");
      ship_int = YWING;
    }
  }
  
  if (ship_int != old_ship) {
    layer_remove_from_parent((Layer*)rott);
    layer_remove_from_parent((Layer*)rot);
    ship_image = gbitmap_create_with_resource(races[ship_int-1]);
    rot = rot_bitmap_layer_create(ship_image);
    GRect rbounds = layer_get_bounds((Layer*)rot);
    const GPoint center = grect_center_point(&bounds);
    GRect image_frame = (GRect) { .origin = center, .size = bounds.size };
    image_frame.origin.x -= rbounds.size.w/2; 
    image_frame.origin.y -= rbounds.size.h/2; 
    layer_set_frame((Layer*)rot,image_frame);
    rot_bitmap_set_compositing_mode(rot, GCompOpSet);
    //set initial angle,
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp); 
    rotate(tick_time, settings.ship_rotate);
    layer_add_child(window_layer, (Layer*)rot);
    if (ship_int == ORZ) {
      //set initial angle,
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp); 
      rotate_turret(tick_time,settings.turret_rotate);
      layer_add_child(window_layer, (Layer*)rott);
    }
  }
  
  if (old_race != random_race_int) {
    set_captain();
  }
}

//initiate the changes
static void change(int min) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "change");
  if (settings.ship_change == min) {
    random_race_int = 0;
    set_ship();
  } else if ((random_race_int == MMRNHRM)&&(min=-1)) {
    //give a chace for the xform to switch to ywing and vica versa
    if (rand() % 20 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Mmrnhrm chance to change");
      set_ship();
    }
  } else if ((random_race_int == MMRNHRM)&&(min==1)&&
             ((settings.ship_rotate!=1)||((settings.turret_rotate!=1)&&(random_race_int=ORZ)))) {
    //give a chace for the xform to switch to ywing and vica versa
    //higher chance
    if (rand() % 2 == 1) {
      //APP_LOG(APP_LOG_LEVEL_INFO, "Mmrnhrm chance to change");
      set_ship();
    }
  }
  if (settings.cap_change == min) {
    set_captain();
  }
  
  if (current_insult > 0) {
    current_insult = -1;
  } else if (current_insult < 0) {
    update_insult("");
    current_insult = 0;
  }
  
  if ((settings.ship_rotate == 1)&&(random_race_int == PKUNK)&&(current_insult==0)) {
    if (rand()%30==1) {
      APP_LOG(APP_LOG_LEVEL_INFO, "Pkunk Insult triggered");
      current_insult = rand() % 14;
      update_insult(pkunk_insult[current_insult]);
    }
  }
}

//Time layer
static void update_time() {
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

//trigger timing updates
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  rotate(tick_time,1);
  rotate_turret(tick_time,1);
  change(-1);
  
  if(tick_time->tm_sec == 0){ // Every minute
    update_time();
    change(1);
    rotate(tick_time,60);
    rotate_turret(tick_time,60);
    if(tick_time->tm_min%5 == 0){
      change(5);
      if(tick_time->tm_min%10 == 0){ // Every ten minutes
        rotate(tick_time,12);
        rotate_turret(tick_time,12);
        change(10);
        if(tick_time->tm_min == 0){ //Every hour
          change(60);
        }
      }
    }
  }
}

//deint
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

//destroy
static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(ship_image);
  rot_bitmap_layer_destroy(rot);
  text_layer_destroy(s_cap_layer);
  text_layer_destroy(s_insult_layer);
  gbitmap_destroy(turret_image);
  rot_bitmap_layer_destroy(rott);
}

//start
static void main_window_load(Window *window) {
  // Get information about the Window
  window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);

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
  
  //Nemesis Turret
  turret_image = gbitmap_create_with_resource(RESOURCE_ID_NEMESIS_TURRET);
  rott = rot_bitmap_layer_create(turret_image);
  GRect rbounds = layer_get_bounds((Layer*)rott);
  const GPoint center = grect_center_point(&bounds);
  GRect image_frame = (GRect) { .origin = center, .size = bounds.size };
  image_frame.origin.x -= rbounds.size.w/2; 
  image_frame.origin.y -= rbounds.size.h/2; 
  layer_set_frame((Layer*)rott,image_frame);
  rot_bitmap_set_compositing_mode(rott, GCompOpSet);
  
  //SHIP
  set_ship();
}
// Initialize the default settings
static void prv_default_settings() {
  settings.ship_rotate = 60;
  settings.ship_change = 60;
  settings.cap_change = 5;
  settings.ship_select = 0;
  settings.turret_rotate = 1;
  //settings.last_ship = 0;
  //settings.last_race = 0;
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
  //ship_int = settings.last_ship;
  //random_race_int = settings.last_race;
}

static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void set_ticker() {
  if ((settings.ship_rotate == 1)||(settings.turret_rotate == 1)) {
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  };
}
//inbox
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) { 
  Tuple *ship_select_t = dict_find(iter, MESSAGE_KEY_ShipSelection);
  Tuple *ship_change_t = dict_find(iter, MESSAGE_KEY_ShipChange);
  Tuple *ship_rotate_t = dict_find(iter, MESSAGE_KEY_ShipRotate);
  Tuple *cap_change_t = dict_find(iter, MESSAGE_KEY_CapChange);
  if (ship_select_t) {
    int old_ship = settings.ship_select;
    settings.ship_select = atoi(ship_select_t->value->cstring);
    log_int(settings.ship_select);
    if (old_ship != settings.ship_select) {
      set_ship();
    }
  }
  if (ship_rotate_t) {
    int old_rotate  = settings.ship_rotate;
    settings.ship_rotate = atoi(ship_rotate_t->value->cstring);
    log_int(settings.ship_rotate);
    
    //redraw ship rotation
    if (old_rotate != settings.ship_rotate) {
      time_t temp = time(NULL);
      struct tm *tick_time = localtime(&temp); 
      rotate(tick_time, settings.ship_rotate);
    }
    
    //reset up timer service
    if ((old_rotate==1)&& ((settings.ship_rotate == 1)||(settings.turret_rotate == 1))) {
      tick_timer_service_unsubscribe();
      set_ticker();
    } else if ((old_rotate!=1)&& ((settings.ship_rotate == 1)||(settings.turret_rotate == 1))) {
      tick_timer_service_unsubscribe();
      set_ticker();
    }
  }
  if (ship_change_t) {
    settings.ship_change = atoi(ship_change_t->value->cstring);
    log_int(settings.ship_change);
  }
  if (cap_change_t) {
    settings.cap_change = atoi(cap_change_t->value->cstring);
    log_int(settings.cap_change);
  }
  prv_save_settings();
}

static void init() {
  prv_load_settings();
  // Create main Window element and assign to pointer
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  // Register with TickTimerService
  set_ticker();
  // Make sure the time is displayed from the start
  update_time();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}