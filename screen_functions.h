void drawSymbols(bool update=false)
{
  if(update==true) display.fillScreen(GxEPD_WHITE);
  display.drawBitmap(10, 10, gImage_bus, 32, 32, GxEPD_BLACK);
  display.drawBitmap(172, 10, gImage_temp, 16, 32, GxEPD_BLACK);
  display.drawBitmap( 10, 158, gImage_trash, 28, 32, GxEPD_BLACK);
  display.drawBitmap( 158, 160, gImage_umbrella, 36, 32, GxEPD_BLACK);
  display.fillCircle(161, 14, 4, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
  display.fillCircle(161, 14, 2, GxEPD_WHITE);  //Xpos,Ypos,r,Farbe
  if(update==true) display.update();
}
void drawData(const uint8_t *pos, String data, const GFXfont* font=f12pt, bool update=true)
{
  if(update==true) display.fillRect(pos[0], pos[1], pos[2],pos[3], GxEPD_WHITE);
  uint16_t cursor_y = pos[1] + pos[3]-1;
  display.setCursor(pos[0], cursor_y);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(font);
  display.print(data);
  if(update==true) display.updateWindow(pos[0], pos[1], pos[2],pos[3], true);
 
}

void updateTime(bool update=true)
{
  if(update==true) display.fillRect(poswatch[0],poswatch[1],poswatch[2],poswatch[3], GxEPD_WHITE);
  RtcDateTime now = Rtc.GetDateTime();
  //printDateTime(now);
  display.setFont(f18pt);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(60, 97);
  if(now.Hour()<10){display.print("0");}
  display.print(now.Hour(), DEC);
  display.print(":");
  if(now.Minute()<10){display.print("0");}
  display.print(now.Minute(), DEC);
  display.setFont(f12pt);
  display.setCursor(45, 125);
  display.print(one2two(now.Day()));
  display.print(".");
  display.print(one2two(now.Month()));
  display.print(".");
  display.print(now.Year(), DEC);
  if(update==true) display.updateWindow(poswatch[0],poswatch[1],poswatch[2],poswatch[3], true);
}
