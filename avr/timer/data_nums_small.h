#define NUM_SML_XSIZE 	6
#define NUM_SML_YSIZE 	32

// Small digit 0 (32x48)
const unsigned char num_sml_0[] PROGMEM = {
	0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF,
	0xFF, 0xF0, 0x00, 0x00, 0x0F, 0xFF,
	0xFF, 0x80, 0x00, 0x00, 0x01, 0xFF,
	0xFE, 0x00, 0x00, 0x00, 0x00, 0x7F,
	0xFC, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F,
	0xE0, 0x00, 0x7F, 0xFE, 0x00, 0x07,
	0xE0, 0x0F, 0xFF, 0xFF, 0xF0, 0x07,
	0xC0, 0x3F, 0xFF, 0xFF, 0xFC, 0x03,
	0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x01,
	0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0x81,
	0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1,
	0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
	0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
	0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1,
	0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0x81,
	0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x01,
	0xC0, 0x3F, 0xFF, 0xFF, 0xFC, 0x03,
	0xE0, 0x0F, 0xFF, 0xFF, 0xF0, 0x07,
	0xE0, 0x00, 0xFF, 0xFF, 0x00, 0x07,
	0xF0, 0x00, 0x00, 0x00, 0x00, 0x0F,
	0xFC, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0xFE, 0x00, 0x00, 0x00, 0x00, 0x7F,
	0xFF, 0x80, 0x00, 0x00, 0x01, 0xFF,
	0xFF, 0xF0, 0x00, 0x00, 0x0F, 0xFF,
	0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF
};

// Small digit 1 (32x48)
const unsigned char num_sml_1[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0xF0, 0x7F, 0xFF, 0xFF, 0xFF, 0xE0,
	0xF0, 0x7F, 0xFF, 0xFF, 0xFF, 0xE0,
	0xF0, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// Small digit 2 (32x48)
const unsigned char num_sml_2[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xE0, 0x3F, 0xFF, 0xFF, 0xE0,
	0xFF, 0x00, 0x07, 0xFF, 0xFF, 0xE0,
	0xFE, 0x00, 0x01, 0xFF, 0xFF, 0xE0,
	0xF8, 0x00, 0x00, 0xFF, 0xFF, 0xE0,
	0xF0, 0x00, 0x00, 0x3F, 0xFF, 0xE0,
	0xF0, 0x00, 0x00, 0x1F, 0xFF, 0xE0,
	0xE0, 0x0F, 0x80, 0x0F, 0xFF, 0xE0,
	0xE0, 0x3F, 0xF0, 0x07, 0xFF, 0xE0,
	0xC0, 0x7F, 0xF8, 0x03, 0xFF, 0xE0,
	0xC0, 0xFF, 0xFE, 0x01, 0xFF, 0xE0,
	0xC1, 0xFF, 0xFF, 0x00, 0xFF, 0xE0,
	0x81, 0xFF, 0xFF, 0x80, 0x7F, 0xE0,
	0x83, 0xFF, 0xFF, 0xC0, 0x3F, 0xE0,
	0x83, 0xFF, 0xFF, 0xE0, 0x1F, 0xE0,
	0x83, 0xFF, 0xFF, 0xF0, 0x07, 0xE0,
	0x83, 0xFF, 0xFF, 0xF8, 0x03, 0xE0,
	0x83, 0xFF, 0xFF, 0xFC, 0x01, 0xE0,
	0x83, 0xFF, 0xFF, 0xFE, 0x00, 0xE0,
	0x83, 0xFF, 0xFF, 0xFF, 0x00, 0x60,
	0x83, 0xFF, 0xFF, 0xFF, 0x80, 0x20,
	0x81, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xE0, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xF0, 0x00,
	0xC0, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
	0xC0, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
	0xE0, 0xFF, 0xFF, 0xFF, 0xFE, 0x00,
	0xE0, 0x7F, 0xFF, 0xFF, 0xFF, 0x00,
	0xE0, 0x7F, 0xFF, 0xFF, 0xFF, 0x80,
	0xF0, 0x3F, 0xFF, 0xFF, 0xFF, 0xC0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// Small digit 3 (32x48)
const unsigned char num_sml_3[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFC, 0x03, 0xFF,
	0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0xFF,
	0xFF, 0x80, 0xFF, 0xE0, 0x00, 0x3F,
	0xFC, 0x00, 0x3F, 0x80, 0x00, 0x1F,
	0xF8, 0x00, 0x1F, 0x80, 0x00, 0x0F,
	0xF0, 0x00, 0x0F, 0x00, 0x00, 0x07,
	0xE0, 0x00, 0x06, 0x01, 0xF8, 0x03,
	0xC0, 0x00, 0x06, 0x07, 0xFE, 0x03,
	0xC0, 0x3F, 0x02, 0x0F, 0xFF, 0x03,
	0x80, 0xFF, 0x80, 0x1F, 0xFF, 0x81,
	0x81, 0xFF, 0xC0, 0x3F, 0xFF, 0xC1,
	0x83, 0xFF, 0xE0, 0x3F, 0xFF, 0xC1,
	0x03, 0xFF, 0xE0, 0x3F, 0xFF, 0xC0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x03, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
	0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1,
	0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1,
	0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0x81,
	0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x83,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// Small digit 4 (32x48)
const unsigned char num_sml_4[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xFF,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xC0, 0xFF, 0xFF, 0xFF, 0x07, 0xFF,
	0xC0, 0x7F, 0xFF, 0xFF, 0x07, 0xFF,
	0xE0, 0x1F, 0xFF, 0xFF, 0x07, 0xFF,
	0xF0, 0x07, 0xFF, 0xFF, 0x07, 0xFF,
	0xFC, 0x03, 0xFF, 0xFF, 0x07, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0x07, 0xFF,
	0xFF, 0x80, 0x7F, 0xFF, 0x07, 0xFF,
	0xFF, 0xE0, 0x1F, 0xFF, 0x07, 0xFF,
	0xFF, 0xF0, 0x0F, 0xFF, 0x07, 0xFF,
	0xFF, 0xFC, 0x03, 0xFF, 0x07, 0xFF,
	0xFF, 0xFE, 0x01, 0xFF, 0x07, 0xFF,
	0xFF, 0xFF, 0x80, 0x7F, 0x07, 0xFF,
	0xFF, 0xFF, 0xC0, 0x1F, 0x07, 0xFF,
	0xFF, 0xFF, 0xF0, 0x0F, 0x07, 0xFF,
	0xFF, 0xFF, 0xF8, 0x03, 0x07, 0xFF,
	0xFF, 0xFF, 0xFE, 0x01, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xC0, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xE0, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xF8, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFC, 0x07, 0xFF
};

// Small digit 5 (32x48)
const unsigned char num_sml_5[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xF0, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0x80, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x3F,
	0x83, 0xFF, 0xFC, 0x00, 0x00, 0x1F,
	0x83, 0xFF, 0xF8, 0x00, 0x00, 0x0F,
	0x83, 0xFF, 0xF0, 0x00, 0x00, 0x07,
	0x83, 0xFF, 0xF0, 0x07, 0xF0, 0x07,
	0x83, 0xFF, 0xE0, 0x1F, 0xFC, 0x03,
	0x83, 0xFF, 0xE0, 0x3F, 0xFF, 0x03,
	0x83, 0xFF, 0xC0, 0x7F, 0xFF, 0x01,
	0x83, 0xFF, 0xC0, 0xFF, 0xFF, 0x81,
	0x83, 0xFF, 0xC1, 0xFF, 0xFF, 0xC1,
	0x83, 0xFF, 0x81, 0xFF, 0xFF, 0xC0,
	0x83, 0xFF, 0x81, 0xFF, 0xFF, 0xC0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x83, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0xC3, 0xFF, 0xFF, 0xE0,
	0x80, 0x00, 0x03, 0xFF, 0xFF, 0xE0,
	0x80, 0x00, 0x01, 0xFF, 0xFF, 0xC0,
	0x80, 0x00, 0x01, 0xFF, 0xFF, 0xC1,
	0x80, 0x00, 0x01, 0xFF, 0xFF, 0xC1,
	0x80, 0x00, 0x01, 0xFF, 0xFF, 0xC1,
	0x80, 0x00, 0x00, 0xFF, 0xFF, 0x81,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x83,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// Small digit 6 (32x48)
const unsigned char num_sml_6[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xE0, 0x07, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
	0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x7F,
	0xC0, 0xFF, 0xF8, 0x00, 0x00, 0x1F,
	0x81, 0xFF, 0xF0, 0x00, 0x00, 0x0F,
	0x81, 0xFF, 0xE0, 0x00, 0x00, 0x07,
	0x83, 0xFF, 0xC0, 0x0F, 0xF0, 0x03,
	0x83, 0xFF, 0xC0, 0x7F, 0xFE, 0x03,
	0x03, 0xFF, 0x80, 0xFF, 0xFF, 0x01,
	0x07, 0xFF, 0x81, 0xFF, 0xFF, 0x81,
	0x07, 0xFF, 0x83, 0xFF, 0xFF, 0xC1,
	0x07, 0xFF, 0x03, 0xFF, 0xFF, 0xC0,
	0x07, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x07, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x03, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x83, 0xFF, 0x07, 0xFF, 0xFF, 0xE0,
	0x81, 0xFF, 0x83, 0xFF, 0xFF, 0xC0,
	0xC1, 0xFF, 0x83, 0xFF, 0xFF, 0xC1,
	0xC0, 0xFF, 0xC1, 0xFF, 0xFF, 0x81,
	0xE0, 0x3F, 0xC0, 0xFF, 0xFF, 0x03,
	0xE0, 0x1F, 0xE0, 0x7F, 0xFE, 0x03,
	0xF0, 0x07, 0xF0, 0x0F, 0xF0, 0x07,
	0xF8, 0x00, 0x78, 0x00, 0x00, 0x0F,
	0xFC, 0x00, 0x00, 0x00, 0x00, 0x1F,
	0xFE, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0xFF, 0x80, 0x00, 0x00, 0x00, 0x7F,
	0xFF, 0xE0, 0x00, 0x00, 0x01, 0xFF,
	0xFF, 0xF8, 0x00, 0x00, 0x0F, 0xFF,
	0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF
};

// Small digit 7 (32x48)
const unsigned char num_sml_7[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0x07, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC0, 0x00, 0x3F, 0xFF, 0xFF, 0xFF,
	0xC0, 0x00, 0x07, 0xFF, 0xFF, 0xFF,
	0xC0, 0x00, 0x01, 0xFF, 0xFF, 0xFF,
	0xC1, 0xC0, 0x00, 0x3F, 0xFF, 0xFF,
	0xC1, 0xF0, 0x00, 0x0F, 0xFF, 0xFF,
	0xC1, 0xFE, 0x00, 0x01, 0xFF, 0xFF,
	0xC1, 0xFF, 0x80, 0x00, 0x7F, 0xFF,
	0xC1, 0xFF, 0xE0, 0x00, 0x0F, 0xFF,
	0xC1, 0xFF, 0xFC, 0x00, 0x03, 0xFF,
	0xC1, 0xFF, 0xFF, 0x00, 0x00, 0x7F,
	0xC1, 0xFF, 0xFF, 0xE0, 0x00, 0x1F,
	0xC1, 0xFF, 0xFF, 0xF8, 0x00, 0x07,
	0xC1, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFE, 0x00,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// Small digit 8 (32x48)
const unsigned char num_sml_8[] PROGMEM = {
	0xFF, 0xFF, 0xFF, 0xFC, 0x03, 0xFF,
	0xFF, 0x01, 0xFF, 0xF0, 0x00, 0x7F,
	0xFC, 0x00, 0x3F, 0xC0, 0x00, 0x1F,
	0xF8, 0x00, 0x1F, 0x80, 0x00, 0x0F,
	0xF0, 0x00, 0x0F, 0x80, 0x00, 0x07,
	0xE0, 0x00, 0x07, 0x00, 0x00, 0x07,
	0xC0, 0x00, 0x06, 0x01, 0xF8, 0x03,
	0xC0, 0x3E, 0x02, 0x07, 0xFE, 0x03,
	0x80, 0xFF, 0x82, 0x0F, 0xFF, 0x01,
	0x81, 0xFF, 0xC0, 0x1F, 0xFF, 0x81,
	0x83, 0xFF, 0xE0, 0x3F, 0xFF, 0xC1,
	0x03, 0xFF, 0xE0, 0x3F, 0xFF, 0xC0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x07, 0xFF, 0xF0, 0x7F, 0xFF, 0xE0,
	0x03, 0xFF, 0xE0, 0x3F, 0xFF, 0xC0,
	0x83, 0xFF, 0xE0, 0x3F, 0xFF, 0xC1,
	0x81, 0xFF, 0xC0, 0x1F, 0xFF, 0x81,
	0x80, 0xFF, 0x82, 0x0F, 0xFF, 0x01,
	0xC0, 0x3E, 0x02, 0x07, 0xFE, 0x01,
	0xC0, 0x00, 0x06, 0x01, 0xF8, 0x03,
	0xE0, 0x00, 0x07, 0x00, 0x00, 0x07,
	0xF0, 0x00, 0x0F, 0x80, 0x00, 0x07,
	0xF8, 0x00, 0x1F, 0x80, 0x00, 0x0F,
	0xFC, 0x00, 0x3F, 0xC0, 0x00, 0x1F,
	0xFF, 0x01, 0xFF, 0xF0, 0x00, 0x7F,
	0xFF, 0xFF, 0xFF, 0xFC, 0x03, 0xFF
};

// Small digit 9 (32x48)
const unsigned char num_sml_9[] PROGMEM = {
	0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF,
	0xFF, 0xF0, 0x00, 0x00, 0x3F, 0xFF,
	0xFF, 0x80, 0x00, 0x00, 0x07, 0xFF,
	0xFE, 0x00, 0x00, 0x00, 0x01, 0xFF,
	0xFC, 0x00, 0x00, 0x00, 0x00, 0x7F,
	0xF8, 0x00, 0x00, 0x00, 0x00, 0x3F,
	0xF0, 0x00, 0x00, 0x1E, 0x00, 0x1F,
	0xE0, 0x0F, 0xF0, 0x0F, 0xE0, 0x0F,
	0xC0, 0x7F, 0xFE, 0x07, 0xF8, 0x07,
	0xC0, 0xFF, 0xFF, 0x03, 0xFC, 0x07,
	0x81, 0xFF, 0xFF, 0x83, 0xFF, 0x03,
	0x83, 0xFF, 0xFF, 0xC1, 0xFF, 0x83,
	0x03, 0xFF, 0xFF, 0xC1, 0xFF, 0x81,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xC1,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xC1,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xE0,
	0x07, 0xFF, 0xFF, 0xE0, 0xFF, 0xE0,
	0x03, 0xFF, 0xFF, 0xC0, 0xFF, 0xE0,
	0x83, 0xFF, 0xFF, 0xC1, 0xFF, 0xE0,
	0x81, 0xFF, 0xFF, 0x81, 0xFF, 0xE0,
	0x80, 0xFF, 0xFF, 0x01, 0xFF, 0xC0,
	0xC0, 0x7F, 0xFE, 0x03, 0xFF, 0xC1,
	0xC0, 0x0F, 0xF0, 0x03, 0xFF, 0xC1,
	0xE0, 0x00, 0x00, 0x07, 0xFF, 0x81,
	0xF0, 0x00, 0x00, 0x0F, 0xFF, 0x81,
	0xF8, 0x00, 0x00, 0x1F, 0xFF, 0x03,
	0xFE, 0x00, 0x00, 0x3F, 0xFF, 0xFF,
	0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
	0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xFF
};
