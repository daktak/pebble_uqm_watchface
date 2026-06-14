#include <pebble.h>
#include "src/c/races.h"
#include "src/c/captain.h"

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

static int last_cap_index;

int get_last_cap_index() {
  return last_cap_index;
}

char* get_captain_by_index(int race, int index) {
  char *current_cap;
  switch (race) {
    case SPATHI:    current_cap = eluder_cap[index % 17]; break;
    case ANDROSYNTH: current_cap = guardian_cap[index % 16]; break;
    case ARILOU:    current_cap = skiff_cap[index % 16]; break;
    case CHENJESU:  current_cap = broodhome_cap[index % 16]; break;
    case CHMMR:     current_cap = avatar_cap[index % 16]; break;
    case DRUUGE:    current_cap = mauler_cap[index % 16]; break;
    case HUMAN:     current_cap = cruiser_cap[index % 16]; break;
    case ILWRATH:   current_cap = avenger_cap[index % 16]; break;
    case KOHRAH:    current_cap = marauder_cap[index % 16]; break;
    case MELNORME:  current_cap = trader_cap[index % 16]; break;
    case MMRNHRM:   current_cap = xform_cap[index % 16]; break;
    case MYCON:     current_cap = podship_cap[index % 16]; break;
    case ORZ:       current_cap = nemesis_cap[index % 16]; break;
    case PKUNK:     current_cap = fury_cap[index % 16]; break;
    case SHOFIXTI:  current_cap = scout_cap[index % 18]; break;
    case SLYLANDRO: current_cap = probe_cap[0]; break;
    case SUPOX:     current_cap = blade_cap[index % 16]; break;
    case SYREEN:    current_cap = penetrator_cap[index % 16]; break;
    case THRADDASH: current_cap = torch_cap[index % 16]; break;
    case UMGAH:     current_cap = drone_cap[index % 16]; break;
    case URQUAN:    current_cap = dreadnought_cap[index % 16]; break;
    case UTWIG:     current_cap = jugger_cap[index % 16]; break;
    case VUX:       current_cap = intruder_cap[index % 16]; break;
    case YEHAT:     current_cap = terminator_cap[index % 16]; break;
    case ZOQFOTPIK: current_cap = stinger_cap[index % 16]; break;
    default:        current_cap = "daktak";
  }
  last_cap_index = index;
  return current_cap;
}

//Randomly select the captains name
char* get_captain(int race) {
  char *current_cap;
  int def_rand = rand() % 16;
  int idx;
  switch (race) {
    case SPATHI:
      idx = rand() % 17;
      current_cap = eluder_cap[idx];
      break;
    case ANDROSYNTH:
      idx = def_rand;
      current_cap = guardian_cap[idx];
      break;
    case ARILOU:
      idx = def_rand;
      current_cap = skiff_cap[idx];
      break;
    case CHENJESU:
      idx = def_rand;
      current_cap = broodhome_cap[idx];
      break;
    case CHMMR:
      idx = def_rand;
      current_cap = avatar_cap[idx];
      break;
    case DRUUGE:
      idx = def_rand;
      current_cap = mauler_cap[idx];
      break;
    case HUMAN:
      idx = def_rand;
      current_cap = cruiser_cap[idx];
      break;
    case ILWRATH:
      idx = def_rand;
      current_cap = avenger_cap[idx];
      break;
    case KOHRAH:
      idx = def_rand;
      current_cap = marauder_cap[idx];
      break;
    case MELNORME:
      idx = def_rand;
      current_cap = trader_cap[idx];
      break;
    case MMRNHRM:
      idx = def_rand;
      current_cap = xform_cap[idx];
      break;
    case MYCON:
      idx = def_rand;
      current_cap = podship_cap[idx];
      break;
    case ORZ:
      idx = def_rand;
      current_cap = nemesis_cap[idx];
      break;
    case PKUNK:
      idx = def_rand;
      current_cap = fury_cap[idx];
      break;
    case SHOFIXTI:
      idx = rand() % 18;
      current_cap = scout_cap[idx];
      break;
    case SLYLANDRO:
      idx = 0;
      current_cap = probe_cap[0];
      break;
    case SUPOX:
      idx = def_rand;
      current_cap = blade_cap[idx];
      break;
    case SYREEN:
      idx = def_rand;
      current_cap = penetrator_cap[idx];
      break;
    case THRADDASH:
      idx = def_rand;
      current_cap = torch_cap[idx];
      break;
    case UMGAH:
      idx = def_rand;
      current_cap = drone_cap[idx];
      break;
    case URQUAN:
      idx = def_rand;
      current_cap = dreadnought_cap[idx];
      break;
    case UTWIG:
      idx = def_rand;
      current_cap = jugger_cap[idx];
      break;
    case VUX:
      idx = def_rand;
      current_cap = intruder_cap[idx];
      break;
    case YEHAT:
      idx = def_rand;
      current_cap = terminator_cap[idx];
      break;
    case ZOQFOTPIK:
      idx = def_rand;
      current_cap = stinger_cap[idx];
      break;
    default:
      idx = 0;
      current_cap = "daktak";
  }
  last_cap_index = idx;
  return current_cap;
}

char* get_insult() {
      int current_insult = rand() % 14;
      return pkunk_insult[current_insult];
}
