//#define WIFI_SSID ""
//#define WIFI_PASS ""
//create wifi.h and copy above lines
#include "wifi.h"

#define BUSAPI "https://transport.opendata.ch/v1/connections?from=8590069&to=8576646&fields[]=connections/from/departure&limit=2"
#define WEATHERAPI "http://api.apixu.com/v1/forecast.json?key=79522758eea5461384a170614180411&q=Bern,ch&days=2"

const PROGMEM  uint8_t posbus1[]  = { 45,14,55,15};
const PROGMEM  uint8_t posbus2[]  = { 45,30,55,15};
const PROGMEM  uint8_t posrain[]  = { 100,150,58,32};
const PROGMEM  uint8_t postemp[]  = { 135,10,20,32};
const PROGMEM  uint8_t pospaper[]  = { 39,155,60,32};
const PROGMEM  uint8_t poswatch[] = {40,70,120,30};

const PROGMEM uint8_t paperdates[12][3] = {{9, 23, 6}, {6, 20}, {6, 20}, {3, 17}, {1, 15, 29}, {12, 26}, {10, 24}, {7, 21}, {4, 18}, {2, 16, 30}, {13, 27}, {11, 24}};
