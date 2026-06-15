#include <pebble.h>
#include "src/c/races.h"
#include "src/c/captain.h"

static char eluder_cap[17][10]={"Thwil","Pwappy","Phwiff","Wiffy","Plibnik","Snurfel","Kwimp","Pkunky","Jinkeze","Thintho","Rupatup","Nargle","Phlendo","Snelopy","Bwinkin","Whuff","Fwiffo"};
static char guardian_cap[16][10]={"BOOJI-1","DORN-3","BIM-XT","JOR-15","976-KILL","KORB-7B","XR4-TI","CRC-16","BHS-79","DOS-1.0","ME-262","AK-47","1040-EZ","NECRO-99","HAL-2001","SR-71"};
static char skiff_cap[16][10]={"Fefaloo","Bezabu","Tiptushi","Marypup","Tinkafo","Patooti","Tifiwilo","Loleelu","Louifoui","Pinywiny","Oowbabe","Dingdup","Wewalia","Yipyapi","Ropilup","Wolwali"};
static char broodhome_cap[16][10]={"Kzzakk","Tzrrow","Zzmzmm","Vziziz","Hmmhmm","Bzrak","Krrtzz","Zzzzz","Zxzakz","Brrzap","Tzaprak","Pzkrakz","Fzzzz","Vrroww","Zznaz","Zzzhmm"};
static char avatar_cap[16][10]={"Mnzgk","Chzrmn","Bzztrm","Zrnzrk","Tzzqrn","Kzzrn","Vzrzn","Qrntz","Rmnzk","Szmrnz","Zbzzn","Frnkzk","Prmtzz","Tzrtzn","Kztztz","Mrnkzt"};
static char mauler_cap[16][10]={"Tuuga","Siinur","Kaapo","Juugl","Paato","Feezo","Maad","Moola","Kooli","Faazur","Zooto","Biitur","Duulard","Piini","Soopi","Peeru"};
static char cruiser_cap[16][10]={"Decker","Trent","Adama","Spiff","Graeme","Kirk","Pike","Halleck","Tuf","Pirx","Wu","VanRijn","Ender","Buck","Solo","Belt"};
static char avenger_cap[16][10]={"Gorgon","Taragon","Kalgon","Borgo","Dirga","Slygor","Rogash","Argarak","Kayzar","Baylor","Zoggak","Targa","Vogar","Lurgo","Regorjo","Manglor"};
static char marauder_cap[16][10]={"Death 11","Death 17","Death 37","Death 23","Death 7","Death 13","Death 19","Death 29","Death 31","Death 41","Death 43","Death 3","Death 5","Death 47","Death 53","Death 83"};
static char trader_cap[16][10]={"Reddish","Orangy","Aqua","Crimson","Magenta","Cheruse","Beige","Fuchsia","Umber","Cerise","Mauve","Grayish","Yellow","Black","Bluish","Purple"};
static char xform_cap[16][10]={"Qir-nha","Jhe-qir","Qua-rhna","Mn-quah","Nrna-mha","Um-hrh","Hm-nhuh","Rrma-hrn","Jra-nr","Ur-mfrs","Qua-qir","Mrm-na","Jhe-mhr","Hmr-hun","Nhuh-na","Hrnm-hm"};
static char podship_cap[16][10]={"Blort","Chupp","Floos","Glish","Glob","Glush","Plork","Shlish","Shlupp","Slingy","Sploozo","Spork","Uffo","Yush","Zaloop","Znuff"};
static char nemesis_cap[16][10] = {"*Wet*","*Happy*","*Frumple*","*Camper*","*Loner*","*Dancer*","*Singer*","*Heavy*","*NewBoy*","*FatFun*","*Pepper*","*Hungry*","*Deep*","*Smell*","*Juice*","*Squirt*"};
static char fury_cap[16][10] = {"Awwky","Tweety","WudStok","Poppy","Brakky","Hooter","Buzzard","Polly","Ernie","Yompin","Fuzzy","Raven","Crow","Jay","Screech","Twitter"};
static char pkunk_insult[14][8] = {"Baby","Dou-Dou","Fool","Idiot","Jerk","Looser","Moron","Nerd","Nitwit","Stupid","Twig","Whimp","Worm","Dummy",};
static char scout_cap[18][10] = {"Hiyata","Wasabe","Kudzu","Ichiban","Bonsai!","Genjiro","Ginzu","Busu","Gaijin","Daikon","Sushi","Naninani","Chimchim","Tora-3","Tofu","Kimba","Tanaka","Katana"};
static char probe_cap[1][10] = {"2418-B"};
static char blade_cap[16][10] = {"Trifid","Crinoid","FlyTrap","Thistle","Ivy","Sprout","Twig","Root","Branch","Thorn","Bark","Bud","Nut","Stem","Bramble","Thicket"};
static char penetrator_cap[16][10] = {"Teela","Dejah","Penny","Alia","Be'lit","Ripley","Yarr","Ardala","Sparta","Munro","Danning","Brawne","Maya","Aelita","Alura","Dale"};
static char torch_cap[16][10] = {"Dthunk","Bardat","Znonk","Mnump","Bronk","Smup","Grulk","Hornk","Knarg","Drulg","Dgako","Znork","Kwamp","Fkank","Pdump","Whumps"};
static char drone_cap[16][10] = {"Julg'ka","Gibj'o","Baguk'i","O'guk'e","Gwap'he","Chez'ef","Znork'i","Bob","Kwik'ow","Ei'Ei'o","Brewz'k","Pruk'u","O'bargy","Kterbi'a","Chup'he","I'buba"};
static char dreadnought_cap[16][10] = {"Lord 999","Lord 342","Lord 88","Lord 156","Lord 43","Lord 412","Lord 666","Lord 18","Lord 237","Lord 89","Lord 3","Lord 476","Lord 103","Lord 783","Lord 52","Lord 21"};
static char jugger_cap[16][10] = {"Endo","Vermi","Manny","Uuter","Nergo","Sami","Duna","Frann","Krisk","Lololo","Snoon","Nestor","Lurg","Thory","Jujuby","Erog"};
static char intruder_cap[16][10] = {"ZIK","PUZ","ZUK","VIP","ZIT","YUK","DAK","ZRN","PIF","FIZ","FUP","ZUP","NRF","ZOG","ORZ","ZEK"};
static char terminator_cap[16][10] = {"Heep-eep","Feep-eep","Reep-eep","Yeep-eep","Beep-eep","Eeep-eep","Meep-eep","Teep-eep","Jeep-eep","Leep-eep","Peep-eep","Weep-eep","Veep-eep","Geep-eep","Zeep-eep","Neep-eep"};
static char stinger_cap[16][10] = {"NikNak","FipPat","DipPak","FatPot","ZikFat","PukYor","TopNik","PorKoo","TikTak","RinTin","FitFap","TotToe","ZipZak","TikTok","MikMok","SikSok"};

static int last_cap_index;

typedef struct {
  char (*names)[10];
  int count;
} CaptainData;

static CaptainData captains[YWING + 1] = {
  [SPATHI]     = { eluder_cap, 17 },
  [ANDROSYNTH] = { guardian_cap, 16 },
  [ARILOU]     = { skiff_cap, 16 },
  [CHENJESU]   = { broodhome_cap, 16 },
  [CHMMR]      = { avatar_cap, 16 },
  [DRUUGE]     = { mauler_cap, 16 },
  [HUMAN]      = { cruiser_cap, 16 },
  [ILWRATH]    = { avenger_cap, 16 },
  [KOHRAH]     = { marauder_cap, 16 },
  [MELNORME]   = { trader_cap, 16 },
  [MMRNHRM]    = { xform_cap, 16 },
  [MYCON]      = { podship_cap, 16 },
  [ORZ]        = { nemesis_cap, 16 },
  [PKUNK]      = { fury_cap, 16 },
  [SHOFIXTI]   = { scout_cap, 18 },
  [SLYLANDRO]  = { probe_cap, 1 },
  [SUPOX]      = { blade_cap, 16 },
  [SYREEN]     = { penetrator_cap, 16 },
  [THRADDASH]  = { torch_cap, 16 },
  [UMGAH]      = { drone_cap, 16 },
  [URQUAN]     = { dreadnought_cap, 16 },
  [UTWIG]      = { jugger_cap, 16 },
  [VUX]        = { intruder_cap, 16 },
  [YEHAT]      = { terminator_cap, 16 },
  [ZOQFOTPIK]  = { stinger_cap, 16 },
};

int get_last_cap_index() {
  return last_cap_index;
}

char* get_captain_by_index(int race, int index) {
  if (race < 1 || race > ZOQFOTPIK) return "daktak";
  CaptainData *d = &captains[race];
  last_cap_index = index % d->count;
  return d->names[last_cap_index];
}

char* get_captain(int race) {
  if (race < 1 || race > ZOQFOTPIK) return "daktak";
  CaptainData *d = &captains[race];
  int idx = (d->count == 1) ? 0 : rand() % d->count;
  last_cap_index = idx;
  return d->names[idx];
}

char* get_insult() {
  int n = rand() % 14;
  return pkunk_insult[n];
}
