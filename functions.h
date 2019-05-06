String one2two(int i) {
  if (i < 10) {
    return String("0" + String(i));
  } else {
    return String(i);
  }
}

void WiFiOff() {
  WiFi.disconnect( true );
  delay( 1 );
  WiFi.mode(WIFI_OFF);
  delay(1);
  WiFi.forceSleepBegin();
  delay(1);

}
void wifiConnect() {
  WiFi.forceSleepWake();
  delay( 1 );
  WiFi.persistent( false );
  WiFi.mode(WIFI_STA);
  delay(10);
  WiFi.config(WIFI_IP, WIFI_GATEWAY, WIFI_GATEWAY, WIFI_SUBNET);
  WiFi.begin(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL, wifi_mac, true);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void retrieveBus() {
  const char* host = "transport.opendata.ch";
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed.");
  }
  Serial.println("Sending GET request to transport.opendata.ch...");
  client.print(String("GET ") + BUSAPI + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received.");
      break;
    }
  }
  String body;
  boolean isBody = false;
  char c;
  int size = 0, index = 0;
  Serial.println("Reading and parsing data from transport.opendata.ch...");
  while (client.connected()) {
    while ((size = client.available()) > 0) {
      c = client.read();
      if (!isBody && (c == '{' || c == '[')) {
        isBody = true;
      }
      if (isBody) {
        body += c;
      }
    }
    Serial.println(" Done.");
    client.stop();
    DynamicJsonBuffer jsonBuffer;
    //DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(body);
    if (root.success()) {
      String bus1 = root["connections"][0]["from"]["departure"];
      String bus2 = root["connections"][1]["from"]["departure"];
      nextBus[0] = bus1.substring(11, 16);
      nextBus[1] = bus2.substring(11, 16);
    }
  }

}
double retrieveWeather() {
  const char* host = "api.apixu.com";
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed.");
    return -1;
  }

  // Send GET request to api.darksky.net
  Serial.println("Sending GET request to api.darksky.net...");
  client.print(String("GET ") + WEATHERAPI + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received.");
      break;
    }
  }
  Serial.println(String(ESP.getFreeHeap()));
  String body;
  boolean isBody = false;
  char c;
  int size = 0, index = 0;
  Serial.print("Reading and parsing data from api.darksky.net...");
  while (client.connected()) {
    while ((size = client.available()) > 0) {
      c = client.read();
      if (!isBody && (c == '{' || c == '[')) {
        isBody = true;
      }
      if (isBody) {
        body += c;
      }
    }
    client.stop();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(body);
    if (root.success()) {
      rain = root["forecast"]["forecastday"][0]["day"]["totalprecip_mm"];
      temp = root["forecast"]["forecastday"][0]["day"]["avgtemp_c"];
    }
  }
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.println(datestring);
}

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))
void checkAbfall(int month, int day) {
  for (int i = 0; i < ARRAY_SIZE(paperdates[month - 1]); i++) {
    if (paperdates[month - 1][i] - 1 == day) {
      paper = "tmrw";
    }
    else if (paperdates[month - 1][i] == day) {
      paper = "tdy";
    }
    else {
      paper = "";
    }
  }
  //if(timestamp.Day)
}

void initClock() {
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    // Common Cuases:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");

    // following line sets the RTC to the date & time this sketch was compiled
    // it will also reset the valid flag internally unless the Rtc device is
    // having an issue

    Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
  printDateTime(now);
  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  Rtc.LatchAlarmsTriggeredFlags();
}
