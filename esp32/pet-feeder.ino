#include <ESP32Time.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define lcdColumns 16
#define lcdRows 2

ESP32Time rtc(0);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800, 60000);
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

const char* ssid = "Steve Lokao";
const char* password = "winstow0822";

byte last_second, second_, minute_, hour_, day_, month_;
int year_;

char Time[14];
char Date[14];


void setup() {

  Serial.begin(115200);


  Wire.begin();

  lcd.init();
  lcd.backlight();

  printLCDFirstLine(true, 0, "Connecting to ");
  lcd.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    for (int col = 0; col < 3; col++) {
      printLCDSecondLine(false, col, ".");
      delay(500);
    }

  }
  printLCDFirstLine(true, 0, "Wifi connected!");
  printLCDSecondLine(false, 0, "xD");
  delay(1000);

  printLCDFirstLine(true, 0, "Setting up...");
  delay(1000);
  timeClient.begin();
  getTimeWithNTP();
  printLCDFirstLine(true, 0, "Ready to go!");
  delay(1000);

  lcd.clear();

}

void loop() {
  handleDateTime();
}


void printLCDFirstLine(bool clear, int col, const char* text) {
  if(clear) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } 
  lcd.setCursor(col, 0);
  lcd.print(text);
}

void printLCDSecondLine(bool clear, int col, const char* text) {
  if(clear) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } 
  lcd.setCursor(col, 1);
  lcd.print(text);
}

void getTimeWithNTP() {
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();
  rtc.setTime(unix_epoch);
  unsigned long ep = rtc.getEpoch();
}

void convertDateTime() {
  String time_str = rtc.getTime("TIME:%T");
  String date_str = rtc.getTime("DATE:%D");
  strcpy(Time, time_str.c_str());
  strcpy(Date, date_str.c_str());
}

void handleDateTime() {
  convertDateTime();
  printLCDFirstLine(false, 0, Time);
  printLCDSecondLine(false, 0, Date);
  delay(200);
}
