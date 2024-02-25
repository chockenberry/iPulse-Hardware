
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

int interval = 500;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

void setup() {
  // put your setup code here, to run once:

  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  Serial.begin(115200);
  while (!Serial); // wait for Serial port to finish opening

  Serial.println("display test");

  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED initialized");

  //display.display();
  //delay(1000);

  display.clearDisplay();
  display.display();

  display.cp437(true);
  display.setRotation(1);
  display.setContrast(255);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  // display.setCursor(0,0);
  // display.print("Disk");
  // display.drawRoundRect(28, 0, 64, 7, 2, SH110X_WHITE);
  // display.fillRect(29, 1, 32, 5, SH110X_WHITE);
  //display.drawRect(0, 0, 128, 64, SH110X_WHITE);

  int16_t spacing = 9;
  int16_t start = 1;

  displayGraph("\x0F", 0, start + (spacing * 0), 64, 8);
  displayGraph("\x18", 0, start + (spacing * 1), 64, 16);
  displayGraph("\x19", 0, start + (spacing * 2), 64, 32);
  displayGraph("\x1E", 0, start + (spacing * 3), 64, 48);
  displayGraph("\x1F", 0, start + (spacing * 4), 64, 63);
  displayGraph("\xB0", 0, start + (spacing * 5), 64, 8);
  displayGraph("jy\x80", 0, start + (spacing * 6), 64, 0);
  display.display();

  Serial.println("setup done");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    String readString = Serial.readStringUntil('\r');
    readString.trim();
    interval = readString.toInt();
    Serial.printf("interval = %d\n", interval);
  }

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(interval);              // wait for a second

  if (!digitalRead(BUTTON_A)) {
    //display.print("A");
    interval = 100;
    display.setContrast(0);
  }
  if (!digitalRead(BUTTON_B)) {
    //display.print("B");
    interval = 500;
    display.setContrast(255);
  }

  if (!digitalRead(BUTTON_C)) {
    //display.print("C");
    interval = 1000;
  }

  //delay(10);
  //yield();
  //display.display();

}

void displayGraph(const char *name, int16_t x, int16_t y, int16_t w, int16_t v) {
  const int16_t offset = 8;

  display.setCursor(x, y);
  display.print(name);
  display.drawRoundRect(x + offset, y, w, 7, 2, SH110X_WHITE);
  display.fillRect(x + offset + 1, y + 1, v, 5, SH110X_WHITE);
}
