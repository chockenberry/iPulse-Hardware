// Display.h

#include <Adafruit_SH110X.h>

#include "Data.h"
#include "Utilities.h"
#include "Bitmaps.h"

#ifndef _Display_H_
#define _Display_H_

// convert color 24-bit color (RRRRRRRR GGGGGGGG BBBBBBBB) to 16-bit color (RRRRR GGGGGGG RRRRR)
#define CONVERT_COLOR(color) ((int)((((color & 0xff0000) >> 16) / 255.0) * 31) << 11) | ((int)((((color & 0x00ff00) >> 8) / 255.0) * 63) << 5) | ((int)((((color & 0x0000ff) >> 0) / 255.0) * 31) << 0)

#define PROCESSOR_COLOR CONVERT_COLOR(0x097ddb)
#define UPLOAD_COLOR CONVERT_COLOR(0x00a802)
#define DOWNLOAD_COLOR CONVERT_COLOR(0xfc3c28)
#define READ_COLOR CONVERT_COLOR(0xf06b00)
#define WRITE_COLOR CONVERT_COLOR(0xffae00)
#define STORAGE_EVEN_COLOR CONVERT_COLOR(0xfb5c6b)
#define STORAGE_ODD_COLOR CONVERT_COLOR(0xe8137a)

// #define MEMORY_WIRED_COLOR CONVERT_COLOR(0x68fcea)
// #define MEMORY_APP_COLOR CONVERT_COLOR(0x1ac8e8)
// #define MEMORY_COMPRESSED_COLOR CONVERT_COLOR(0xcf72fe)
#define MEMORY_WIRED_COLOR CONVERT_COLOR(0x0b81ad)
#define MEMORY_APP_COLOR CONVERT_COLOR(0x005c94)
#define MEMORY_COMPRESSED_COLOR CONVERT_COLOR(0x791da8)

//#define BACKGROUND_COLOR CONVERT_COLOR(0x444444)
#define BACKGROUND_COLOR CONVERT_COLOR(0x241b52)
#define FILL_COLOR CONVERT_COLOR(0x241b52)

const int16_t barWidth = 240;
const int16_t diskWidth = 150;
const int16_t spacer = 4;
const int16_t characterWidth = 10;
const int16_t characterHeight = 18;
const int16_t characterSpacer = 1;
const int16_t bitmapDimension = 7;
const int16_t inset = 60;
const int16_t ascenderOffset = 3;

// display disk mode: 0 = bytes used, 1 = percentage used, 2 = bytes free, 3 = percentage free
int displayDiskMode = 0;

// display activity mode: 0 = 1 Mbps & 10 MB/s, 1 = 10 Mbps & 100 MB/s, 2 = 100 Mbps & 1 GB/s
int displayActivityMode = 0;

void configureDisplay(Adafruit_ST7789 &display, const GFXfont &font, GFXcanvas16 &canvas) {
  display.init(135, 240);  // Init ST7789 240x135

  display.setRotation(1);
  display.cp437(true);

  canvas.setFont(&font);
  canvas.setTextSize(1);
  canvas.setTextColor(ST77XX_WHITE);
  canvas.fillScreen(ST77XX_BLACK);
}

void renderGraph(GFXcanvas16 &canvas, uint16_t color, const char *label, const char *units, int16_t x, int16_t y, float value) {
  value = clampedValue(value);

  const int16_t graphHeight = 19;

  const int16_t width = barWidth * value;
  canvas.fillRect(x, y, width, graphHeight, color);
  canvas.fillTriangle(width, y, width + (graphHeight / 2), y + (graphHeight / 2), width, y + graphHeight - 1, color);

  int16_t labelX = 0;
  int16_t labelY = 0;
  uint16_t labelWidth = 0;
  uint16_t labelHeight = 0;
  canvas.getTextBounds(label, 0, 0, &labelX, &labelY, &labelWidth, &labelHeight);

  canvas.setCursor(x + barWidth - inset - spacer - labelWidth, y + characterHeight - ascenderOffset);
  canvas.print(label);
  canvas.setCursor(x + barWidth - inset + spacer, y + characterHeight - ascenderOffset);
  canvas.print(units);
}

void renderMemory(GFXcanvas16 &canvas, uint16_t wiredColor, uint16_t appColor, uint16_t compressedColor, const char *label, const char *units, int16_t x, int16_t y, float wiredValue, float appValue, float compressedValue) {
  wiredValue = clampedValue(wiredValue);
  appValue = clampedValue(appValue);
  compressedValue = clampedValue(compressedValue);

  const int16_t graphHeight = 19;

  int16_t start = x;
  {
    const int16_t width = barWidth * wiredValue;
    canvas.fillRect(start, y, width, graphHeight, wiredColor);
    start += width;
  }
  {
    const int16_t width = barWidth * appValue;
    canvas.fillRect(start, y, width, graphHeight, appColor);
    start += width;
  }
  {
    const int16_t width = barWidth * compressedValue;
    canvas.fillRect(barWidth - width, y, width, graphHeight, compressedColor);
  }


  int16_t labelX = 0;
  int16_t labelY = 0;
  uint16_t labelWidth = 0;
  uint16_t labelHeight = 0;
  canvas.getTextBounds(label, 0, 0, &labelX, &labelY, &labelWidth, &labelHeight);

  canvas.setCursor(x + barWidth - inset - spacer - labelWidth, y + characterHeight - ascenderOffset);
  canvas.print(label);
  canvas.setCursor(x + barWidth - inset + spacer, y + characterHeight - ascenderOffset);
  canvas.print(units);
}

void renderActivity(GFXcanvas16 &canvas, DataPtr data) {
  const int16_t stride = 19;
  const int16_t start = 0;
  const int16_t indent = 0;

  char cpuLabel[5];
  percentageLabel(data->cpuUsage, 1, cpuLabel);

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

  canvas.fillScreen(ST77XX_BLUE);

  // display activity mode: 0 = 1 Mbps & 10 MB/s, 1 = 10 Mbps & 100 MB/s, 2 = 100 Mbps & 1 GB/s
  float networkScale = 0.0;
  float diskScale = 0.0;
  switch (displayActivityMode) {
    default:
    case 0:
      networkScale = 1.0 * 1000.0 * 1000.0;  // 1 Mbps
      diskScale = 10.0 * 1024.0 * 1024.0;    // 10 MB/s
      break;
    case 1:
      networkScale = 10.0 * 1000.0 * 1000.0;  // 10 Mbps
      diskScale = 100.0 * 1024.0 * 1024.0;    // 100 MB/s
      break;
    case 2:
      networkScale = 100.0 * 1000.0 * 1000.0;  // 100 Mbps
      diskScale = 1024.0 * 1024.0 * 1024.0;    // 1 GB/s
      break;
  }

#if 0
  canvas.fillScreen(ST77XX_BLACK);

  renderGraph(canvas, PROCESSOR_COLOR, cpuLabel, "%", indent, start + (stride * 0), 0.95);
  renderGraph(canvas, DOWNLOAD_COLOR, downloadLabel, downloadUnits, indent, start + (stride * 1), 1.0);
  renderGraph(canvas, UPLOAD_COLOR, uploadLabel, uploadUnits, indent, start + (stride * 2), 1.0);
  renderGraph(canvas, READ_COLOR, readLabel, readUnits, indent, start + (stride * 3), 1.0);
  renderGraph(canvas, WRITE_COLOR, writeLabel, writeUnits, indent, start + (stride * 4), 0.9);

  //renderGraph(canvas, MEMORY_COMPRESSED_COLOR, memoryLabel, memoryUnits, indent, start + (stride * 5), 0.8);
  renderMemory(canvas, MEMORY_WIRED_COLOR, MEMORY_APP_COLOR, MEMORY_COMPRESSED_COLOR, memoryLabel, memoryUnits, indent, start + (stride * 5),
      0.2, 0.4, 0.3);
#else
  canvas.fillScreen(ST77XX_BLACK);

  renderGraph(canvas, PROCESSOR_COLOR, cpuLabel, "%", indent, start + (stride * 0), data->cpuUsage);
  renderGraph(canvas, DOWNLOAD_COLOR, downloadLabel, downloadUnits, indent, start + (stride * 1), (float)(data->networkReceivedBps) / networkScale);
  renderGraph(canvas, UPLOAD_COLOR, uploadLabel, uploadUnits, indent, start + (stride * 2), (float)(data->networkSentBps) / networkScale);
  renderGraph(canvas, READ_COLOR, readLabel, readUnits, indent, start + (stride * 3), (float)(data->diskReadBytes) / diskScale);
  renderGraph(canvas, WRITE_COLOR, writeLabel, writeUnits, indent, start + (stride * 4), (float)(data->diskWriteBytes) / diskScale);

  //renderGraph(canvas, MEMORY_WIRED_COLOR, memoryLabel, memoryUnits, indent, start + (stride * 5), (float)memoryTotalBytes / (float)data->memoryPhysicalSize);
  renderMemory(canvas, MEMORY_WIRED_COLOR, MEMORY_APP_COLOR, MEMORY_COMPRESSED_COLOR, memoryLabel, memoryUnits, indent, start + (stride * 5),
      (float)data->memoryWiredSize / (float)data->memoryPhysicalSize, (float)data->memoryAppSize / (float)data->memoryPhysicalSize, (float)data->memoryCompressedSize / (float)data->memoryPhysicalSize);
#endif

  int64_t bottomHeight = 135 - 5;

  canvas.fillRect(0, 135 - 21, 240, 21, BACKGROUND_COLOR);

  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  char uptimeLabel[16];
  snprintf(uptimeLabel, sizeof(uptimeLabel), "%.1f days", daysUptime);

  canvas.setCursor(indent + spacer, bottomHeight);
  canvas.print(uptimeLabel);

  char loadLabel[10];
  snprintf(loadLabel, sizeof(loadLabel), "%.1f", data->load);

  int16_t labelX = 0;
  int16_t labelY = 0;
  uint16_t labelWidth = 0;
  uint16_t labelHeight = 0;
  canvas.getTextBounds(loadLabel, 0, 0, &labelX, &labelY, &labelWidth, &labelHeight);

  canvas.setCursor(indent + barWidth - inset - spacer - labelWidth, bottomHeight);
  canvas.print(loadLabel);
  canvas.setCursor(indent + barWidth - inset + spacer, bottomHeight);
  canvas.print("load");
}

void renderDisk(GFXcanvas16 &canvas, uint16_t color, const char *name, const char *label, const char *units, int16_t x, int16_t y, float value) {
  value = clampedValue(value);

  //canvas.fillRoundRect(x, y, diskWidth * value, 7, 2, ST77XX_WHITE);

  const int16_t graphHeight = 19;

  const int16_t width = diskWidth * value;
  canvas.fillRect(x, y, width, graphHeight, color);

  int16_t labelX = 0;
  int16_t labelY = 0;
  uint16_t labelWidth = 0;
  uint16_t labelHeight = 0;
  canvas.getTextBounds(label, 0, 0, &labelX, &labelY, &labelWidth, &labelHeight);

  canvas.setCursor(x + barWidth - (inset / 2) - spacer - labelWidth, y + characterHeight - ascenderOffset);
  canvas.print(label);
  canvas.setCursor(x + barWidth - (inset / 2) + spacer, y + characterHeight - ascenderOffset);
  canvas.print(units);

  // canvas.setCursor(x + diskWidth - inset - spacer - labelWidth, y + characterHeight - ascenderOffset);
  // canvas.print(label);
  // canvas.setCursor(x + diskWidth - inset + spacer, y + characterHeight - ascenderOffset);
  // canvas.print(units);

  canvas.setCursor(x + spacer, y + characterHeight - ascenderOffset);
  canvas.print(name);
}

void renderDisks(GFXcanvas16 &canvas, DataPtr data) {
  const int16_t stride = 19;
  const int16_t start = 0;
  const int16_t indent = 0;

  canvas.fillScreen(ST77XX_BLACK);


#if 1
  for (int i = 0; i < volumeCount; i++) {
    Volume volume = data->volumes[i];
    if (volume.inUse) {
      char diskLabel[volumeNameLength + 1];
      snprintf(diskLabel, volumeNameLength + 1, data->volumes[i].name);

      int64_t totalBytes = data->volumes[i].totalBytes;
      int64_t freeBytes = data->volumes[i].availableBytes;
      int64_t usedBytes = totalBytes - freeBytes;
      char sizeLabel[5];
      char sizeUnits[5];
      switch (displayDiskMode) {
        default:
        case 0: // bytes used
          bytes10Label(usedBytes, sizeLabel, sizeUnits);
          break;
        case 1: // percentage used
          percentageLabel(((float)usedBytes / (float)totalBytes), 0, sizeLabel);
          strncpy(sizeUnits, "%", 2);
         break;
        case 2: // bytes free
          bytes10Label(freeBytes, sizeLabel, sizeUnits);
          break;
        case 3: // percentage free
          percentageLabel(((float)freeBytes / (float)totalBytes), 0, sizeLabel);
          strncpy(sizeUnits, "%", 2);
          break;
      }

      int16_t color = (i % 2 == 0 ? STORAGE_EVEN_COLOR : STORAGE_ODD_COLOR);
      renderDisk(canvas, color, diskLabel, sizeLabel, sizeUnits, indent, start + (stride * i), (float)usedBytes / (float)totalBytes);
    }
  }
#else
  for (int i = 0; i < volumeCount; i++) {
    char diskLabel[volumeNameLength + 1];
    snprintf(diskLabel, volumeNameLength + 1, "Volume %d", i);

    char sizeLabel[5];
    char sizeUnits[5];
    snprintf(sizeLabel, 5, "%d.%d", i * 10, i + 1);
    strncpy(sizeUnits, "%", 2);

    int16_t color = (i % 2 == 0 ? STORAGE_EVEN_COLOR : STORAGE_ODD_COLOR);
    renderDisk(canvas, color, diskLabel, sizeLabel, sizeUnits, indent, start + (stride * i), (float)(i + 1) / (float)volumeCount);
  }
#endif

  int64_t bottomHeight = 135 - 5;

  canvas.writeFastVLine(diskWidth, 0, bottomHeight, BACKGROUND_COLOR);

  canvas.fillRect(0, 135 - 21, 240, 21, BACKGROUND_COLOR);

  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  char uptimeLabel[16];
  snprintf(uptimeLabel, sizeof(uptimeLabel), "%.1f days", daysUptime);

  canvas.setCursor(indent + spacer, bottomHeight);
  canvas.print(uptimeLabel);

  char loadLabel[10];
  snprintf(loadLabel, sizeof(loadLabel), "%.1f", data->load);

  int16_t labelX = 0;
  int16_t labelY = 0;
  uint16_t labelWidth = 0;
  uint16_t labelHeight = 0;
  canvas.getTextBounds(loadLabel, 0, 0, &labelX, &labelY, &labelWidth, &labelHeight);

  canvas.setCursor(indent + barWidth - inset - spacer - labelWidth, bottomHeight);
  canvas.print(loadLabel);
  canvas.setCursor(indent + barWidth - inset + spacer, bottomHeight);
  canvas.print("load");
}

void renderStart(GFXcanvas16 &canvas) {
  canvas.fillScreen(ST77XX_BLACK);

  canvas.fillRoundRect(0, 0, 240, 135, 10, FILL_COLOR);
  canvas.setCursor(32, ((characterHeight + characterSpacer) * 2.25));
  canvas.print("   Start the iPulse");
  canvas.setCursor(32, ((characterHeight + characterSpacer) * 3.25) + (spacer * 1));
  canvas.print("daemon in Terminal");
  canvas.setCursor(32, ((characterHeight + characterSpacer) * 4.25) + (spacer * 2));
  canvas.print("     on your Mac.");
  canvas.drawRoundRect(0, 0, 240, 135, 10, MEMORY_COMPRESSED_COLOR);
  canvas.drawRoundRect(1, 1, 238, 133, 10, MEMORY_COMPRESSED_COLOR);
}

void renderSleep(GFXcanvas16 &canvas) {
  int x = random(0, 240 - ((characterWidth + characterSpacer) * 3));
  int y = random(0, 135 - (characterHeight + characterSpacer)) + characterHeight;
  canvas.fillScreen(ST77XX_BLACK);
  canvas.setCursor(x, y);
  canvas.print("zzz");
}

#endif  // _Display_H_