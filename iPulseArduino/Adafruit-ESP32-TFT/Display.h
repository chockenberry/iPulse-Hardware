// Display.h

#include <Adafruit_SH110X.h>

#include "Data.h"
#include "Utilities.h"
#include "Bitmaps.h"

#ifndef _Display_H_
#define _Display_H_

const int16_t barWidth = 240;
const int16_t diskWidth = 120;
const int16_t spacer = 3;
const int16_t characterWidth = 10;
const int16_t characterHeight = 18;
const int16_t characterSpacer = 1;
const int16_t bitmapDimension = 7;

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
  if (value < 0.0) {
    value = 0.0;
  } else if (value > 1.0) {
    value = 1.0;
  }

  const int16_t graphHeight = 19;

  const int16_t width = barWidth * value;
  canvas.fillRect(x, y, width, graphHeight, color);
  canvas.fillTriangle(width, y, width + (graphHeight / 2), y + (graphHeight / 2), width, y + graphHeight - 1, color);

  const int16_t inset = 100;
  const int16_t ascenderOffset = 3;

  canvas.setCursor(x + barWidth - inset, y + characterHeight - ascenderOffset);
  canvas.print(label);
  canvas.setCursor(x + barWidth - inset + ((characterWidth + characterSpacer) * 4) + spacer, y + characterHeight - ascenderOffset);
  canvas.print(units);
}

void renderActivity(GFXcanvas16 &canvas, DataPtr data) {
  const int16_t stride = 19;
  const int16_t start = 0;
  const int16_t indent = 0;

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

  canvas.fillScreen(ST77XX_BLUE);

  // TODO: These should be configurable somehow...
  const float networkScale = 1.0 * 1000.0 * 1000.0;  // 1 Mbps
  const float diskScale = 10.0 * 1024.0 * 1024.0;    // 10 MB/s

  /*
#define ST77XX_BLACK 0x0000
#define ST77XX_WHITE 0xFFFF
#define ST77XX_RED 0xF800
#define ST77XX_GREEN 0x07E0
#define ST77XX_BLUE 0x001F
#define ST77XX_CYAN 0x07FF
#define ST77XX_MAGENTA 0xF81F
#define ST77XX_YELLOW 0xFFE0
#define ST77XX_ORANGE 0xFC00
*/

// convert color RRRRRRRR GGGGGGGG BBBBBBBB (24-bit) to RRRRR GGGGGGG RRRRR (16-bit)
#define CONVERT_COLOR(color) ((int)((((color & 0xff0000) >> 16) / 255.0) * 31) << 11) | ((int)((((color & 0x00ff00) >> 8) / 255.0) * 63) << 5) | ((int)((((color & 0x0000ff) >> 0) / 255.0) * 31) << 0)

#define PROCESSOR_COLOR CONVERT_COLOR(0x097ddb)
#define UPLOAD_COLOR CONVERT_COLOR(0x00a802)
#define DOWNLOAD_COLOR CONVERT_COLOR(0xfc3c28)
#define READ_COLOR CONVERT_COLOR(0xf06b00)
#define WRITE_COLOR CONVERT_COLOR(0xffae00)

#define MEMORY_SYSTEM_COLOR CONVERT_COLOR(0x68fcea)
#define MEMORY_APP_COLOR CONVERT_COLOR(0x1ac8e8)
#define MEMORY_COMPRESSED_COLOR CONVERT_COLOR(0xcf72fe)

#define BACKGROUND_COLOR CONVERT_COLOR(0x333333)

#if 0
  canvas.fillScreen(ST77XX_BLACK);

  renderGraph(canvas, PROCESSOR_COLOR, cpuLabel, "%", indent, start + (stride * 0), 0.95);
  renderGraph(canvas, DOWNLOAD_COLOR, downloadLabel, downloadUnits, indent, start + (stride * 1), 1.0);
  renderGraph(canvas, UPLOAD_COLOR, uploadLabel, uploadUnits, indent, start + (stride * 2), 1.0);
  renderGraph(canvas, READ_COLOR, readLabel, readUnits, indent, start + (stride * 3), 1.0);
  renderGraph(canvas, WRITE_COLOR, writeLabel, writeUnits, indent, start + (stride * 4), 0.9);
  renderGraph(canvas, MEMORY_COMPRESSED_COLOR, memoryLabel, memoryUnits, indent, start + (stride * 5), 0.8);
#else
  canvas.fillScreen(ST77XX_BLACK);

  renderGraph(canvas, PROCESSOR_COLOR, cpuLabel, "%", indent, start + (stride * 0), data->cpuUsage);
  renderGraph(canvas, DOWNLOAD_COLOR, downloadLabel, downloadUnits, indent, start + (stride * 1), (float)(data->networkReceivedBps) / networkScale);
  renderGraph(canvas, UPLOAD_COLOR, uploadLabel, uploadUnits, indent, start + (stride * 2), (float)(data->networkSentBps) / networkScale);
  renderGraph(canvas, READ_COLOR, readLabel, readUnits, indent, start + (stride * 3), (float)(data->diskReadBytes) / diskScale);
  renderGraph(canvas, WRITE_COLOR, writeLabel, writeUnits, indent, start + (stride * 4), (float)(data->diskWriteBytes) / diskScale);
  renderGraph(canvas, MEMORY_COMPRESSED_COLOR, memoryLabel, memoryUnits, indent, start + (stride * 5), (float)memoryTotalBytes / (float)data->memoryPhysicalSize);
#endif

  int64_t bottomHeight = 135 - 5;

  canvas.fillRect(0, 135 - 21, 240, 21, BACKGROUND_COLOR);

  canvas.setCursor(indent, bottomHeight);
  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  if (daysUptime > 99.9) {
    uptimeDecimalPlaces = 0;
  }
  char uptimeLabel[5];
  snprintf(uptimeLabel, sizeof(uptimeLabel), "%4.*f", uptimeDecimalPlaces, daysUptime);
  canvas.print(uptimeLabel);
  canvas.setCursor(indent + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  canvas.print("days");

  canvas.setCursor(indent + 120, bottomHeight);
  int loadDecimalPlaces = 1;
  if (data->load > 99.9) {
    loadDecimalPlaces = 0;
  }
  char loadLabel[5];
  snprintf(loadLabel, sizeof(loadLabel), "%4.*f", loadDecimalPlaces, data->load);
  canvas.print(loadLabel);
  canvas.setCursor(indent + 120 + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  canvas.print("load");
}

void renderDisk(GFXcanvas16 &canvas, const char *name, const char *label, const char *units, int16_t x, int16_t y, float value) {
  if (value < 0.0) {
    value = 0.0;
  } else if (value > 1.0) {
    value = 1.0;
  }

  canvas.fillRoundRect(x, y, diskWidth * value, 7, 2, ST77XX_WHITE);

  canvas.setCursor(x + diskWidth + spacer, y);
  canvas.print(label);
  canvas.setCursor(x + diskWidth + spacer + ((characterWidth + characterSpacer) * 4) + spacer, y);
  canvas.print(units);

  canvas.setCursor(x + diskWidth + spacer + ((characterWidth + characterSpacer) * 4) + spacer + ((characterWidth + characterSpacer) * 2) + (spacer * 2), y);
  canvas.print(name);
}

void renderDisks(GFXcanvas16 &canvas, DataPtr data) {
  int16_t stride = 18;
  int16_t start = 0;
  int16_t indent = 0;

  canvas.fillScreen(ST77XX_BLACK);

  // TODO: These should be configurable somehow...
  const float networkScale = 1.0 * 1000.0 * 1000.0;  // 1 Mbps
  const float diskScale = 10.0 * 1024.0 * 1024.0;    // 10 MB/s

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

      renderDisk(canvas, diskLabel, sizeLabel, sizeUnits, indent, start + (stride * i), (float)usedBytes / (float)totalBytes);
    }
  }

  canvas.drawLine(0, 64 - 8 - 3, 128, 64 - 8 - 3, ST77XX_WHITE);

  int64_t bottomHeight = 64 - characterHeight - characterSpacer;

  canvas.drawBitmap(indent, bottomHeight, bitmapUptime, 7, 7, ST77XX_WHITE);
  canvas.setCursor(indent + bitmapDimension + spacer, bottomHeight);
  int uptimeDecimalPlaces = 1;
  float daysUptime = (float)data->uptime / 60.0 / 60.0 / 24.0;
  if (daysUptime > 99.9) {
    uptimeDecimalPlaces = 0;
  }
  char uptimeLabel[5];
  snprintf(uptimeLabel, sizeof(uptimeLabel), "%4.*f", uptimeDecimalPlaces, daysUptime);
  canvas.print(uptimeLabel);
  canvas.setCursor(indent + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  canvas.print("days");

  canvas.drawBitmap(indent + barWidth + spacer, bottomHeight, bitmapLoad, 7, 7, ST77XX_WHITE);
  canvas.setCursor(indent + barWidth + spacer + bitmapDimension + spacer, bottomHeight);
  int loadDecimalPlaces = 1;
  if (data->load > 99.9) {
    loadDecimalPlaces = 0;
  }
  char loadLabel[5];
  snprintf(loadLabel, sizeof(loadLabel), "%4.*f", loadDecimalPlaces, data->load);
  canvas.print(loadLabel);
  canvas.setCursor(indent + barWidth + spacer + bitmapDimension + spacer + ((characterWidth + characterSpacer) * 4) + spacer, bottomHeight);
  canvas.print("load");
}

void renderStart(GFXcanvas16 &canvas) {
  canvas.fillScreen(ST77XX_BLACK);

  canvas.setCursor(10, ((characterWidth + characterSpacer) * 3));
  canvas.print(" Start the iPulse");
  canvas.setCursor(10, ((characterWidth + characterSpacer) * 4) + (spacer * 1));
  canvas.print("daemon in Terminal");
  canvas.setCursor(10, ((characterWidth + characterSpacer) * 5) + (spacer * 2));
  canvas.print("   on your Mac.");
  canvas.drawRoundRect(0, 0, 128, 64, 10, ST77XX_WHITE);
}

void renderSleep(GFXcanvas16 &canvas) {
  int x = random(0, 128 - ((characterWidth + characterSpacer) * 3));
  int y = random(0, 64 - (characterHeight + characterSpacer));
  canvas.fillScreen(ST77XX_BLACK);
  canvas.setCursor(x, y);
  canvas.print("zzz");
}

#endif  // _Display_H_