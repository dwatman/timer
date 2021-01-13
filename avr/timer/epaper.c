// Based on example code "GDEH029A1_Arduino_20191016" from GooDisplay

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "epaper.h"
#include "common.h"

#include "data_nums_small.h"
#include "data_nums_large.h"

//C221 25C Full update waveform
const uint8_t LUT_DATA_full[30] PROGMEM = {
	0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//C221 25C partial update waveform
const uint8_t LUT_DATA_part[30] PROGMEM = {
	0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

digit_t hr01, chm, min10, min01, cms, sec10, sec01;

void init_digits(void) {
	sec01.type = DIGIT_SMALL;
	sec01.x_start = DISP_STARTX_SEC;
	sec01.x_end = DISP_STARTX_SEC + NUM_SML_XSIZE - 1;
	sec01.y_start = DISP_STARTY_S01;
	sec01.y_end = DISP_STARTY_S01 + NUM_SML_YSIZE - 1;
	sec01.num_bytes = NUM_SML_XSIZE*NUM_SML_YSIZE;

	sec10.type = DIGIT_SMALL;
	sec10.x_start = DISP_STARTX_SEC;
	sec10.x_end = DISP_STARTX_SEC + NUM_SML_XSIZE - 1;
	sec10.y_start = DISP_STARTY_S10;
	sec10.y_end = DISP_STARTY_S10 + NUM_SML_YSIZE - 1;
	sec10.num_bytes = NUM_SML_XSIZE*NUM_SML_YSIZE;

	cms.type = DIGIT_COLON;
	cms.x_start = DISP_STARTX_CLN;
	cms.x_end = DISP_STARTX_CLN + 4 - 1;
	cms.y_start = DISP_STARTY_CMS;
	cms.y_end = DISP_STARTY_CMS + 6 - 1;
	cms.num_bytes = 4*6;

	min01.type = DIGIT_BIG;
	min01.x_start = DISP_STARTX_MIN;
	min01.x_end = DISP_STARTX_MIN + NUM_BIG_XSIZE - 1;
	min01.y_start = DISP_STARTY_M01;
	min01.y_end = DISP_STARTY_M01 + NUM_BIG_YSIZE - 1;
	min01.num_bytes = NUM_BIG_XSIZE*NUM_BIG_YSIZE;

	min10.type = DIGIT_BIG;
	min10.x_start = DISP_STARTX_MIN;
	min10.x_end = DISP_STARTX_MIN + NUM_BIG_XSIZE - 1;
	min10.y_start = DISP_STARTY_M10;
	min10.y_end = DISP_STARTY_M10 + NUM_BIG_YSIZE - 1;
	min10.num_bytes = NUM_BIG_XSIZE*NUM_BIG_YSIZE;

	chm.type = DIGIT_COLON;
	chm.x_start = DISP_STARTX_CLN;
	chm.x_end = DISP_STARTX_CLN + 4 - 1;
	chm.y_start = DISP_STARTY_CHM;
	chm.y_end = DISP_STARTY_CHM + 6 - 1;
	chm.num_bytes = 4*6;

	hr01.type = DIGIT_BIG;
	hr01.x_start = DISP_STARTX_HR;
	hr01.x_end = DISP_STARTX_HR + NUM_BIG_XSIZE - 1;
	hr01.y_start = DISP_STARTY_H01;
	hr01.y_end = DISP_STARTY_H01 + NUM_BIG_YSIZE - 1;
	hr01.num_bytes = NUM_BIG_XSIZE*NUM_BIG_YSIZE;
}

/*
static void ep_SPIdelay(void) {
	uint8_t i;
	i = 0;
	i++;
}

static void ep_write_cmd(uint8_t cmd) {
	//ep_SPIdelay();	// wait a bit?

	PORTB &= ~CS_MASK;	// CS# select
	PORTB &= ~CMD_MASK;	// CMD# mode

	SPDR = cmd;					// start SPI transfer
	while (!(SPSR & (1<<SPIF)));	// wait for transfer compete

	PORTB |= CS_MASK; // CS# deselect
}

static void ep_write_data(uint8_t data) {
	//ep_SPIdelay();	// wait a bit?

	PORTB &= ~CS_MASK;	// CS# select
	PORTB |= CMD_MASK;	// DATA mode

	SPDR = data;				// start SPI transfer
	while (!(SPSR & (1<<SPIF)));	// wait for transfer compete

	PORTB |= CS_MASK; // CS# deselect
}
*/

static inline void ep_cs_select(void) {
	PORTB &= ~CS_MASK;	// CS# select
}

static inline void ep_cs_deselect(void) {
	PORTB |= CS_MASK; // CS# deselect
}

static void ep_write_cmd(uint8_t cmd) {
	//ep_SPIdelay();	// wait a bit?

	PORTB &= ~CMD_MASK;	// CMD# mode

	SPDR = cmd;					// start SPI transfer
	while (!(SPSR & (1<<SPIF)));	// wait for transfer compete
}

static void ep_write_data(uint8_t data) {
	PORTB |= CMD_MASK;	// DATA mode

	SPDR = data;				// start SPI transfer
	while (!(SPSR & (1<<SPIF)));	// wait for transfer compete
}

static uint8_t ep_check_busy(void) {
	if (PINB & BUSY_MASK)
		return 1;
	else
		return 0;
}

static void ep_set_LUT(const uint8_t *data) {
	uint8_t i;

	ep_cs_select();
	ep_write_cmd(0x32);		// Write LUT register

	for (i=0; i<30; i++)
		ep_write_data(pgm_read_byte(&data[i]));

	ep_cs_deselect();
}

void ep_init_hw(void) {
	PORTB &= ~RST_MASK;	// RST# on
	delay_ms(15);
	PORTB |= RST_MASK;	// RST# off
	delay_ms(15);

	ep_cs_select();
	ep_write_cmd(0x01);		// Driver output control
	ep_write_data(0x27);	// (295+1)=296
	ep_write_data(0x01);
	ep_write_data(0x00);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x0C);		// Soft start
	ep_write_data(0xD7);
	ep_write_data(0xD6);
	ep_write_data(0x9D);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x2C);		// VCOM Voltage
	ep_write_data(0x9A);//A8????
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x3A);		// Set dummy line period
	ep_write_data(0x1A);	// 4 dummy line per gate?
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x3B);		// Set Gate line width
	ep_write_data(0x08);	// 2us per line?
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x11);		// Set Data Entry mode
	ep_write_data(0x03);	// Y inc, X inc, addr count updates in X direction (default)
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x3C);		// Border waveform control
	ep_write_data(0x33);//03???
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x44);		// Set RAM X address start/end position
	ep_write_data(0x00);	// start = 0
	ep_write_data(0x0F);	// end = 15
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x45);		// Set RAM Y address start/end position
	ep_write_data(0x00);	// start = 0
	ep_write_data(0x00);	// (high bit)
	ep_write_data(0x27);	// end = 295
	ep_write_data(0x01);	// (high bit)
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x4E);		// Set RAM x address counter
	ep_write_data(0x00);	// x = 0
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x4F);		// Set RAM y address counter
	ep_write_data(0x00);	// y = 0
	ep_write_data(0x00);	// (high bit)
	ep_cs_deselect();

	while (ep_check_busy());	// Wait until operations are complete

	ep_set_LUT(LUT_DATA_full);	// Set full update waveform (move out of init_part??)
}

void ep_init_part(void) {
	ep_init_hw(); // needed?????
	ep_set_LUT(LUT_DATA_part);	// Set partial update waveform
	//??? ep_write_cmd(0x4F);		// Set RAM y address counter

	// power on
	ep_cs_select();
	ep_write_cmd(0x22);		// Display update control 2
	ep_write_data(0xC0);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x20);		// Activate display update sequence
	ep_cs_deselect();

	while (ep_check_busy());	// Wait until operations are complete

	ep_cs_select();
	ep_write_cmd(0x3C);		// Border waveform control
	ep_write_data(0x01);
	ep_cs_deselect();
}

void ep_set_all_white(void) {
	uint16_t i;

	ep_cs_select();
	ep_write_cmd(0x24);		// Write RAM

	for (i=0; i<EP_TOTAL_BYTES; i++)
		ep_write_data(0xFF);	// all white

	ep_cs_deselect();
}

void ep_set_num(digit_t *digit, uint8_t val) {
	uint16_t i;
	uint8_t x_start = digit->x_start;
	uint8_t x_end = digit->x_end;
	uint8_t y_startL = digit->y_start&0xFF;
	uint8_t y_startH = (digit->y_start>>8)&0xFF;
	uint8_t y_endL = digit->y_end&0xFF;
	uint8_t y_endH = (digit->y_end>>8)&0xFF;
	const uint8_t *data;

	ep_cs_select();
	ep_write_cmd(0x44);		// Set RAM X address start/end position
	ep_write_data(x_start);	// start
	ep_write_data(x_end);	// end
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x45);		// Set RAM Y address start/end position
	ep_write_data(y_startL);	// start
	ep_write_data(y_startH);	// (high bit)
	ep_write_data(y_endL);	// end
	ep_write_data(y_endH);	// (high bit)
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x4E);		// Set RAM x address counter
	ep_write_data(x_start);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x4F);		// Set RAM y address counter
	ep_write_data(y_startL);
	ep_write_data(y_startH);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x24);		// Write RAM

	if (digit->type == DIGIT_COLON) {
		for (i=0; i<digit->num_bytes; i++) {
			// pattern B W W B
			if (((i & 0x0003) == 0) || ((i & 0x0003) == 3))
				ep_write_data(0x00);
			else
				ep_write_data(0xFF);
		}
	}
	else {
		if (digit->type == DIGIT_SMALL) {
			switch (val) {
				case 0: data = num_sml_0; break;
				case 1: data = num_sml_1; break;
				case 2: data = num_sml_2; break;
				case 3: data = num_sml_3; break;
				case 4: data = num_sml_4; break;
				case 5: data = num_sml_5; break;
				case 6: data = num_sml_6; break;
				case 7: data = num_sml_7; break;
				case 8: data = num_sml_8; break;
				case 9: data = num_sml_9; break;
				default: return;	// Abort on unknown digit
			}
		}
		else if (digit->type == DIGIT_BIG) {
			switch (val) {
				case 0: data = num_big_0; break;
				case 1: data = num_big_1; break;
				case 2: data = num_big_2; break;
				case 3: data = num_big_3; break;
				case 4: data = num_big_4; break;
				case 5: data = num_big_5; break;
				case 6: data = num_big_6; break;
				case 7: data = num_big_7; break;
				case 8: data = num_big_8; break;
				case 9: data = num_big_9; break;
				default: return;	// Abort on unknown digit
			}
		}
		else	// Unknown type, abort
			return;

		for (i=0; i<digit->num_bytes; i++) {
			ep_write_data(pgm_read_byte(&data[i]));
		}
	}
	ep_cs_deselect();

}

void ep_update_display(void) {
	ep_cs_select();
	ep_write_cmd(0x22);		// Display update control 2
	ep_write_data(0xC4);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x20);		// Activate display update sequence
	//ep_write_cmd(0xFF);//terminate?
	ep_cs_deselect();

	while (ep_check_busy());	// Wait until operations are complete
}

void ep_update_display_partial(void) {
	ep_cs_select();
	ep_write_cmd(0x22);		// Display update control 2
	ep_write_data(0x04);
	ep_cs_deselect();

	ep_cs_select();
	ep_write_cmd(0x20);		// Activate display update sequence
	ep_cs_deselect();

	while (ep_check_busy());	// Wait until operations are complete
}

void ep_deepsleep(void) {
	ep_cs_select();
	ep_write_cmd(0x10);		// Deep sleep mode
	ep_write_data(0x01);
	ep_cs_deselect();
}
