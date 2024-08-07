
#include <Arduino.h>
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SH110X.h>
#include <Adafruit_ST7789.h> 

#include <ArduinoJson.h>

#include "Bitmaps.h"
#include "Utilities.h"
#include "Data.h"
#include "Display.h"

#include "Geneva-2x.h"
//#include "EspySans-9pt2x.h"
//#include "Monaco-2x.h"

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_C 2

#define DEBUG 0

JsonDocument document;  // the JSON document received from the serial port
Data data;              // data from the JSON document

//Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);  // the OLED display
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); // the TFT display
GFXcanvas16 canvas(240, 135);

// set to true when data is received from serial port and ready to display
bool needsUpdate = false;

// displaying cpu/network/disk/memory activity or disk capacities
bool displayingActivity = true;

// displaying mode status
bool displayingMode = false;

// displaying overlay
bool displayingOverlay = false;

void configurePins() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLDOWN);
  pinMode(BUTTON_C, INPUT_PULLDOWN);

  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
#if DEBUG
  while (!Serial)
    ;  // wait for Serial port to finish opening
  Serial.println("ready");
#endif

  delay(100);  // wait for the TFT to power up
  configureDisplay(display, font, canvas);
  configurePins();

  setHeartbeat(false);
  renderStart(canvas);
  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);

#if DEBUG
  Serial.println("setup done");
#endif
  // turn these on for easier debugging
  //needsUpdate = true;
  //displayingActivity = false;
}

// built-in LED lights up for 1/10 of a second each time data is received over the serial port (once per second)
const int heartbeatPulse = 100;
unsigned long previousHeartbeatMillis = 0;

// sleep mode starts after three seconds without any serial data
const int sleepInterval = 3000;
unsigned long previousSleepMillis = 0;

// sleep mode starts after three seconds without any serial data
const int modeInterval = 5000;
unsigned long previousModeMillis = 0;

// set to true when data is first received from serial port
bool hasFirstUpdate = false;


// debounced button states
unsigned long debounceDelay = 100;
unsigned long previousButtonAMillis = 0;
unsigned long previousButtonBMillis = 0;
unsigned long previousButtonCMillis = 0;
bool pressedButtonA = false;
bool pressedButtonB = false;
bool pressedButtonC = false;

void loop() {
  // put your main code here, to run repeatedly:

  // timestamp for this iteration of loop
  unsigned long currentMillis = millis();

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
    } else {
      updateData(document, &data);
      hasFirstUpdate = true;
      needsUpdate = true;
    }
  }

  if (!digitalRead(BUTTON_A)) {
    if (previousButtonAMillis == 0) {
      previousButtonAMillis = currentMillis;
    } else if (currentMillis - previousButtonAMillis > debounceDelay) {
      pressedButtonA = true;
    }
  } else {
    if (pressedButtonA) {
      if (displayingActivity) {
        displayingOverlay = true;
        previousModeMillis = currentMillis;
        needsUpdate = true;
      }
      pressedButtonA = false;
    }

    previousButtonAMillis = 0;
  }

  if (!digitalRead(BUTTON_B)) {
    if (previousButtonBMillis == 0) {
      previousButtonBMillis = currentMillis;
    } else if (currentMillis - previousButtonBMillis > debounceDelay) {
      pressedButtonB = true;
    }
  } else {
    if (pressedButtonB) {
      if (displayingMode) {
        if (displayingActivity) {
          displayActivityMode += 1;
          if (displayActivityMode > 3) {
            displayActivityMode = 0;
          }
        }
        else {
          displayDiskMode += 1;
          if (displayDiskMode > 3) {
            displayDiskMode = 0;
          }
        }
      }
      displayingMode = true;
      previousModeMillis = currentMillis;

      needsUpdate = true;
      pressedButtonB = false;
    }
    previousButtonBMillis = 0;
  }

  if (!digitalRead(BUTTON_C)) {
    if (previousButtonCMillis == 0) {
      previousButtonCMillis = currentMillis;
    } else if (currentMillis - previousButtonCMillis > debounceDelay) {
      pressedButtonC = true;
    }
  } else {
    if (pressedButtonC) {
      //if (hasFirstUpdate) {
        displayingActivity = !displayingActivity;
        needsUpdate = true;
      //}
      pressedButtonC = false;
    }
    previousButtonCMillis = 0;
  }

  if (needsUpdate) {
    if (displayingActivity) {
      renderActivity(canvas, &data);
      if (displayingOverlay) {
        renderOverlay(canvas);
      }
    } else {
      renderDisks(canvas, &data);
    }
    if (displayingMode) {
      renderMode(canvas, displayingActivity);
    }
    else {
      renderStatus(canvas, &data);
    }
    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);

    setHeartbeat(true);
    previousHeartbeatMillis = currentMillis;
    previousSleepMillis = currentMillis;

    needsUpdate = false;
  } else {
    // update heartbeat, if necessary
    if (previousHeartbeatMillis != 0) {
      if (currentMillis - previousHeartbeatMillis > heartbeatPulse) {
        setHeartbeat(false);
        previousHeartbeatMillis = 0;
      }
    }

    // update sleep mode, if necessary
    if (hasFirstUpdate) {
      if (currentMillis - previousSleepMillis > sleepInterval) {
        renderSleep(canvas);
        display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
        previousSleepMillis = currentMillis;
      }
    }

    if (displayingMode || displayingOverlay) {
      if (currentMillis - previousModeMillis > modeInterval) {
        displayingMode = false;
        displayingOverlay = false;
        previousModeMillis = 0;
        needsUpdate = true;
      }
    }
  }
}

bool heartbeatState = false;
void setHeartbeat(bool flag) {
  if (heartbeatState != flag) {
    digitalWrite(LED_BUILTIN, flag ? HIGH : LOW);
    heartbeatState = flag;
  }
}
void toggleHeartbeat() {
  setHeartbeat(!heartbeatState);
}
