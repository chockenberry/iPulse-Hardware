const uint8_t fontBitmaps[] PROGMEM = {
  0x00, 0xFF, 0xFF, 0xF0, 0xF0, 0xCF, 0x3C, 0xF3, 0x33, 0x0C, 0xCF, 0xFF,
  0xFF, 0x33, 0x0C, 0xCF, 0xFF, 0xFF, 0x33, 0x0C, 0xC0, 0x0C, 0x03, 0x03,
  0xF0, 0xFC, 0xCC, 0xF3, 0x3C, 0xC3, 0x30, 0x3F, 0x0F, 0xC0, 0xCC, 0x33,
  0xCC, 0xF3, 0x33, 0xF0, 0xFC, 0x0C, 0x03, 0x00, 0x3F, 0xFF, 0x3F, 0xFF,
  0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x30, 0xC3, 0x30, 0x3C, 0xFC, 0x3C, 0xFC,
  0x03, 0xC3, 0x03, 0xC3, 0x0C, 0xC3, 0x0C, 0xC3, 0x30, 0x3C, 0x30, 0x3C,
  0x0F, 0x00, 0x3C, 0x03, 0x0C, 0x0C, 0x30, 0x33, 0x00, 0xCC, 0x00, 0xC0,
  0x03, 0x00, 0x33, 0x30, 0xCC, 0xCC, 0x0C, 0x30, 0x30, 0xC3, 0x33, 0x0C,
  0xC3, 0xC0, 0xCF, 0x03, 0xFF, 0x0C, 0x33, 0x0C, 0xC3, 0x0C, 0x30, 0xC3,
  0x0C, 0x30, 0xC3, 0x03, 0x0C, 0x0C, 0x30, 0xC3, 0x03, 0x0C, 0x0C, 0x30,
  0xC3, 0x0C, 0x30, 0xC3, 0x0C, 0x33, 0x0C, 0xC3, 0x00, 0x33, 0x0C, 0xC0,
  0xC0, 0x30, 0xFF, 0xFF, 0xF0, 0xC0, 0x30, 0x33, 0x0C, 0xC0, 0x0C, 0x03,
  0x00, 0xC0, 0x30, 0xFF, 0xFF, 0xF0, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0x33,
  0x33, 0xCC, 0xFF, 0xFF, 0xF0, 0x03, 0x03, 0x03, 0x03, 0x0C, 0x0C, 0x0C,
  0x0C, 0x30, 0x30, 0x30, 0x30, 0xC0, 0xC0, 0xC0, 0xC0, 0x3F, 0x0F, 0xCC,
  0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0x0F, 0x03,
  0x3F, 0x0F, 0xC0, 0x33, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x0F,
  0xCC, 0x0F, 0x03, 0x00, 0xC0, 0x30, 0x30, 0x0C, 0x0C, 0x03, 0x03, 0x00,
  0xC0, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xF0, 0x30, 0x0C, 0x0C, 0x03, 0x03,
  0xF0, 0xFC, 0x00, 0xC0, 0x3C, 0x0F, 0x03, 0x3F, 0x0F, 0xC0, 0x03, 0x00,
  0xC0, 0xF0, 0x3C, 0x33, 0x0C, 0xCC, 0x33, 0x0C, 0xFF, 0xFF, 0xF0, 0x30,
  0x0C, 0x03, 0x00, 0xC0, 0xFF, 0xFF, 0xFC, 0x03, 0x00, 0xFF, 0x3F, 0xC0,
  0x0C, 0x03, 0x00, 0xC0, 0x3C, 0x0F, 0x03, 0x3F, 0x0F, 0xC0, 0x0F, 0x03,
  0xC3, 0x00, 0xC0, 0xC0, 0x30, 0x0F, 0xF3, 0xFC, 0xC0, 0xF0, 0x3C, 0x0F,
  0x03, 0x3F, 0x0F, 0xC0, 0xFF, 0xFF, 0xF0, 0x0C, 0x03, 0x03, 0x00, 0xC0,
  0x30, 0x0C, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0x3F, 0x0F,
  0xCC, 0x0F, 0x03, 0xC0, 0xF0, 0x33, 0xF0, 0xFC, 0xC0, 0xF0, 0x3C, 0x0F,
  0x03, 0x3F, 0x0F, 0xC0, 0x3F, 0x0F, 0xCC, 0x0F, 0x03, 0xC0, 0xF0, 0x33,
  0xFC, 0xFF, 0x00, 0xC0, 0x30, 0x30, 0x0C, 0x3C, 0x0F, 0x00, 0xF0, 0x0F,
  0x33, 0x00, 0x00, 0x33, 0x33, 0xCC, 0x0C, 0x33, 0x0C, 0xC3, 0x03, 0x0C,
  0x0C, 0x30, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xF0, 0xC3, 0x03,
  0x0C, 0x0C, 0x33, 0x0C, 0xC3, 0x00, 0x3C, 0x3C, 0xC3, 0xC3, 0x03, 0x03,
  0x0C, 0x0C, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x0F, 0xC0, 0x3F, 0x03,
  0x03, 0x0C, 0x0C, 0xC3, 0xCF, 0x0F, 0x3C, 0xCC, 0xF3, 0x33, 0xCC, 0xCF,
  0x33, 0x3C, 0x3F, 0x30, 0xFC, 0x30, 0x00, 0xC0, 0x00, 0xFC, 0x03, 0xF0,
  0x0C, 0x03, 0x00, 0xC0, 0x30, 0x33, 0x0C, 0xC3, 0x30, 0xCC, 0xFF, 0xFF,
  0xFC, 0x0F, 0x03, 0xC0, 0xF0, 0x30, 0xFF, 0x3F, 0xCC, 0x0F, 0x03, 0xC0,
  0xF0, 0x3F, 0xF3, 0xFC, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xFF, 0x3F, 0xC0,
  0x3F, 0x0F, 0xCC, 0x0F, 0x03, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30,
  0x0C, 0x0F, 0x03, 0x3F, 0x0F, 0xC0, 0xFC, 0x3F, 0x0C, 0x33, 0x0C, 0xC0,
  0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0x33, 0x0C, 0xFC, 0x3F, 0x00,
  0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0,
  0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC0, 0xC0,
  0xC0, 0xC0, 0xC0, 0xC0, 0x3F, 0x0F, 0xCC, 0x0F, 0x03, 0xC0, 0x30, 0x0C,
  0x3F, 0x0F, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x3F, 0x0F, 0xC0, 0xC0, 0xF0,
  0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x3F, 0xFF, 0xFF, 0xC0, 0xF0, 0x3C, 0x0F,
  0x03, 0xC0, 0xF0, 0x30, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xC0, 0x30, 0x0C,
  0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x3F,
  0x0F, 0xC0, 0xC0, 0xF0, 0x3C, 0x33, 0x0C, 0xCC, 0x33, 0x0F, 0x03, 0xC0,
  0xCC, 0x33, 0x0C, 0x33, 0x0C, 0xC0, 0xF0, 0x30, 0xC0, 0xC0, 0xC0, 0xC0,
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0x0F,
  0x00, 0x3F, 0x03, 0xFC, 0x0F, 0xCC, 0xCF, 0x33, 0x3C, 0x30, 0xF0, 0xC3,
  0xC0, 0x0F, 0x00, 0x3C, 0x00, 0xF0, 0x03, 0xC0, 0x0F, 0x00, 0x30, 0xF0,
  0xFC, 0x3F, 0x0F, 0xC3, 0xCC, 0xF3, 0x3C, 0xCF, 0x33, 0xC3, 0xF0, 0xFC,
  0x3F, 0x0F, 0xC0, 0xF0, 0x30, 0x3F, 0x0F, 0xCC, 0x0F, 0x03, 0xC0, 0xF0,
  0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x3F, 0x0F, 0xC0, 0xFF,
  0x3F, 0xCC, 0x0F, 0x03, 0xC0, 0xF0, 0x3F, 0xF3, 0xFC, 0xC0, 0x30, 0x0C,
  0x03, 0x00, 0xC0, 0x30, 0x00, 0x3F, 0x0F, 0xCC, 0x0F, 0x03, 0xC0, 0xF0,
  0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0xCF, 0x33, 0x3F, 0x0F, 0xC0, 0x30,
  0x0C, 0xFF, 0x3F, 0xCC, 0x0F, 0x03, 0xC0, 0xF0, 0x3F, 0xF3, 0xFC, 0xCC,
  0x33, 0x0C, 0x33, 0x0C, 0xC0, 0xF0, 0x30, 0x3F, 0x0F, 0xCC, 0x0F, 0x03,
  0xC0, 0x30, 0x03, 0xF0, 0xFC, 0x00, 0xC0, 0x3C, 0x0F, 0x03, 0x3F, 0x0F,
  0xC0, 0xFF, 0xFF, 0xF0, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C,
  0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0xF0, 0x3C, 0x0F, 0x03,
  0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x3F, 0x0F,
  0xC0, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0xC0, 0xF0, 0x33, 0x30, 0xCC, 0x33,
  0x0C, 0xC0, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x0F, 0x00, 0x3C, 0x00,
  0xF0, 0x03, 0x33, 0x30, 0xCC, 0xC3, 0x33, 0x0C, 0xCC, 0x0C, 0xC0, 0x33,
  0x00, 0xCC, 0x03, 0x30, 0x0C, 0xC0, 0x33, 0x00, 0xC0, 0xF0, 0x3C, 0x0F,
  0x03, 0x33, 0x0C, 0xC0, 0xC0, 0x30, 0x33, 0x0C, 0xCC, 0x0F, 0x03, 0xC0,
  0xF0, 0x30, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x33, 0x0C, 0xC0, 0xC0, 0x30,
  0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xFF, 0xFF, 0x03, 0x03,
  0x0C, 0x0C, 0x30, 0x30, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 0xCC,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0x30,
  0x30, 0x30, 0x30, 0x0C, 0x0C, 0x0C, 0x0C, 0x03, 0x03, 0x03, 0x03, 0xFF,
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xFF, 0x30, 0xCC, 0xF3, 0xFF,
  0xFF, 0xFF, 0xCC, 0x33, 0x3F, 0x3F, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
  0x3F, 0x3F, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC, 0xC3, 0xC3, 0xC3, 0xC3,
  0xC3, 0xC3, 0xFC, 0xFC, 0x3C, 0x3C, 0xC3, 0xC3, 0xC0, 0xC0, 0xC3, 0xC3,
  0x3C, 0x3C, 0x03, 0x03, 0x03, 0x03, 0x3F, 0x3F, 0xC3, 0xC3, 0xC3, 0xC3,
  0xC3, 0xC3, 0x3F, 0x3F, 0x3C, 0x3C, 0xC3, 0xC3, 0xFF, 0xFF, 0xC0, 0xC0,
  0x3C, 0x3C, 0x0F, 0x0F, 0x30, 0x30, 0xFC, 0xFC, 0x30, 0x30, 0x30, 0x30,
  0x30, 0x30, 0x30, 0x30, 0x3C, 0x3C, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
  0x3F, 0x3F, 0x03, 0x03, 0x3C, 0x3C, 0xC0, 0xC0, 0xC0, 0xC0, 0xFC, 0xFC,
  0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x33, 0x00, 0xFF, 0x33,
  0x33, 0x33, 0x33, 0x0C, 0x30, 0x00, 0x0C, 0x30, 0xC3, 0x0C, 0x30, 0xC3,
  0x0C, 0x30, 0xC3, 0xF3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC3, 0xC3, 0xCC,
  0xCC, 0xF0, 0xF0, 0xCC, 0xCC, 0xC3, 0xC3, 0xFF, 0xFF, 0xFF, 0xF0, 0xFC,
  0xF3, 0xF3, 0xCC, 0x30, 0xF0, 0xC3, 0xC3, 0x0F, 0x0C, 0x3C, 0x30, 0xF0,
  0xC3, 0xC3, 0x0F, 0x0C, 0x30, 0xFC, 0xFC, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
  0xC3, 0xC3, 0xC3, 0x3C, 0x3C, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x3C,
  0x3C, 0xFC, 0xFC, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFC, 0xFC, 0xC0,
  0xC0, 0xC0, 0xC0, 0x3C, 0x3C, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x3F,
  0x3F, 0x03, 0x03, 0x03, 0x03, 0xCF, 0xCF, 0xF0, 0xF0, 0xC0, 0xC0, 0xC0,
  0xC0, 0xC0, 0xC0, 0x3F, 0x3F, 0xC0, 0xC0, 0x3C, 0x3C, 0x03, 0x03, 0xFC,
  0xFC, 0x30, 0xC3, 0x0C, 0xFF, 0xF3, 0x0C, 0x30, 0xC3, 0x0C, 0x0C, 0x30,
  0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x3F, 0x3F, 0xC0, 0xF0,
  0x33, 0x30, 0xCC, 0x33, 0x0C, 0xC0, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0,
  0x0F, 0x00, 0x33, 0x33, 0x0C, 0xCC, 0x33, 0x30, 0xCC, 0xC0, 0xCC, 0x03,
  0x30, 0x0C, 0xC0, 0x33, 0x00, 0xC0, 0xF0, 0x33, 0x30, 0xCC, 0x0C, 0x03,
  0x03, 0x30, 0xCC, 0xC0, 0xF0, 0x30, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
  0xC3, 0xC3, 0x3F, 0x3F, 0x03, 0x03, 0x3C, 0x3C, 0xFF, 0xFF, 0xF0, 0x30,
  0x0C, 0x0C, 0x03, 0x03, 0x00, 0xC0, 0xFF, 0xFF, 0xF0, 0x0C, 0x33, 0x0C,
  0x30, 0xC3, 0x0C, 0xC3, 0x03, 0x0C, 0x30, 0xC3, 0x0C, 0x0C, 0x30, 0xFF,
  0xFF, 0xFF, 0xFF, 0xF0, 0xC3, 0x03, 0x0C, 0x30, 0xC3, 0x0C, 0x0C, 0x33,
  0x0C, 0x30, 0xC3, 0x0C, 0xC3, 0x00, 0x3C, 0xCF, 0x3C, 0xF3, 0x3C };

const GFXglyph fontGlyphs[] PROGMEM = {
  {     0,   1,   1,   6,    0,    0 },   // 0x20 ' '
  {     1,   2,  14,   4,    0,  -13 },   // 0x21 '!'
  {     5,   6,   4,   8,    0,  -13 },   // 0x22 '"'
  {     8,  10,  10,  12,    0,  -15 },   // 0x23 '#'
  {    21,  10,  18,  12,    0,  -15 },   // 0x24 '$'
  {    44,  16,  14,  18,    0,  -13 },   // 0x25 '%'
  {    72,  14,  16,  16,    0,  -15 },   // 0x26 '&'
  {   100,   2,   4,   4,    0,  -13 },   // 0x27 '''
  {   101,   6,  18,   8,    0,  -15 },   // 0x28 '('
  {   115,   6,  18,   8,    0,  -15 },   // 0x29 ')'
  {   129,  10,  10,  12,    0,  -13 },   // 0x2A '*'
  {   142,  10,  10,  12,    0,  -11 },   // 0x2B '+'
  {   155,   4,   6,   6,    0,   -1 },   // 0x2C ','
  {   158,   8,   2,  10,    0,   -7 },   // 0x2D '-'
  {   160,   2,   2,   4,    0,   -1 },   // 0x2E '.'
  {   161,   8,  16,  10,    0,  -15 },   // 0x2F '/'
  {   177,  10,  14,  12,    0,  -13 },   // 0x30 '0'
  {   195,   4,  14,   6,    0,  -13 },   // 0x31 '1'
  {   202,  10,  14,  12,    0,  -13 },   // 0x32 '2'
  {   220,  10,  14,  12,    0,  -13 },   // 0x33 '3'
  {   238,  10,  14,  12,    0,  -13 },   // 0x34 '4'
  {   256,  10,  14,  12,    0,  -13 },   // 0x35 '5'
  {   274,  10,  14,  12,    0,  -13 },   // 0x36 '6'
  {   292,  10,  14,  12,    0,  -13 },   // 0x37 '7'
  {   310,  10,  14,  12,    0,  -13 },   // 0x38 '8'
  {   328,  10,  14,  12,    0,  -13 },   // 0x39 '9'
  {   346,   2,   8,   4,    0,   -7 },   // 0x3A ':'
  {   348,   4,  12,   6,    0,   -7 },   // 0x3B ';'
  {   354,   6,  10,   8,    0,  -11 },   // 0x3C '<'
  {   362,  10,   6,  12,    0,   -9 },   // 0x3D '='
  {   370,   6,  10,   8,    0,  -11 },   // 0x3E '>'
  {   378,   8,  14,  10,    0,  -13 },   // 0x3F '?'
  {   392,  14,  16,  16,    0,  -13 },   // 0x40 '@'
  {   420,  10,  14,  12,    0,  -13 },   // 0x41 'A'
  {   438,  10,  14,  12,    0,  -13 },   // 0x42 'B'
  {   456,  10,  14,  12,    0,  -13 },   // 0x43 'C'
  {   474,  10,  14,  12,    0,  -13 },   // 0x44 'D'
  {   492,   8,  14,  10,    0,  -13 },   // 0x45 'E'
  {   506,   8,  14,  10,    0,  -13 },   // 0x46 'F'
  {   520,  10,  14,  12,    0,  -13 },   // 0x47 'G'
  {   538,  10,  14,  12,    0,  -13 },   // 0x48 'H'
  {   556,   2,  14,   4,    0,  -13 },   // 0x49 'I'
  {   560,  10,  14,  12,    0,  -13 },   // 0x4A 'J'
  {   578,  10,  14,  12,    0,  -13 },   // 0x4B 'K'
  {   596,   8,  14,  10,    0,  -13 },   // 0x4C 'L'
  {   610,  14,  14,  16,    0,  -13 },   // 0x4D 'M'
  {   635,  10,  14,  12,    0,  -13 },   // 0x4E 'N'
  {   653,  10,  14,  12,    0,  -13 },   // 0x4F 'O'
  {   671,  10,  14,  12,    0,  -13 },   // 0x50 'P'
  {   689,  10,  16,  12,    0,  -13 },   // 0x51 'Q'
  {   709,  10,  14,  12,    0,  -13 },   // 0x52 'R'
  {   727,  10,  14,  12,    0,  -13 },   // 0x53 'S'
  {   745,  10,  14,  12,    0,  -13 },   // 0x54 'T'
  {   763,  10,  14,  12,    0,  -13 },   // 0x55 'U'
  {   781,  10,  14,  12,    0,  -13 },   // 0x56 'V'
  {   799,  14,  14,  16,    0,  -13 },   // 0x57 'W'
  {   824,  10,  14,  12,    0,  -13 },   // 0x58 'X'
  {   842,  10,  14,  12,    0,  -13 },   // 0x59 'Y'
  {   860,   8,  14,  10,    0,  -13 },   // 0x5A 'Z'
  {   874,   4,  18,   6,    0,  -15 },   // 0x5B '['
  {   883,   8,  16,  10,    0,  -15 },   // 0x5C '\'
  {   899,   4,  18,   6,    0,  -15 },   // 0x5D ']'
  {   908,   6,   4,   8,    0,  -13 },   // 0x5E '^'
  {   911,  12,   2,  14,    0,   -1 },   // 0x5F '_'
  {   914,   4,   4,   6,    0,  -13 },   // 0x60 '`'
  {   916,   8,  10,  10,    0,   -9 },   // 0x61 'a'
  {   926,   8,  14,  10,    0,  -13 },   // 0x62 'b'
  {   940,   8,  10,  10,    0,   -9 },   // 0x63 'c'
  {   950,   8,  14,  10,    0,  -13 },   // 0x64 'd'
  {   964,   8,  10,  10,    0,   -9 },   // 0x65 'e'
  {   974,   8,  14,  10,    0,  -13 },   // 0x66 'f'
  {   988,   8,  14,  10,    0,   -9 },   // 0x67 'g'
  {  1002,   8,  14,  10,    0,  -13 },   // 0x68 'h'
  {  1016,   4,  14,   6,    0,  -13 },   // 0x69 'i'
  {  1023,   6,  18,   8,    0,  -13 },   // 0x6A 'j'
  {  1037,   8,  14,  10,    0,  -13 },   // 0x6B 'k'
  {  1051,   2,  14,   4,    0,  -13 },   // 0x6C 'l'
  {  1055,  14,  10,  16,    0,   -9 },   // 0x6D 'm'
  {  1073,   8,  10,  10,    0,   -9 },   // 0x6E 'n'
  {  1083,   8,  10,  10,    0,   -9 },   // 0x6F 'o'
  {  1093,   8,  14,  10,    0,   -9 },   // 0x70 'p'
  {  1107,   8,  14,  10,    0,   -9 },   // 0x71 'q'
  {  1121,   8,  10,  10,    0,   -9 },   // 0x72 'r'
  {  1131,   8,  10,  10,    0,   -9 },   // 0x73 's'
  {  1141,   6,  14,   8,    0,  -13 },   // 0x74 't'
  {  1152,   8,  10,  10,    0,   -9 },   // 0x75 'u'
  {  1162,  10,  10,  12,    0,   -9 },   // 0x76 'v'
  {  1175,  14,  10,  16,    0,   -9 },   // 0x77 'w'
  {  1193,  10,  10,  12,    0,   -9 },   // 0x78 'x'
  {  1206,   8,  14,  10,    0,   -9 },   // 0x79 'y'
  {  1220,  10,  10,  12,    0,   -9 },   // 0x7A 'z'
  {  1233,   6,  18,   8,    0,  -15 },   // 0x7B '{'
  {  1247,   2,  18,   4,    0,  -15 },   // 0x7C '|'
  {  1252,   6,  18,   8,    0,  -15 },   // 0x7D '}'
  {  1266,  10,   4,  12,    0,  -13 } }; // 0x7E '~'

const GFXfont font PROGMEM = {
  (uint8_t  *)fontBitmaps,
  (GFXglyph *)fontGlyphs,
  0x20, 0x7E, 28 };

// Approx. 1943 bytes
