#include <pebble.h>

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

#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  int ship_select;
  int ship_change;
  int ship_rotate;
  int cap_change;
  int last_race;
  char* last_cap;
} ClaySettings;
static ClaySettings settings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_cap_layer;
static GBitmap *ship_image;
static RotBitmapLayer *rot;
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

Layer *window_layer;
GRect bounds;
int race_int;

static void set_race() {
  if ((race_int <= 0)||(race_int > 26)) {
    race_int = rand() % 25 +1;
    settings.ship_select = race_int;
  } else {
    race_int = settings.ship_select;
  }
}

static void update_captain(char *captain) {
  static char s_buffer[10];
  strcpy(s_buffer, captain);
  text_layer_set_text(s_cap_layer, s_buffer);
}

static void set_captain() {
  char *current_cap;
  set_race();
  int def_rand = rand() % 16;

  switch (race_int) {
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
      current_cap = "Daktak";
  }
  update_captain(current_cap);
  settings.last_cap = current_cap;
}

static void rotate(struct tm *tick_time, int min) {
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

static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void set_ship(){
  int old_race = race_int;
  set_race();
  
  if (race_int != old_race) {
    int ship_int = race_int;
    // 50 / 50 chance mmrnhrm is ywing
    if (ship_int == 11) {
      if (rand() %2 == 1) {
        ship_int = 26;
      }
    }
    layer_remove_from_parent((Layer*)rot);
    ship_image = gbitmap_create_with_resource(races[ship_int-1]);
    rot = rot_bitmap_layer_create(ship_image);
    GRect rbounds = layer_get_bounds((Layer*)rot);
    const GPoint center = grect_center_point(&bounds);
    GRect image_frame = (GRect) { .origin = center, .size = bounds.size };
    image_frame.origin.x -= rbounds.size.w/2; //rotlayer does something odd with positioning, and this is the only way I could correct it
    image_frame.origin.y -= rbounds.size.h/2; //-17
    layer_set_frame((Layer*)rot,image_frame);
    rot_bitmap_set_compositing_mode(rot, GCompOpSet);
    //set initial angle,
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    rotate(tick_time, settings.ship_rotate);
    layer_add_child(window_layer, (Layer*)rot);
    
    set_captain();
    prv_save_settings();
  }
}

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

static void change(int min) {
  if (settings.ship_change == min) {
    set_ship();
  }
  if (settings.cap_change == min) {
    set_captain();
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  rotate(tick_time,1);
  
  if(tick_time->tm_sec == 0){ // Every minute
    update_time();
    rotate(tick_time,60);
    change(1);
    if(tick_time->tm_min%5 == 0){
      change(5);
      if(tick_time->tm_min%10 == 0){ // Every ten minutes
        rotate(tick_time, 60); 
        if(tick_time->tm_min == 0){ //Every hour
          change(60);
        }
      }
    }
  }
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(ship_image);
  rot_bitmap_layer_destroy(rot);
  text_layer_destroy(s_cap_layer);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);

  // Time
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(8, 2), bounds.size.w, 50)); //58.52
  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  //Captain Name
  s_cap_layer = text_layer_create(
      GRect(0, bounds.size.h - 32, bounds.size.w, 50));
  text_layer_set_background_color(s_cap_layer, GColorBlack);
  text_layer_set_text_color(s_cap_layer, GColorWhite);
  //set_captain();
  text_layer_set_font(s_cap_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_cap_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_cap_layer));
  
  //SHIP
  set_ship();
}

// Initialize the default settings
static void prv_default_settings() {
  settings.ship_rotate = 60;
  settings.ship_change = 60;
  settings.cap_change = 5;
  settings.ship_select = 0;
  settings.last_race = 1;
  settings.last_cap = "Daktak";
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
  race_int = settings.last_race;
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *ship_select_t = dict_find(iter, MESSAGE_KEY_ship_selection);
  Tuple *ship_change_t = dict_find(iter, MESSAGE_KEY_ship_change);
  Tuple *ship_rotate_t = dict_find(iter, MESSAGE_KEY_ship_rotate);
  Tuple *cap_change_t = dict_find(iter, MESSAGE_KEY_cap_change);
  if (ship_select_t) {
    settings.ship_select = ship_select_t->value->int32;
  }
  if (ship_rotate_t) {
    settings.ship_rotate = ship_rotate_t->value->int32;
  }
  if (ship_change_t) {
    settings.ship_change = ship_change_t->value->int32;
  }
  if (cap_change_t) {
    settings.cap_change = cap_change_t->value->int32;
  }
  prv_save_settings();
}

static void init() {
   prv_load_settings();
  // Create main Window element and assign to pointer
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
  if (settings.ship_rotate == 1) {
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  };
  // Make sure the time is displayed from the start
  update_time();
  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}