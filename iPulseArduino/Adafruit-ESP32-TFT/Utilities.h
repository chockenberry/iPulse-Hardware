// Utilities.h

#ifndef _Utilities_H_
#define _Utilities_H_

float clampedValue(float value) {
  if (value < 0.0) {
    value = 0.0;
  } else if (value > 1.0) {
    value = 1.0;
  }
  return value;
}

void percentageLabel(float value, int decimalPlaces, char *label) {
  if (value > 0.99) {
    decimalPlaces = 0;
  }

  snprintf(label, 5, "%.*f", decimalPlaces, value * 100.0);
 }

void bitsPerSecondLabel(int64_t value, char *label, char *units) {
  if (value == 0) {
    strcpy(label, "-");
    strcpy(units, "bps");
  }
  else {
    int maxScale = (int)floorf(log10f(value));

    char *suffix = 0;
    float scalingPower = 0.0;
    int decimalPlaces = 0;

    if (maxScale >= 0 && maxScale < 3) {
      // 0 .. 1000
      strcpy(units, "bps");
      scalingPower = 0;
      decimalPlaces = 0;
    }
    else if (maxScale >= 3 && maxScale < 6) {
      // 1000 .. 1,000,000
      strcpy(units, "Kbps");
      scalingPower = 3;
      decimalPlaces = 0;
    }
    else if (maxScale >= 6 && maxScale < 9) {
      // 1,000,000 .. 1,000,000,000
      strcpy(units, "Mbps");
      scalingPower = 6;
      decimalPlaces = 0;
    }
    else {
      // over 1,000,000,000
      strcpy(units, "Gbps");
      scalingPower = 9;
      decimalPlaces = 1;
    }

    float scaling = powf(10, scalingPower);
    snprintf(label, 5, "%.*f", decimalPlaces, ((float)value / scaling));
  }
}

void bytesPerSecondLabel(int64_t value, char *label, char *units) {
  if (value == 0) {
    strcpy(label, "-");
    strcpy(units, "B/s");
  }
  else {
    int maxScale = (int)floorf(log2f(value));

    char *suffix = 0;
    float scalingPower = 0.0;
    int decimalPlaces = 0;

    if (maxScale >= 0 && maxScale < 10) {
      // 0 .. 1K
      strcpy(units, "B/s");
      scalingPower = 0;
      decimalPlaces = 0;
    }
    else if (maxScale >= 10 && maxScale < 20) {
      // 1K .. 1MB
      strcpy(units, "KB/s");
      scalingPower = 10;
      decimalPlaces = 0;
    }
    else if (maxScale >= 20 && maxScale < 30) {
      // 1MB .. 1GB
      strcpy(units, "MB/s");
      scalingPower = 20;
      decimalPlaces = 0;
    }
    else {
      // over 1GB
      strcpy(units, "GB/s");
      scalingPower = 30;
      decimalPlaces = 1;
    }

    float scaling = powf(2, scalingPower);
    snprintf(label, 5, "%.*f", decimalPlaces, ((float)value / scaling));
  }
}

void bytesLabel(int64_t value, char *label, char *units) {
  if (value == 0) {
    strcpy(label, "-");
    strcpy(units, "B");
  }
  else {
    int maxScale = (int)floorf(log2f(value));

    char *suffix = 0;
    float scalingPower = 0.0;
    int decimalPlaces = 0;

    if (maxScale >= 0 && maxScale < 10) {
      // 0 .. 1K
      strcpy(units, "B");
      scalingPower = 0;
      decimalPlaces = 0;
    }
    else if (maxScale >= 10 && maxScale < 20) {
      // 1K .. 1MB
      strcpy(units, "KB");
      scalingPower = 10;
      decimalPlaces = 0;
    }
    else if (maxScale >= 20 && maxScale < 30) {
      // 1MB .. 1GB
      strcpy(units, "MB");
      scalingPower = 20;
      decimalPlaces = 0;
    }
    else {
      // over 1GB
      strcpy(units, "GB");
      scalingPower = 30;
      decimalPlaces = 1;
    }

    float scaling = powf(2, scalingPower);
    snprintf(label, 5, "%.*f", decimalPlaces, ((float)value / scaling));
  }
}

void bytes10Label(int64_t value, char *label, char *units) {
  if (value == 0) {
    strcpy(label, "-");
    strcpy(units, "B");
  }
  else {
    int maxScale = (int)floorf(log10f(value));

    char *suffix = 0;
    float scalingPower = 0.0;
    int decimalPlaces = 0;

    if (maxScale >= 0 && maxScale < 3) {
      // 0 .. 1000
      strcpy(units, "B");
      scalingPower = 0;
    }
    else if (maxScale >= 3 && maxScale < 6) {
      // 1000 .. 1,000,000
      strcpy(units, "KB");
      scalingPower = 3;
    }
    else if (maxScale >= 6 && maxScale < 9) {
      // 1,000,000 .. 1,000,000,000
      strcpy(units, "MB");
      scalingPower = 6;
    }
    else if (maxScale >= 9 && maxScale < 12) {
      // over 1,000,000,000
      strcpy(units, "GB");
      scalingPower = 9;
    }
    else {
      // over 1,000,000,000,000
      strcpy(units, "TB");
      scalingPower = 12;
      decimalPlaces = 1;
    }

    float scaling = powf(10, scalingPower);
    snprintf(label, 5, "%.*f", decimalPlaces, ((float)value / scaling));
  }
}

#endif // _Utilities_H_