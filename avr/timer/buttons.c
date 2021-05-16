#include <avr/io.h>

#include "buttons.h"
#include "common.h"
#include "timer_funcs.h"

extern timer_t count_time;	// ? volatile

uint8_t tpress_sec01 = 0;
uint8_t tpress_sec10 = 0;
uint8_t tpress_min01 = 0;
uint8_t tpress_min10 = 0;
uint8_t tpress_hr = 0;

uint8_t tpress_clr = 0;
uint8_t tpress_start = 0;
uint8_t tpress_mem1 = 0;
uint8_t tpress_mem2 = 0;
uint8_t tpress_mem3 = 0;

// Process buttons
void check_buttons(void) {
	// Check START button
	if (PIND & START_MASK) {	// Button not pressed
		tpress_start = 0;
	}
	else {		// Button pressed
		if (tpress_start == BTN_TSHORT)	// Take action for short press
			//*********

		if (tpress_start <= BTN_TSHORT)	// Increment count until over threshold (then stop)
			tpress_start++;
	}

	// Check CLEAR button
	if (PIND & CLR_MASK) {	// Button not pressed
		tpress_clr = 0;
	}
	else {		// Button pressed
		if (tpress_clr == BTN_TSHORT) {	// Take action for short press
			//timer_clear(&count_time);	// Clear timer count
			//flg |= FLG_UPD;		// Set flag to update display
		}

		if (tpress_clr <= BTN_TSHORT)	// Increment count until over threshold (then stop)
			tpress_clr++;
	}

	// Check MEM1 button
	if (PIND & MEM1_MASK) {	// Button not pressed
		tpress_mem1 = 0;
	}
	else {		// Button pressed
		if (tpress_mem1 == BTN_TSHORT)	// Take action for short press
			timer_read_mem1(&count_time);

		if (tpress_mem1 == BTN_TLONG)	// Take action for long press
			//*********

		if (tpress_mem1 <= BTN_TLONG)	// Increment count until over threshold (then stop)
			tpress_mem1++;
	}

	// Check MEM2 button
	if (PIND & MEM2_MASK) {	// Button not pressed
		tpress_mem2 = 0;
	}
	else {		// Button pressed
		if (tpress_mem2 == BTN_TSHORT)	// Take action for short press
			timer_read_mem2(&count_time);

		if (tpress_mem2 == BTN_TLONG)	// Take action for long press
			//*********

		if (tpress_mem2 <= BTN_TLONG)	// Increment count until over threshold (then stop)
			tpress_mem2++;
	}

	// Check MEM3 button
	if (PIND & MEM3_MASK) {	// Button not pressed
		tpress_mem3 = 0;
	}
	else {		// Button pressed
		if (tpress_mem3 == BTN_TSHORT)	// Take action for short press
			timer_read_mem3(&count_time);

		if (tpress_mem3 == BTN_TLONG)	// Take action for long press
			//*********

		if (tpress_mem3 <= BTN_TLONG)	// Increment count until over threshold (then stop)
			tpress_mem3++;
	}

	// Check SEC01 button
	if (PINA & SEC01_MASK) {	// Button not pressed
		tpress_sec01 = 0;
	}
	else {		// Button pressed
		if (tpress_sec01 == BTN_TSHORT)	// Take action for short press
			timer_inc_sec01(&count_time);

		if (tpress_sec01 == BTN_TLONG) {	// Take action for long press
			timer_inc_sec01(&count_time);
			tpress_sec01 = BTN_TSHORT;	// Reset so that action will repeat
		}

		// Increment count until over threshold (and repeat)
		tpress_sec01++;
	}

	// Check SEC10 button
	if (PINA & SEC10_MASK) {	// Button not pressed
		tpress_sec10 = 0;
	}
	else {		// Button pressed
		if (tpress_sec10 == BTN_TSHORT)	// Take action for short press
			timer_inc_sec10(&count_time);

		if (tpress_sec10 == BTN_TLONG) {	// Take action for long press
			timer_inc_sec10(&count_time);
			tpress_sec10 = BTN_TSHORT;	// Reset so that action will repeat
		}

		// Increment count until over threshold (and repeat)
		tpress_sec10++;
	}

	// Check MIN01 button
	if (PINA & MIN01_MASK) {	// Button not pressed
		tpress_min01 = 0;
	}
	else {		// Button pressed
		if (tpress_min01 == BTN_TSHORT)	// Take action for short press
			timer_inc_min01(&count_time);

		if (tpress_min01 == BTN_TLONG) {	// Take action for long press
			timer_inc_min01(&count_time);
			tpress_min01 = BTN_TSHORT;	// Reset so that action will repeat
		}

		// Increment count until over threshold (and repeat)
		tpress_min01++;
	}

	// Check MIN10 button
	if (PINA & MIN10_MASK) {	// Button not pressed
		tpress_min10 = 0;
	}
	else {		// Button pressed
		if (tpress_min10 == BTN_TSHORT)	// Take action for short press
		timer_inc_min10(&count_time);

		if (tpress_min10 == BTN_TLONG) {	// Take action for long press
			timer_inc_min10(&count_time);
			tpress_min10 = BTN_TSHORT;	// Reset so that action will repeat
		}

		// Increment count until over threshold (and repeat)
		tpress_min10++;
	}

	// Check HR button
	if (PINA & HR_MASK) {	// Button not pressed
		tpress_hr = 0;
	}
	else {		// Button pressed
		if (tpress_hr == BTN_TSHORT)	// Take action for short press
		timer_inc_hr01(&count_time);

		if (tpress_hr == BTN_TLONG) {	// Take action for long press
			timer_inc_hr01(&count_time);
			tpress_hr = BTN_TSHORT;	// Reset so that action will repeat
		}

		// Increment count until over threshold (and repeat)
		tpress_hr++;
	}

}
