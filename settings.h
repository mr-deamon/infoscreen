//#define WIFI_SSID ""
//#define WIFI_PASS ""
//create wifi.h and copy above lines
#include "wifi.h"
#define WIFI_IP IPAddress(192,168,0,7)
#define WIFI_SUBNET IPAddress(255,255,255,0)
#define WIFI_GATEWAY IPAddress(192,168,0,1)
#define WIFI_HOSTNAME "Infoscreen"
#define WIFI_CHANNEL 2
#define WIFI_MAC "abc"

#define BUSAPI "https://transport.opendata.ch/v1/stationboard?id=8590069&limit=2&fields[]=stationboard/passList/departure"
#define WEATHERAPI "http://api.apixu.com/v1/forecast.json?key=79522758eea5461384a170614180411&q=Bern,ch&days=2"

/*const PROGMEM  uint8_t posbus1[]  = { 48, 6, 52, 19};
  const PROGMEM  uint8_t posbus2[]  = { 48, 26, 52, 19};
  const PROGMEM  uint8_t posrain[]  = { 115, 160, 75, 19};
  const PROGMEM  uint8_t postemp[]  = { 136, 15, 75, 19};
  const PROGMEM  uint8_t pospaper[]  = { 42, 167, 72, 19};
*/
const PROGMEM  uint8_t posbus1[]  = { 45,19,55,15};
const PROGMEM  uint8_t posbus2[]  = { 45,35,55,15};
const PROGMEM  uint8_t posrain[]  = { 100,155,58,32};
const PROGMEM  uint8_t postemp[]  = { 135,10,20,32};
const PROGMEM  uint8_t pospaper[]  = { 39,155,60,32};
const PROGMEM  uint8_t poswatch[] = {40,70,120,30};

const PROGMEM uint8_t paperdates[12][3] = {{9, 23, 6}, {6, 20}, {6, 20}, {3, 17}, {1, 15, 29}, {12, 26}, {10, 24}, {7, 21}, {4, 18}, {2, 16, 30}, {13, 27}, {11, 24}};
