/*
* Created By Balaji for Rajan Robotics
* Rajan Robotics
*/

#include <Wire.h>
#include <RTClib.h>
#include <TM1637Display.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- PIN CONFIGURATION ----------------

#define CLK 6
#define DIO 7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ---------------- OBJECTS ----------------

RTC_DS3231 rtc;

TM1637Display segment(CLK, DIO);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(9600);
  Wire.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

    // Set RTC time manually
  rtc.adjust(DateTime(2026, 11, 11, 11, 11, 00));

  // Set RTC only if battery backup was lost
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("RTC time updated");
  }

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Initialize TM1637
  segment.setBrightness(7);
  segment.clear();

  Serial.println("Digital Clock Started");
}

// ---------------- MAIN LOOP ----------------

void loop() {

  DateTime now = rtc.now();

  // Get time
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  // Get date
  int day = now.day();
  int month = now.month();
  int year = now.year();

  // ---------------- TM1637 ----------------

  int clockValue = hour * 100 + minute;

  // Blink colon every second
  uint8_t colon = (second % 2 == 0)
                    ? 0b01000000
                    : 0;

  segment.showNumberDecEx(
    clockValue,
    colon,
    true
  );

  // ---------------- OLED ----------------

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(20, 0);
  display.println("RAJAN ROBOTICS");

  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  // Time
  display.setTextSize(2);
  display.setCursor(10, 20);

  if (hour < 10) display.print("0");
  display.print(hour);
  display.print(":");

  if (minute < 10) display.print("0");
  display.print(minute);
  display.print(":");

  if (second < 10) display.print("0");
  display.print(second);

  // Date
  display.setTextSize(1);
  display.setCursor(10, 45);

  if (day < 10) display.print("0");
  display.print(day);
  display.print("-");

  if (month < 10) display.print("0");
  display.print(month);
  display.print("-");
  display.print(year);

  // Temperature
  display.setCursor(10, 56);
  display.print("Temp: ");
  display.print(rtc.getTemperature(), 1);
  display.print(" C");

  display.display();

  // Serial Monitor
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  delay(200);
}
