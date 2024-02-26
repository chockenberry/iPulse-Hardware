
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

bool isDimmed = true;
bool heartbeatState = false;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

#define DEBUG 0

struct data {
  float cpuLevel;
} data;
typedef struct data *dataPtr;

void displayData(dataPtr data);

void configureDisplay() {
  display.cp437(true);
  display.setRotation(1);
  setDimmed(true);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
}

void configurePins() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}

void setup() {
  // put your setup code here, to run once:

   Serial.begin(115200);
#if DEBUG
  while (!Serial); // wait for Serial port to finish opening
  Serial.println("ready");
#endif


  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

#if DEBUG
  Serial.println("initialized");
#endif

  display.clearDisplay();
  display.display();

  configureDisplay();
  configurePins();
  
  toggleHeartbeat();

 #if DEBUG
  Serial.println("setup done");
#endif
}

const int heartbeatInterval = 500;
unsigned long previousHeartbeatMillis = 0;

bool needsUpdate = true;



void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    String readString = Serial.readStringUntil('\r');
    readString.trim();
    data.cpuLevel = readString.toFloat();
    //interval = readString.toInt();
  #if DEBUG
    Serial.printf("readString = '%s'\n", readString.c_str());
    Serial.printf("cpuLevel = %f\n", data.cpuLevel);
    needsUpdate = true;
  #endif
  }

 unsigned long currentMillis = millis();
  if (currentMillis - previousHeartbeatMillis > heartbeatInterval) {
    toggleHeartbeat();
    previousHeartbeatMillis = currentMillis;
    needsUpdate = true;
  }

  if (!digitalRead(BUTTON_A)) {
    setDimmed(true);
  }
  if (!digitalRead(BUTTON_B)) {
    setDimmed(false);
  }

  if (needsUpdate) {
    displayData(&data);
    needsUpdate = false;
  }
}

void displayGraph(const char *name, const char *value, const char *units, int16_t x, int16_t y, int16_t w, int16_t v) {
  const int16_t offset = 8;
  const int16_t spacer = 2;

  display.setCursor(x, y);
  display.print(name);
  //display.drawRoundRect(x + offset, y, w, 7, 2, SH110X_WHITE);
  //display.fillRect(x + offset + 1, y + 1, v, 5, SH110X_WHITE);
  display.fillRoundRect(x + offset, y, v, 7, 2, SH110X_WHITE);
  display.setCursor(x + offset + w + spacer, y);
  display.print(value);
  display.print(" ");
  display.print(units);
}

void setDimmed(bool flag) {
  isDimmed = flag;
  display.setContrast(isDimmed ? 0 : 255);
}

void toggleHeartbeat() {
    digitalWrite(LED_BUILTIN, heartbeatState ? HIGH : LOW);
    heartbeatState = !heartbeatState;
}

void displayData(dataPtr data) {
  int16_t barWidth = 64;
  int16_t stride = 8;
  int16_t start = 0;
  int16_t indent = 0;

  int16_t scaledCpuLevel = (int16_t)(round((float)barWidth * data->cpuLevel));
  char cpuLabel[5];
  snprintf(cpuLabel, 5, "%4d %", (int16_t)(round(data->cpuLevel * 100.0)));

  display.clearDisplay();
  
  displayGraph("\x03", cpuLabel, "%", indent, start + (stride * 0), barWidth, scaledCpuLevel);
  displayGraph("\x18", "   -", "bps", indent, start + (stride * 1), barWidth, 16);
  displayGraph("\x19", " 100", "Mbps", indent, start + (stride * 2), barWidth, 32);
  displayGraph("\x1E", "1023", "MB/s", indent, start + (stride * 3), barWidth, 64);
  displayGraph("\x1F", "   1", "KB/s", indent, start + (stride * 4), barWidth, 1);
  displayGraph("\xB0", "12.1", "GB", indent, start + (stride * 5), barWidth, 8);

  //display.setCursor(0, 64 - 15);
  //display.print("NEXT TO LAST LINE");

  display.drawLine(0, 64 - 8 - 4, 128, 64 - 8 - 4, SH110X_WHITE);
  //display.drawLine(63, 64 - 8 - 4, 63, 64, SH110X_WHITE);
  //display.drawRect(0, 64 - 8 - 5, 128, 8 + 5, SH110X_WHITE);
  //display.drawLine(63, 64 - 8 - 5, 63, 64, SH110X_WHITE);

  display.setCursor(0, 64 - 8);
  //display.setCursor(2, 64 - 8 - 2);
  display.print("400.5 days");

  //display.setCursor(66, 64 - 8);
  //display.setCursor(66, 64 - 8 - 2);
  display.setCursor(8 + 64 + 2, 64 - 8);
  display.print("2.05 avg");

  //displayGraph("\x80", "   -", "TBD", indent, start + (stride * 6), barWidth, 0);
  display.display();
}
