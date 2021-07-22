#include <avr/eeprom.h>

#include "timer_funcs.h"
#include "common.h"

extern volatile uint16_t ep_upd_flg;	// Indicates which areas of the display need updating
extern volatile uint16_t ep_upd_flg2;	// For the other image buffer in the display

void swap_upd_buffers(void) {
	uint16_t tmp;

	// Swap display update flags between the two buffers
	tmp = ep_upd_flg;
	ep_upd_flg = ep_upd_flg2;
	ep_upd_flg2 = tmp;
}

void timer_clear(timer_t *timer) {
	timer->hr01 = 0;
	timer->min10 = 0;
	timer->min01 = 0;
	timer->sec10 = 0;
	timer->sec01 = 0;

	ep_upd_flg |= EPD_UPD_ALL;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_inc_sec01(timer_t *timer) {
	if (timer->sec01 < 9)
		timer->sec01++;
	else
		timer->sec01 = 0;

	ep_upd_flg |= EPD_UPD_S01;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_inc_sec10(timer_t *timer) {
	if (timer->sec10 < 5)
		timer->sec10++;
	else
		timer->sec10 = 0;

	ep_upd_flg |= EPD_UPD_S10;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_inc_min01(timer_t *timer) {
	if (timer->min01 < 9)
		timer->min01++;
	else
		timer->min01 = 0;

	ep_upd_flg |= EPD_UPD_M01;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_inc_min10(timer_t *timer) {
	if (timer->min10 < 5)
		timer->min10++;
	else
		timer->min10 = 0;

	ep_upd_flg |= EPD_UPD_M10;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_inc_hr01(timer_t *timer) {
	if (timer->hr01 < 9)
		timer->hr01++;
	else
		timer->hr01 = 0;

	ep_upd_flg |= EPD_UPD_H01;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

uint8_t timer_count_down(timer_t *timer) {
	// Decrement seconds 01 digit if non-zero
	if (timer->sec01 > 0) {
		timer->sec01--;

		ep_upd_flg |= EPD_UPD_S01;
		ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
		return 0;
	}

	// Digits are X:XX:X0
	if (timer->sec10 > 0) {
		timer->sec10--;		// Carry from seconds 10 digit
		timer->sec01 = 9;	// To seconds 01 digit

		ep_upd_flg |= EPD_UPD_S01;
		ep_upd_flg |= EPD_UPD_S10;
		ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
		return 0;
	}

	// Digits are X:XX:00
	if (timer->min01 > 0) {
		timer->min01--;		// Carry from minutes 01 digit
		timer->sec10 = 5;	// To seconds 10 digit
		timer->sec01 = 9;	// And seconds 01 digit

		ep_upd_flg |= EPD_UPD_S01;
		ep_upd_flg |= EPD_UPD_S10;
		ep_upd_flg |= EPD_UPD_M01;
		ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
		return 0;
	}

	// Digits are X:X0:00
	if (timer->min10 > 0) {
		timer->min10--;		// Carry from minutes 10 digit
		timer->min01 = 9;	// To minutes 01 digit
		timer->sec10 = 5;	// And seconds 10 digit
		timer->sec01 = 9;	// And seconds 01 digit

		ep_upd_flg |= EPD_UPD_S01;
		ep_upd_flg |= EPD_UPD_S10;
		ep_upd_flg |= EPD_UPD_M01;
		ep_upd_flg |= EPD_UPD_M10;
		ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
		return 0;
	}

	// Digits are X:00:00
	if (timer->hr01 > 0) {
		timer->hr01--;		// Carry from hours 01 digit
		timer->min10 = 5;	// To minutes 10 digit
		timer->min01 = 9;	// And minutes 01 digit
		timer->sec10 = 5;	// And seconds 10 digit
		timer->sec01 = 9;	// And seconds 01 digit

		ep_upd_flg |= EPD_UPD_S01;
		ep_upd_flg |= EPD_UPD_S10;
		ep_upd_flg |= EPD_UPD_M01;
		ep_upd_flg |= EPD_UPD_M10;
		ep_upd_flg |= EPD_UPD_H01;
		ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
		return 0;
	}

	// Digits are 0:00:00 (time expired)
	return 1;
}

void timer_read_mem1(timer_t *timer) {
	eeprom_read_block(timer, (void *)EEPROM_MEM1_ADDR, sizeof(timer_t));
	ep_upd_flg |= EPD_UPD_S01;
	ep_upd_flg |= EPD_UPD_S10;
	ep_upd_flg |= EPD_UPD_M01;
	ep_upd_flg |= EPD_UPD_M10;
	ep_upd_flg |= EPD_UPD_H01;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_read_mem2(timer_t *timer) {
	eeprom_read_block(timer, (void *)EEPROM_MEM2_ADDR, sizeof(timer_t));
	ep_upd_flg |= EPD_UPD_S01;
	ep_upd_flg |= EPD_UPD_S10;
	ep_upd_flg |= EPD_UPD_M01;
	ep_upd_flg |= EPD_UPD_M10;
	ep_upd_flg |= EPD_UPD_H01;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_read_mem3(timer_t *timer) {
	eeprom_read_block(timer, (void *)EEPROM_MEM3_ADDR, sizeof(timer_t));
	ep_upd_flg |= EPD_UPD_S01;
	ep_upd_flg |= EPD_UPD_S10;
	ep_upd_flg |= EPD_UPD_M01;
	ep_upd_flg |= EPD_UPD_M10;
	ep_upd_flg |= EPD_UPD_H01;
	ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
}

void timer_write_mem1(timer_t *timer) {
	eeprom_update_block(timer, (void *)EEPROM_MEM1_ADDR, sizeof(timer_t));
}

void timer_write_mem2(timer_t *timer) {
	eeprom_update_block(timer, (void *)EEPROM_MEM2_ADDR, sizeof(timer_t));
}

void timer_write_mem3(timer_t *timer) {
	eeprom_update_block(timer, (void *)EEPROM_MEM3_ADDR, sizeof(timer_t));
}
