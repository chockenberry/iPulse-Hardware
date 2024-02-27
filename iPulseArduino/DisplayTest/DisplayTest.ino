
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

bool isDimmed = true;
bool heartbeatState = false;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5

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

// 'CPU', 8x8px
const unsigned char bitmapCPU[] PROGMEM = {
  0xa5, 0x00, 0xbd, 0x3c, 0x3c, 0xbd, 0x00, 0xa5
};
// 'Uptime', 8x8px
const unsigned char bitmapUptime[] PROGMEM = {
  0xff, 0x81, 0xa5, 0xa5, 0xa5, 0x99, 0x81, 0xff
};
// 'Download', 8x8px
const unsigned char bitmapDownload[] PROGMEM = {
  0x55, 0xaa, 0x00, 0x04, 0x04, 0x15, 0x0e, 0x04
};
// 'Load', 8x8px
const unsigned char bitmapLoad[] PROGMEM = {
  0xff, 0x81, 0xa1, 0xa1, 0xa1, 0xbd, 0x81, 0xff
};
// 'Memory', 8x8px
const unsigned char bitmapMemory[] PROGMEM = {
  0x0e, 0xee, 0xee, 0xe0, 0x07, 0x77, 0x77, 0x70
};
// 'Read', 8x8px
const unsigned char bitmapRead[] PROGMEM = {
  0x20, 0x70, 0xa8, 0x20, 0x20, 0x00, 0xdf, 0xff
};
// 'Upload', 8x8px
const unsigned char bitmapUpload[] PROGMEM = {
  0x55, 0xaa, 0x00, 0x20, 0x70, 0xa8, 0x20, 0x20
};
// 'Write', 8x8px
const unsigned char bitmapWrite[] PROGMEM = {
  0x04, 0x04, 0x15, 0x0e, 0x04, 0x00, 0xfb, 0xff
};


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
#if DEBUG
  while (!Serial)
    ;  // wait for Serial port to finish opening
  Serial.println("ready");
#endif


  delay(250);                 // wait for the OLED to power up
  display.begin(0x3C, true);  // Address 0x3C default

#if DEBUG
  Serial.println("initialized");
#endif

  display.clearDisplay();
  display.display();

  configureDisplay();
  configurePins();

  //toggleHeartbeat();
  setHeartbeat(false);

#if DEBUG
  Serial.println("setup done");
#endif
}

const int heartbeatInterval = 500;
const int heartbeatPulse = 50;
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
#endif
    needsUpdate = true;
  }

  unsigned long currentMillis = millis();
  // if (currentMillis - previousHeartbeatMillis > heartbeatInterval) {
  //   toggleHeartbeat();
  //   previousHeartbeatMillis = currentMillis;
  //   needsUpdate = true;
  // }

  if (!digitalRead(BUTTON_A)) {
    setDimmed(true);
  }
  if (!digitalRead(BUTTON_B)) {
    setDimmed(false);
  }

  if (needsUpdate) {
    displayData(&data);
    setHeartbeat(true);
    previousHeartbeatMillis = currentMillis;
    needsUpdate = false;
  }
  else {
    if (currentMillis - previousHeartbeatMillis > heartbeatPulse) {
      setHeartbeat(false);
    }
  }
}

//void displayGraph(const char *icon, const char *value, const char *units, int16_t x, int16_t y, int16_t w, int16_t v) {
void displayGraph(const unsigned char *bitmap, const char *value, const char *units, int16_t x, int16_t y, int16_t w, int16_t v) {
#if 0
  const int16_t offset = 8;
  const int16_t spacer = 2;

  display.setCursor(x, y);
  display.print(icon);
  //display.drawRoundRect(x + offset, y, w, 7, 2, SH110X_WHITE);
  //display.fillRect(x + offset + 1, y + 1, v, 5, SH110X_WHITE);
  display.fillRoundRect(x + offset, y, v, 7, 2, SH110X_WHITE);
  display.setCursor(x + offset + w + spacer, y);
  display.print(value);
  display.print(" ");
  display.print(units);
#else
  const int16_t spacer = 2;
  const int16_t character = 5;

  display.fillRoundRect(x, y, v, 7, 2, SH110X_WHITE);

  display.drawBitmap(x + w + spacer, y, bitmap, 8, 8, SH110X_WHITE);

  //display.setCursor(x + w + spacer, y);
  //display.print(icon);
  display.setCursor(x + w + spacer + character + spacer, y);
  display.print(value);
  display.setCursor(x + w + spacer + character + spacer + ((character + 1) * 4) + spacer, y);
  display.print(units);
#endif
}

void setDimmed(bool flag) {
  isDimmed = flag;
  display.setContrast(isDimmed ? 0 : 255);
}

void toggleHeartbeat() {
  digitalWrite(LED_BUILTIN, heartbeatState ? HIGH : LOW);
  heartbeatState = !heartbeatState;
}

void setHeartbeat(bool flag) {
  if (heartbeatState != flag) {
    digitalWrite(LED_BUILTIN, flag ? HIGH : LOW);
    heartbeatState = flag;
  }
}

void displayData(dataPtr data) {
  int16_t barWidth = 64;
  int16_t stride = 9;
  int16_t start = 0;
  int16_t indent = 0;

  int16_t scaledCpuLevel = (int16_t)(round((float)barWidth * data->cpuLevel));
  char cpuLabel[5];
  snprintf(cpuLabel, 5, "%4d %", (int16_t)(round(data->cpuLevel * 100.0)));

  display.clearDisplay();

  // displayGraph("\x03", cpuLabel, "%", indent, start + (stride * 0), barWidth, scaledCpuLevel);
  // displayGraph("\x18", "   -", "bps", indent, start + (stride * 1), barWidth, 0);
  // displayGraph("\x19", "   -", "bps", indent, start + (stride * 2), barWidth, 0);
  // displayGraph("\x1E", "   -", "B/s", indent, start + (stride * 3), barWidth, 0);
  // displayGraph("\x1F", "   -", "B/s", indent, start + (stride * 4), barWidth, 0);
  // displayGraph("\xB0", "   -", "GB", indent, start + (stride * 5), barWidth, 0);
  displayGraph(bitmapCPU, cpuLabel, "%", indent, start + (stride * 0), barWidth, scaledCpuLevel);
  displayGraph(bitmapUpload, "   -", "bps", indent, start + (stride * 1), barWidth, 0);
  displayGraph(bitmapDownload, "   -", "bps", indent, start + (stride * 2), barWidth, 0);
  displayGraph(bitmapRead, "   -", "B/s", indent, start + (stride * 3), barWidth, 0);
  displayGraph(bitmapWrite, "   -", "B/s", indent, start + (stride * 4), barWidth, 0);
  displayGraph(bitmapMemory, "   -", "GB", indent, start + (stride * 5), barWidth, 0);

  //display.setCursor(0, 64 - 15);
  //display.print("NEXT TO LAST LINE");

  display.drawLine(0, 64 - 8 - 3, 128, 64 - 8 - 3, SH110X_WHITE);
  //display.drawLine(63, 64 - 8 - 4, 63, 64, SH110X_WHITE);
  //display.drawRect(0, 64 - 8 - 5, 128, 8 + 5, SH110X_WHITE);
  //display.drawLine(63, 64 - 8 - 5, 63, 64, SH110X_WHITE);

  display.setCursor(0, 64 - 8);
  //display.setCursor(2, 64 - 8 - 2);
  display.print("0.0 days");

  //display.setCursor(66, 64 - 8);
  //display.setCursor(66, 64 - 8 - 2);
  display.setCursor(8 + 64 + 2, 64 - 8);
  display.print("1.0 avg.");

  display.drawBitmap(64, 64 - 8, bitmapUptime, 8, 8, SH110X_WHITE);

  //displayGraph("\x80", "   -", "TBD", indent, start + (stride * 6), barWidth, 0);
  display.display();
}
