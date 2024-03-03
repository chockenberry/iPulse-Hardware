// Bitmaps.h

#ifndef _Bitmaps_H_
#define _Bitmaps_H_

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

#endif // _Bitmaps_H_