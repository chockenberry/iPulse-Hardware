
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <ArduinoJson.h>

#include "Bitmaps.h"
#include "Utilities.h"

JsonDocument document;

bool isDimmed = true;
bool heartbeatState = false;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5

#define DEBUG 0

struct data {
  float cpuUsage;  // value from 0.0 to 1.0 (inclusive)

  int64_t networkReceivedBps;
  int64_t networkSentBps;

  int64_t diskReadBytes;
  int64_t diskWriteBytes;

  int64_t memoryPhysicalSize;
  int64_t memoryWiredSize;
  int64_t memoryAppSize;
  int64_t memoryCompressedSize;

  float load;
  int32_t uptime;
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

PROGMEM


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
  displayStart();

#if DEBUG
  Serial.println("setup done");
#endif
}

const int heartbeatInterval = 500;
const int heartbeatPulse = 50;
unsigned long previousHeartbeatMillis = 0;

const int sleepInterval = 5000;
unsigned long previousSleepMillis = 0;

bool needsUpdate = false;
//bool needsUpdate = true;


void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    String readString = Serial.readStringUntil('\r');
    readString.trim();

    DeserializationError error = deserializeJson(document, readString);
    if (error != DeserializationError::Ok) {
#if DEBUG
      Serial.print("deserialization error: ");
      Serial.println(error.c_str());
      Serial.print("with string: ");
      Serial.println(readString);
#endif
    }
    else {
      data.cpuUsage = document["cpu_usage"];
      data.networkReceivedBps = document["network_received_bps"];
      data.networkSentBps = document["network_sent_bps"];
      data.diskReadBytes = document["disk_read_bytes"];
      data.diskWriteBytes = document["disk_write_bytes"];
      data.memoryPhysicalSize = document["memory_physical_size"];
      data.memoryWiredSize = document["memory_wired_size"];
      data.memoryAppSize = document["memory_app_size"];
      data.memoryCompressedSize = document["memory_compressed_size"];
      data.load = document["load_five_minutes"];
      data.uptime = document["uptime"];
#if DEBUG
      //Serial.println(readString);
      //Serial.printf("foo\n");
      //Serial.printf("cpuLevel = %f", data.cpuUsage);
#endif
      needsUpdate = true;
    }
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
    previousSleepMillis = currentMillis;
    needsUpdate = false;
  }
  else {
    if (currentMillis - previousHeartbeatMillis > heartbeatPulse) {
      setHeartbeat(false);
    }
    if (currentMillis - previousSleepMillis > sleepInterval) {
      displaySleep();
      delay(1000);
    }
  }
}

const int16_t barWidth = 64;
const int16_t spacer = 2;
const int16_t characterWidth = 5;
const int16_t characterHeight = 7;
const int16_t characterSpacer = 1;
const int16_t bitmapDimension = 7;

void displayGraph(const unsigned char *bitmap, const char *label, const char *units, int16_t x, int16_t y, float value) {

  if (value < 0.0) {
    value = 0.0;
  }
  else if (value > 1.0) {
    value = 1.0;
  }
  
  display.fillRoundRect(x, y, barWidth * value, 7, 2, SH110X_WHITE);

  display.drawBitmap(x + barWidth + spacer, y, bitmap, 7, 7, SH110X_WHITE);

  //display.setCursor(x + w + spacer, y);
  //display.print(icon);
  display.setCursor(x + barWidth + spacer + bitmapDimension + spacer, y);
  display.print(label);
  display.setCursor(x + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, y);
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

void setHeartbeat(bool flag) {
  if (heartbeatState != flag) {
    digitalWrite(LED_BUILTIN, flag ? HIGH : LOW);
    heartbeatState = flag;
  }
}

void displayData(dataPtr data) {
  int16_t stride = 9;
  int16_t start = 0;
  int16_t indent = 0;

  char cpuLabel[5];
  percentageLabel(data->cpuUsage, cpuLabel);

  char downloadLabel[5];
  char downloadUnits[5];
  bitsPerSecondLabel(data->networkReceivedBps, downloadLabel, downloadUnits);

   
  char uploadLabel[5];
  char uploadUnits[5];
  bitsPerSecondLabel(data->networkSentBps, uploadLabel, uploadUnits);

  char readLabel[5];
  char readUnits[5];
  bytesPerSecondLabel(data->diskReadBytes, readLabel, readUnits);

  char writeLabel[5];
  char writeUnits[5];
  bytesPerSecondLabel(data->diskWriteBytes, writeLabel, writeUnits);

  int64_t memoryTotalBytes = data->memoryWiredSize + data->memoryAppSize + data->memoryCompressedSize;
  char memoryLabel[5];
  char memoryUnits[5];
  bytesLabel(memoryTotalBytes, memoryLabel, memoryUnits);

  // Serial.print(readLabel);
  // Serial.print(" ");
  // Serial.println(readUnits);
 
  display.clearDisplay();

  // TODO: These should be configurable somehow...
  const float networkScale = 1.0 * 1000.0 * 1000.0; // 1 Mbps
  const float diskScale = 10.0 * 1024.0 * 1024.0; // 10 MB/s

  
  displayGraph(bitmapCPU, cpuLabel, "%", indent, start + (stride * 0), data->cpuUsage);
  displayGraph(bitmapUpload, uploadLabel, uploadUnits, indent, start + (stride * 1), (float)(data->networkSentBps) / networkScale);
  displayGraph(bitmapDownload, downloadLabel, downloadUnits, indent, start + (stride * 2), (float)(data->networkReceivedBps) / networkScale);
  displayGraph(bitmapRead, readLabel, readUnits, indent, start + (stride * 3), (float)(data->diskReadBytes) / diskScale);
  displayGraph(bitmapWrite, writeLabel, writeUnits, indent, start + (stride * 4), (float)(data->diskWriteBytes) / diskScale);
  displayGraph(bitmapMemory, memoryLabel, memoryUnits, indent, start + (stride * 5), (float)memoryTotalBytes / (float)data->memoryPhysicalSize );

  //display.setCursor(0, 64 - 15);
  //display.print("NEXT TO LAST LINE");

  display.drawLine(0, 64 - 8 - 3, 128, 64 - 8 - 3, SH110X_WHITE);
  //display.drawLine(63, 64 - 8 - 4, 63, 64, SH110X_WHITE);
  //display.drawRect(0, 64 - 8 - 5, 128, 8 + 5, SH110X_WHITE);
  //display.drawLine(63, 64 - 8 - 5, 63, 64, SH110X_WHITE);

  int64_t bottomHeight = 64 - characterHeight - characterSpacer;

  display.drawBitmap(indent , bottomHeight, bitmapUptime, 7, 7, SH110X_WHITE);
  display.setCursor(indent + bitmapDimension + spacer, bottomHeight);
  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  if (daysUptime > 99.9) {
    uptimeDecimalPlaces = 0;
  }
  char uptimeLabel[5];
  snprintf(uptimeLabel, 5, "%4.*f", uptimeDecimalPlaces, daysUptime);
  display.print(uptimeLabel);
  //display.print("----");
  display.setCursor(indent + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  display.print("days");

  //display.setCursor(0, 64 - 8);
  //display.setCursor(2, 64 - 8 - 2);
  //display.print("---- days");

  //display.setCursor(66, 64 - 8);
  //display.setCursor(66, 64 - 8 - 2);
 

  display.drawBitmap(indent + barWidth + spacer, bottomHeight, bitmapLoad, 7, 7, SH110X_WHITE);
  display.setCursor(indent + barWidth + spacer + bitmapDimension + spacer, bottomHeight);
  int loadDecimalPlaces = 1;
  if (data->load > 99.9) {
    loadDecimalPlaces = 0;
  }
  char loadLabel[5];
  snprintf(loadLabel, 5, "%4.*f", loadDecimalPlaces, data->load);
  display.print(loadLabel);
  display.setCursor(indent + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  display.print("load");

  //display.setCursor(x + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, y);

  //displayGraph("\x80", "   -", "TBD", indent, start + (stride * 6), 0);
  display.display();
}

void displayStart() {
  display.clearDisplay();
  display.setCursor(16, 8);
  display.print("Start iPulse from");
  display.setCursor(16, 17);
  display.print("the Terminal on");
  display.setCursor(16, 26);
  display.print("your Mac.");
  display.display();
}

void displaySleep() {
  int x = random(0, 128 - (6 * 4));
  int y = random(0, 64 - 8);
  display.clearDisplay();
  display.setCursor(x, y);
  display.print("Zzzz");
  display.display();
}
