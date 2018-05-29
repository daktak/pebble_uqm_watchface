module.exports = [
{
  "type": "heading",
  "defaultValue": "UQM Configuration"
},
{
  "type": "section",
  "items": [
  {
    "type": "heading",
    "defaultValue": "Watch Settings"
  },
  {
    "type": "toggle",
    "messageKey": "Animations",
    "label": "Animation"
    "defaultValue": true
  },
  {
    "type": "toggle",
    "messageKey": "Hd2x",
    "label": "UQM HD Ships",
    "defaultValue": true
  },
  {
    "type": "select",
    "messageKey": "ShipSelection",
    "label": "Ship selection",
    "defaultValue" : "0",
    "options": [
      { 
        "label": "Random", 
        "value": "0"
      },
      { 
        "label": "Spathi", 
         "value": "1"
      },
        { 
        "label": "Androsynth", 
         "value": "2"
      },
      { 
        "label": "Arilou", 
         "value": "3"
      },
      { 
        "label": "Chenjesu", 
         "value": "4"
      },
      { 
        "label": "CHMMR", 
         "value": "5"
      },
      { 
        "label": "Druuge", 
         "value": "6"
      },
      { 
        "label": "Human", 
         "value": "7"
      },
      { 
        "label": "Ilwrath", 
         "value": "8"
      },
      { 
        "label": "Kohrah", 
         "value": "9"
      },
      { 
        "label": "Melnorme", 
         "value": "10"
      },
      { 
        "label": "Mmrnhrm", 
         "value": "11"
      },
      { 
        "label": "Mycon", 
         "value": "12"
      },
      { 
        "label": "Orz", 
         "value": "13"
      },
      { 
        "label": "Pkunk", 
         "value": "14"
      },
      { 
        "label": "Shofixti", 
         "value": "15"
      },
     { 
        "label": "Slylandro", 
         "value": "16"
      },
      { 
        "label": "Supox", 
         "value": "17"
      },
      { 
        "label": "Syreen", 
         "value": "18"
      },
      { 
        "label": "Thraddash", 
         "value": "19"
      },
      { 
        "label": "Umgah", 
         "value": "20"
      },
      { 
        "label": "Ur-Quan", 
         "value": "21"
      },
      { 
        "label": "Utwig", 
         "value": "22"
      },
      { 
        "label": "Vux", 
         "value": "23"
      },
      { 
        "label": "Yehat", 
         "value": "24"
      },
      { 
        "label": "Zoqfotpik", 
         "value": "25"
      }
    ]
  },
  {
    "type": "select",
    "messageKey": "ShipChange",
    "label": "Change ship every",
    "defaultValue" : "60",
    "options": [
      { 
        "label": "Never", 
        "value": "0"
      },
      { 
        "label": "Minute", 
        "value": "1"
      },
      { 
        "label": "5 Minutes", 
        "value": "5"
      },
      { 
        "label": "10 Minutes", 
        "value": "10"
      },
      { 
        "label": "60 Minutes", 
        "value": "60"
      }
    ]
  },
  {
    "type": "radiogroup",
    "messageKey": "ShipRotate",
    "label": "Rotate ship every",
    "defaultValue" : "60",
    "options": [
      {
        "label": "Never",
        "value": "0"
      },
      { 
        "label": "Minute", 
        "value": "1"
      },
      { 
        "label": "60 Minutes", 
        "value": "60"
      },
      { 
        "label": "12 Hours", 
        "value": "12"
      }
    ]
  },
  {
    "type": "radiogroup",
    "messageKey": "TurretRotate",
    "label": "Rotate turret every",
    "defaultValue" : "60",
    "options": [
      {
        "label": "Never",
        "value": "0"
      },
      { 
        "label": "Minute", 
        "value": "1"
      },
      { 
        "label": "60 Minutes", 
        "value": "60"
      },
      { 
        "label": "12 Hours", 
        "value": "12"
      }
    ]
  },
  {
    "type": "select",
    "messageKey": "CapChange",
    "label": "Change captain every",
    "defaultValue" : "5",
    "options": [
      { 
        "label": "Never", 
        "value": "0"
      },
      { 
        "label": "Minute", 
        "value": "1"
      },
      { 
        "label": "5 Minutes", 
        "value": "5"
      },
      { 
        "label": "10 Minutes", 
        "value": "10"
      },
      { 
        "label": "60 Minutes", 
        "value": "60"
      }
    ]
  }
  ]
},
{
  "type": "submit",
  "defaultValue": "Save Settings"
}
];
