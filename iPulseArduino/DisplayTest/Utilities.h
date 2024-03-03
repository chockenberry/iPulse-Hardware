// Utilities.h

#ifndef _Utilities_H_
#define _Utilities_H_

void percentageLabel(float value, char *label) {
  snprintf(label, 5, "%4d", (int16_t)(value * 100.0));
}

void bitsPerSecondLabel(int64_t value, char *label, char *units) {
  snprintf(label, 5, "%4d", (int16_t)(value / 1000));
  strcpy(units, "Kbps");
}

#endif // _Utilities_H_