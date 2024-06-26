const uint8_t fontBitmaps[] PROGMEM = {
  0x00, 0xFA, 0xB4, 0x57, 0xD5, 0xF5, 0x00, 0x23, 0xAB, 0x47, 0x16, 0xAE,
  0x20, 0x7F, 0x92, 0x94, 0x6E, 0x19, 0x29, 0x46, 0x30, 0x91, 0x41, 0x05,
  0x51, 0x25, 0x31, 0xC0, 0x2A, 0x49, 0x22, 0x20, 0x88, 0x92, 0x4A, 0x80,
  0x51, 0x3E, 0x45, 0x00, 0x21, 0x3E, 0x42, 0x00, 0x58, 0xF0, 0x80, 0x11,
  0x22, 0x44, 0x88, 0x74, 0x63, 0x18, 0xC5, 0xC0, 0x75, 0x54, 0x74, 0x42,
  0x22, 0x23, 0xE0, 0xF8, 0x88, 0xE0, 0xC5, 0xC0, 0x11, 0x95, 0x2F, 0x88,
  0x40, 0xFC, 0x3C, 0x10, 0xC5, 0xC0, 0x32, 0x21, 0xE8, 0xC5, 0xC0, 0xF8,
  0x44, 0x22, 0x10, 0x80, 0x74, 0x62, 0xE8, 0xC5, 0xC0, 0x74, 0x62, 0xF0,
  0x89, 0x80, 0x90, 0x41, 0x60, 0x2A, 0x22, 0xF8, 0x3E, 0x88, 0xA8, 0x69,
  0x12, 0x40, 0x40, 0x38, 0x8A, 0x6D, 0x5A, 0xB3, 0x90, 0x1C, 0x21, 0x14,
  0xAF, 0xC6, 0x20, 0xF4, 0x63, 0xE8, 0xC7, 0xC0, 0x74, 0x61, 0x08, 0x45,
  0xC0, 0xE4, 0xA3, 0x18, 0xCB, 0x80, 0xF8, 0x8E, 0x88, 0xF0, 0xF8, 0x8E,
  0x88, 0x80, 0x74, 0x61, 0x38, 0xC5, 0xC0, 0x8C, 0x63, 0xF8, 0xC6, 0x20,
  0xFE, 0x08, 0x42, 0x18, 0xC5, 0xC0, 0x8C, 0xA9, 0x8A, 0x4A, 0x20, 0x88,
  0x88, 0x88, 0xF0, 0x83, 0x8E, 0xAC, 0x98, 0x30, 0x60, 0x80, 0xCE, 0x6B,
  0x59, 0xCE, 0x20, 0x74, 0x63, 0x18, 0xC5, 0xC0, 0xF4, 0x63, 0xE8, 0x42,
  0x00, 0x74, 0x63, 0x18, 0xD5, 0xC2, 0xF4, 0x63, 0xEA, 0x4A, 0x20, 0x74,
  0x60, 0xE0, 0xC5, 0xC0, 0xF9, 0x08, 0x42, 0x10, 0x80, 0x8C, 0x63, 0x18,
  0xC5, 0xC0, 0x8C, 0x62, 0xA5, 0x10, 0x80, 0x83, 0x05, 0x52, 0xA2, 0x85,
  0x0A, 0x00, 0x8C, 0x54, 0x45, 0x46, 0x20, 0x8C, 0x54, 0x42, 0x10, 0x80,
  0xF1, 0x24, 0x88, 0xF0, 0xEA, 0xAA, 0xC0, 0x88, 0x44, 0x22, 0x11, 0xD5,
  0x55, 0xC0, 0x54, 0xFC, 0x90, 0x79, 0x99, 0x70, 0x88, 0xE9, 0x99, 0xE0,
  0x69, 0x89, 0x60, 0x11, 0x79, 0x99, 0x70, 0x69, 0xF8, 0x60, 0x34, 0xE4,
  0x44, 0x40, 0x69, 0x99, 0x71, 0x60, 0x88, 0xE9, 0x99, 0x90, 0x4D, 0x54,
  0x20, 0x92, 0x49, 0xC0, 0x88, 0x9A, 0xCA, 0x90, 0xFE, 0xED, 0x26, 0x4C,
  0x99, 0x20, 0xE9, 0x99, 0x90, 0x69, 0x99, 0x60, 0xE9, 0x99, 0xE8, 0x80,
  0x69, 0x99, 0x71, 0x10, 0xBC, 0x88, 0x80, 0x78, 0x61, 0xE0, 0x4B, 0xA4,
  0x88, 0x99, 0x99, 0x70, 0x8A, 0x94, 0x42, 0x00, 0x82, 0xA9, 0x51, 0x42,
  0x80, 0x8A, 0x88, 0xA8, 0x80, 0x99, 0x99, 0x71, 0x60, 0xF8, 0x88, 0x8F,
  0x80, 0x29, 0x28, 0x92, 0x20, 0xFF, 0x80, 0x89, 0x22, 0x92, 0x80, 0x6D,
  0x80 };

const GFXglyph fontGlyphs[] PROGMEM = {
  {     0,   1,   1,   5,    0,    0 },   // 0x20 ' '
  {     1,   1,   7,   2,    0,   -6 },   // 0x21 '!'
  {     2,   3,   2,   4,    0,   -6 },   // 0x22 '"'
  {     3,   5,   5,   6,    0,   -7 },   // 0x23 '#'
  {     7,   5,   9,   6,    0,   -7 },   // 0x24 '$'
  {    13,   8,   7,   9,    0,   -6 },   // 0x25 '%'
  {    20,   7,   8,   8,    0,   -7 },   // 0x26 '&'
  {    27,   1,   2,   2,    0,   -6 },   // 0x27 '''
  {    28,   3,   9,   4,    0,   -7 },   // 0x28 '('
  {    32,   3,   9,   4,    0,   -7 },   // 0x29 ')'
  {    36,   5,   5,   6,    0,   -6 },   // 0x2A '*'
  {    40,   5,   5,   6,    0,   -5 },   // 0x2B '+'
  {    44,   2,   3,   3,    0,    0 },   // 0x2C ','
  {    45,   4,   1,   5,    0,   -3 },   // 0x2D '-'
  {    46,   1,   1,   2,    0,    0 },   // 0x2E '.'
  {    47,   4,   8,   5,    0,   -7 },   // 0x2F '/'
  {    51,   5,   7,   6,    0,   -6 },   // 0x30 '0'
  {    56,   2,   7,   3,    0,   -6 },   // 0x31 '1'
  {    58,   5,   7,   6,    0,   -6 },   // 0x32 '2'
  {    63,   5,   7,   6,    0,   -6 },   // 0x33 '3'
  {    68,   5,   7,   6,    0,   -6 },   // 0x34 '4'
  {    73,   5,   7,   6,    0,   -6 },   // 0x35 '5'
  {    78,   5,   7,   6,    0,   -6 },   // 0x36 '6'
  {    83,   5,   7,   6,    0,   -6 },   // 0x37 '7'
  {    88,   5,   7,   7,    1,   -6 },   // 0x38 '8'
  {    93,   5,   7,   6,    0,   -6 },   // 0x39 '9'
  {    98,   1,   4,   2,    0,   -3 },   // 0x3A ':'
  {    99,   2,   6,   3,    0,   -3 },   // 0x3B ';'
  {   101,   3,   5,   4,    0,   -5 },   // 0x3C '<'
  {   103,   5,   3,   6,    0,   -4 },   // 0x3D '='
  {   105,   3,   5,   4,    0,   -5 },   // 0x3E '>'
  {   107,   4,   7,   5,    0,   -6 },   // 0x3F '?'
  {   111,   7,   8,   8,    0,   -6 },   // 0x40 '@'
  {   118,   5,   7,   6,    0,   -6 },   // 0x41 'A'
  {   123,   5,   7,   6,    0,   -6 },   // 0x42 'B'
  {   128,   5,   7,   6,    0,   -6 },   // 0x43 'C'
  {   133,   5,   7,   6,    0,   -6 },   // 0x44 'D'
  {   138,   4,   7,   5,    0,   -6 },   // 0x45 'E'
  {   142,   4,   7,   5,    0,   -6 },   // 0x46 'F'
  {   146,   5,   7,   6,    0,   -6 },   // 0x47 'G'
  {   151,   5,   7,   6,    0,   -6 },   // 0x48 'H'
  {   156,   1,   7,   2,    0,   -6 },   // 0x49 'I'
  {   157,   5,   7,   6,    0,   -6 },   // 0x4A 'J'
  {   162,   5,   7,   6,    0,   -6 },   // 0x4B 'K'
  {   167,   4,   7,   5,    0,   -6 },   // 0x4C 'L'
  {   171,   7,   7,   8,    0,   -6 },   // 0x4D 'M'
  {   178,   5,   7,   6,    0,   -6 },   // 0x4E 'N'
  {   183,   5,   7,   6,    0,   -6 },   // 0x4F 'O'
  {   188,   5,   7,   6,    0,   -6 },   // 0x50 'P'
  {   193,   5,   8,   6,    0,   -6 },   // 0x51 'Q'
  {   198,   5,   7,   6,    0,   -6 },   // 0x52 'R'
  {   203,   5,   7,   6,    0,   -6 },   // 0x53 'S'
  {   208,   5,   7,   6,    0,   -6 },   // 0x54 'T'
  {   213,   5,   7,   6,    0,   -6 },   // 0x55 'U'
  {   218,   5,   7,   6,    0,   -6 },   // 0x56 'V'
  {   223,   7,   7,   8,    0,   -6 },   // 0x57 'W'
  {   230,   5,   7,   6,    0,   -6 },   // 0x58 'X'
  {   235,   5,   7,   6,    0,   -6 },   // 0x59 'Y'
  {   240,   4,   7,   5,    0,   -6 },   // 0x5A 'Z'
  {   244,   2,   9,   3,    0,   -7 },   // 0x5B '['
  {   247,   4,   8,   5,    0,   -7 },   // 0x5C '\'
  {   251,   2,   9,   3,    0,   -7 },   // 0x5D ']'
  {   254,   3,   2,   4,    0,   -6 },   // 0x5E '^'
  {   255,   6,   1,   7,    0,    0 },   // 0x5F '_'
  {   256,   2,   2,   3,    0,   -6 },   // 0x60 '`'
  {   257,   4,   5,   5,    0,   -4 },   // 0x61 'a'
  {   260,   4,   7,   5,    0,   -6 },   // 0x62 'b'
  {   264,   4,   5,   5,    0,   -4 },   // 0x63 'c'
  {   267,   4,   7,   5,    0,   -6 },   // 0x64 'd'
  {   271,   4,   5,   5,    0,   -4 },   // 0x65 'e'
  {   274,   4,   7,   5,    0,   -6 },   // 0x66 'f'
  {   278,   4,   7,   5,    0,   -4 },   // 0x67 'g'
  {   282,   4,   7,   5,    0,   -6 },   // 0x68 'h'
  {   286,   2,   7,   3,    0,   -6 },   // 0x69 'i'
  {   288,   3,   9,   4,    0,   -6 },   // 0x6A 'j'
  {   292,   4,   7,   5,    0,   -6 },   // 0x6B 'k'
  {   296,   1,   7,   2,    0,   -6 },   // 0x6C 'l'
  {   297,   7,   5,   8,    0,   -4 },   // 0x6D 'm'
  {   302,   4,   5,   5,    0,   -4 },   // 0x6E 'n'
  {   305,   4,   5,   5,    0,   -4 },   // 0x6F 'o'
  {   308,   4,   7,   5,    0,   -4 },   // 0x70 'p'
  {   312,   4,   7,   5,    0,   -4 },   // 0x71 'q'
  {   316,   4,   5,   5,    0,   -4 },   // 0x72 'r'
  {   319,   4,   5,   5,    0,   -4 },   // 0x73 's'
  {   322,   3,   7,   4,    0,   -6 },   // 0x74 't'
  {   325,   4,   5,   5,    0,   -4 },   // 0x75 'u'
  {   328,   5,   5,   6,    0,   -4 },   // 0x76 'v'
  {   332,   7,   5,   8,    0,   -4 },   // 0x77 'w'
  {   337,   5,   5,   6,    0,   -4 },   // 0x78 'x'
  {   341,   4,   7,   5,    0,   -4 },   // 0x79 'y'
  {   345,   5,   5,   6,    0,   -4 },   // 0x7A 'z'
  {   349,   3,   9,   4,    0,   -7 },   // 0x7B '{'
  {   353,   1,   9,   2,    0,   -7 },   // 0x7C '|'
  {   355,   3,   9,   4,    0,   -7 },   // 0x7D '}'
  {   359,   5,   2,   6,    0,   -6 } }; // 0x7E '~'

const GFXfont font PROGMEM = {
  (uint8_t  *)fontBitmaps,
  (GFXglyph *)fontGlyphs,
  0x20, 0x7E, 14 };

// Approx. 1033 bytes
