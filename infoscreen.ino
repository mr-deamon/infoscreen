//Libs for Display
#include <GxFont_GFX.h>
#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
const GFXfont* f18pt = &FreeSans18pt7b;
const GFXfont* f12pt = &FreeSans12pt7b;
const GFXfont* f9pt = &FreeSans9pt7b;
GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ /*BUSY=D2*/2, -1); // default selection of D4(=2), D2(=4)

//Libs for RTC
#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

//Libs for Wifi
#define USING_AXTLS
#include <ESP8266WiFi.h>
//#include <WiFiClientSecure.h>
#include "WiFiClientSecureAxTLS.h"
using namespace axTLS;
WiFiClientSecure client;
#include <sys/time.h>
#include <time.h>


//Libs for anything else
#include <ArduinoJson.h>


int8_t rain;
int8_t temp;
String nextBus[2];
String paper;


#include "settings.h"
#include "functions.h";
#include "images.h"
#include "screen_functions.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  display.powerDown();
  display.init();
  delay(100);
  Rtc.Begin();
  //display.setRotation(0);

  // See if alarm was triggered
  DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags();

  initClock();
  RtcDateTime timestamp = Rtc.GetDateTime();

  ///// default alarm, to be overwritten
  DS3231AlarmOne alarm1(
    0,
    0,
    0,
    45,
    DS3231AlarmOneControl_SecondsMatch);
  DS3231AlarmTwo alarm2(
    0,
    22,
    22,
    DS3231AlarmTwoControl_HoursMinutesMatch);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
  Rtc.SetAlarmOne(alarm1);
  Rtc.SetAlarmTwo(alarm2);


  if ((timestamp.Hour() == 5 && timestamp.Minute() == 33) || !flag) {
    /*if (timestamp.Day() % 11 == 0) {
      configTime(0, 0, "0.ch.pool.ntp.org");
      setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 3);
      while (time(nullptr) <= 100000) {
        Serial.print(".");
        delay(100);
      }
      time_t tnow = time(nullptr);
      Serial.print(String(ctime(&tnow)));
    }*/
    Serial.println("Start of the day");
    display.fillScreen(GxEPD_WHITE);
    display.update();
    //start of the day
    drawSymbols();
    wifiConnect();
    //Serial.println("Update NTP");
    Serial.println("Update Weather");
    retrieveWeather();
    if (rain > 0.3) {
      drawData(posrain, String("yes"), f12pt, false);
    } else {
      drawData(posrain, String(" no"), f12pt, false);
    }
    drawData(postemp, String(temp), f12pt, false);
    Serial.println("Update Abfall");
    checkAbfall(timestamp.Month(), timestamp.Day());
    drawData(pospaper, paper, f12pt, false);
    Serial.println("Update Bus");
    retrieveBus();
    WiFiOff();
    drawData(posbus1, String(nextBus[0]), f9pt, false);
    drawData(posbus2, String(nextBus[1]), f9pt, false);
    Serial.println("Update Time");
    updateTime(false);
    display.updateWindow(0, 0, 200, 200);
    
    ESP.deepSleepInstant(0, WAKE_RF_DISABLED);
  } else if (timestamp.Hour() == 22 && timestamp.Minute() == 22) {
    //normal nightmode

    DS3231AlarmTwo alarm2(
      0,
      5,
      33,
      DS3231AlarmTwoControl_HoursMinutesMatch);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmTwo);
    Rtc.SetAlarmTwo(alarm2);
    Serial.println("Sleep until 0530, set Image");
    display.eraseDisplay();
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 40, gImage_cat, 200, 120, GxEPD_BLACK);
    display.update();

    ESP.deepSleepInstant(0, WAKE_RF_DISABLED);
  }

  if (timestamp.Hour() > 21 || timestamp.Hour() < 5) {
    //unexpected nightmode

    DS3231AlarmOne alarm1(
      0,
      5,
      33,
      0,
      DS3231AlarmOneControl_HoursMinutesSecondsMatch);
    Rtc.SetAlarmOne(alarm1);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne);
    Serial.println("Should never happen, except reboot");
    display.eraseDisplay();
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 40, gImage_cat, 200, 120, GxEPD_BLACK);
    display.update();

    ESP.deepSleepInstant(0, WAKE_RF_DISABLED);
  }
  else {
    //daymode, do everything else before!
    Serial.println("Okay, daymode!");
    DS3231AlarmOne alarm1(
      0,
      0,
      0,
      45,
      DS3231AlarmOneControl_SecondsMatch);
    DS3231AlarmTwo alarm2(
      0,
      22,
      22,
      DS3231AlarmTwoControl_HoursMinutesMatch);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth);
    Rtc.SetAlarmOne(alarm1);
    Rtc.SetAlarmTwo(alarm2);
    if (!flag) {
      ESP.deepSleepInstant(0, WAKE_RF_DISABLED);
      Serial.println("no flag,sleeping");
    } else {
      Serial.println("Update Time on Watch");
      updateTime();
      if (timestamp.Minute() % 5 == 0) {
        Serial.println("Update Bus");
        wifiConnect();
        retrieveBus();
        WiFiOff();
        drawData(posbus1, String(nextBus[0]), f9pt);
        drawData(posbus2, String(nextBus[1]), f9pt);
      }

      ESP.deepSleepInstant(0, WAKE_RF_DISABLED);
    }

  }

display.powerDown();

}
void loop() {}
