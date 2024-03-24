// Display.h

#include <Adafruit_SH110X.h>

#include "Data.h"
#include "Utilities.h"
#include "Bitmaps.h"

#ifndef _Display_H_
#define _Display_H_

const int16_t barWidth = 64;
const int16_t diskWidth = 32;
const int16_t spacer = 3;
const int16_t characterWidth = 5;
const int16_t characterHeight = 7;
const int16_t characterSpacer = 1;
const int16_t bitmapDimension = 7;

void configureDisplay(Adafruit_SH1107 &display) {
  display.begin(0x3C, true);  // Address 0x3C default

  display.cp437(true);
  display.setRotation(3);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.clearDisplay();
  display.display();
}

void displayGraph(Adafruit_SH1107 &display, const unsigned char *bitmap, const char *label, const char *units, int16_t x, int16_t y, float value) {
  if (value < 0.0) {
    value = 0.0;
  }
  else if (value > 1.0) {
    value = 1.0;
  }
  
  display.fillRoundRect(x, y, barWidth * value, 7, 2, SH110X_WHITE);

  display.drawBitmap(x + barWidth + spacer, y, bitmap, 7, 7, SH110X_WHITE);

  display.setCursor(x + barWidth + spacer + bitmapDimension + spacer, y);
  display.print(label);
  display.setCursor(x + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, y);
  display.print(units);
}

void displayActivity(Adafruit_SH1107 &display, DataPtr data) {
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
 
  display.clearDisplay();

  // TODO: These should be configurable somehow...
  const float networkScale = 1.0 * 1000.0 * 1000.0; // 1 Mbps
  const float diskScale = 10.0 * 1024.0 * 1024.0; // 10 MB/s
  
  displayGraph(display, bitmapCPU, cpuLabel, "%", indent, start + (stride * 0), data->cpuUsage);
  displayGraph(display, bitmapUpload, uploadLabel, uploadUnits, indent, start + (stride * 1), (float)(data->networkSentBps) / networkScale);
  displayGraph(display, bitmapDownload, downloadLabel, downloadUnits, indent, start + (stride * 2), (float)(data->networkReceivedBps) / networkScale);
  displayGraph(display, bitmapRead, readLabel, readUnits, indent, start + (stride * 3), (float)(data->diskReadBytes) / diskScale);
  displayGraph(display, bitmapWrite, writeLabel, writeUnits, indent, start + (stride * 4), (float)(data->diskWriteBytes) / diskScale);
  displayGraph(display, bitmapMemory, memoryLabel, memoryUnits, indent, start + (stride * 5), (float)memoryTotalBytes / (float)data->memoryPhysicalSize );

  display.drawLine(0, 64 - 8 - 3, 128, 64 - 8 - 3, SH110X_WHITE);

  int64_t bottomHeight = 64 - characterHeight - characterSpacer;

  display.drawBitmap(indent , bottomHeight, bitmapUptime, 7, 7, SH110X_WHITE);
  display.setCursor(indent + bitmapDimension + spacer, bottomHeight);
  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  if (daysUptime > 99.9) {
    uptimeDecimalPlaces = 0;
  }
  char uptimeLabel[5];
  snprintf(uptimeLabel, sizeof(uptimeLabel), "%4.*f", uptimeDecimalPlaces, daysUptime);
  display.print(uptimeLabel);
  display.setCursor(indent + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  display.print("days");

  display.drawBitmap(indent + barWidth + spacer, bottomHeight, bitmapLoad, 7, 7, SH110X_WHITE);
  display.setCursor(indent + barWidth + spacer + bitmapDimension + spacer, bottomHeight);
  int loadDecimalPlaces = 1;
  if (data->load > 99.9) {
    loadDecimalPlaces = 0;
  }
  char loadLabel[5];
  snprintf(loadLabel, sizeof(loadLabel), "%4.*f", loadDecimalPlaces, data->load);
  display.print(loadLabel);
  display.setCursor(indent + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  display.print("load");

  display.display();
}

void displayDisk(Adafruit_SH1107 &display, const char *name, const char *label, const char *units, int16_t x, int16_t y, float value) {
  if (value < 0.0) {
    value = 0.0;
  }
  else if (value > 1.0) {
    value = 1.0;
  }
 
  display.fillRoundRect(x, y, diskWidth * value, 7, 2, SH110X_WHITE);

  display.setCursor(x + diskWidth + spacer, y);
  display.print(label);
  display.setCursor(x + diskWidth + spacer + ((characterWidth + characterSpacer) * 4) + spacer, y);
  display.print(units);

  display.setCursor(x + diskWidth + spacer + ((characterWidth + characterSpacer) * 4) + spacer + ((characterWidth + characterSpacer) * 2) + (spacer * 2), y);
  display.print(name);
}

void displayDisks(Adafruit_SH1107 &display, DataPtr data) {
  int16_t stride = 9;
  int16_t start = 0;
  int16_t indent = 0;

  display.clearDisplay();

  // TODO: These should be configurable somehow...
  const float networkScale = 1.0 * 1000.0 * 1000.0; // 1 Mbps
  const float diskScale = 10.0 * 1024.0 * 1024.0; // 10 MB/s
  
  for (int i = 0; i < volumeCount; i++) {
    Volume volume = data->volumes[i];
    if (volume.inUse) {
      char diskLabel[volumeNameLength + 1];
      snprintf(diskLabel, volumeNameLength + 1, data->volumes[i].name);

      int64_t totalBytes = data->volumes[i].totalBytes;
      int64_t usedBytes = totalBytes - data->volumes[i].availableBytes;
      char sizeLabel[5];
      char sizeUnits[5];
      bytes10Label(usedBytes, sizeLabel, sizeUnits);

      displayDisk(display, diskLabel, sizeLabel, sizeUnits, indent, start + (stride * i), (float)usedBytes / (float)totalBytes);
    }
  }
 
  display.drawLine(0, 64 - 8 - 3, 128, 64 - 8 - 3, SH110X_WHITE);

  int64_t bottomHeight = 64 - characterHeight - characterSpacer;

  display.drawBitmap(indent , bottomHeight, bitmapUptime, 7, 7, SH110X_WHITE);
  display.setCursor(indent + bitmapDimension + spacer, bottomHeight);
  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  if (daysUptime > 99.9) {
    uptimeDecimalPlaces = 0;
  }
  char uptimeLabel[5];
  snprintf(uptimeLabel, sizeof(uptimeLabel), "%4.*f", uptimeDecimalPlaces, daysUptime);
  display.print(uptimeLabel);
  display.setCursor(indent + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  display.print("days");

  display.drawBitmap(indent + barWidth + spacer, bottomHeight, bitmapLoad, 7, 7, SH110X_WHITE);
  display.setCursor(indent + barWidth + spacer + bitmapDimension + spacer, bottomHeight);
  int loadDecimalPlaces = 1;
  if (data->load > 99.9) {
    loadDecimalPlaces = 0;
  }
  char loadLabel[5];
  snprintf(loadLabel, sizeof(loadLabel), "%4.*f", loadDecimalPlaces, data->load);
  display.print(loadLabel);
  display.setCursor(indent + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  display.print("load");

  display.display();
}

void displayStart(Adafruit_SH1107 &display) {
  display.clearDisplay();
  display.setCursor(10, ((characterWidth + characterSpacer) * 3));
  display.print(" Start the iPulse");
  display.setCursor(10, ((characterWidth + characterSpacer) * 4) + (spacer * 1));
  display.print("daemon in Terminal");
  display.setCursor(10, ((characterWidth + characterSpacer) * 5) + (spacer * 2));
  display.print("   on your Mac.");
  display.drawRoundRect(0, 0, 128, 64, 10, SH110X_WHITE);
  display.display();
}

void displaySleep(Adafruit_SH1107 &display) {
  int x = random(0, 128 - ((characterWidth + characterSpacer) * 3));
  int y = random(0, 64 - (characterHeight + characterSpacer));
  display.clearDisplay();
  display.setCursor(x, y);
  display.print("zzz");
  display.display();
}

#endif // _Display_H_