#include <avr/io.h>

#include "buttons.h"
#include "common.h"
#include "timer_funcs.h"
#include "epaper.h"

extern timer_t count_time;		// Counter time (volatile?)
extern volatile uint8_t flg;			// General purpose flags
extern volatile enum state_e state;		// Operating state

extern volatile uint16_t ep_upd_flg;	// Indicates which areas of the display need updating
extern volatile uint16_t ep_upd_flg2;	// For the other image buffer in the display

// Process buttons
void check_buttons(void) {
	static uint8_t tpress_sec01 = 0;
	static uint8_t tpress_sec10 = 0;
	static uint8_t tpress_min01 = 0;
	static uint8_t tpress_min10 = 0;
	static uint8_t tpress_hr = 0;

	static uint8_t tpress_clr = 0;
	static uint8_t tpress_start = 0;
	static uint8_t tpress_mem1 = 0;
	static uint8_t tpress_mem2 = 0;
	static uint8_t tpress_mem3 = 0;

	// Check START button
	if (PIND & START_MASK) {	// Button not pressed
		tpress_start = 0;
	}
	else {		// Button pressed
		if (tpress_start == BTN_SHORT_PRESS)	// Take action for short press
			start_button_func();

		if (tpress_start <= BTN_SHORT_PRESS)	// Increment count until over threshold (then stop)
			tpress_start++;
	}

	// Check CLEAR button
	if (PIND & CLR_MASK) {	// Button not pressed
		tpress_clr = 0;
	}
	else {		// Button pressed
		if (tpress_clr == BTN_SHORT_PRESS)	// Take action for short press
			clear_button_short_func();
		
		if (tpress_clr == MEM_LONG_PRESS)	// Take action for long press
			clear_button_long_func();

		if (tpress_clr <= MEM_LONG_PRESS)	// Increment count until over threshold (then stop)
			tpress_clr++;
	}

	// Check MEM1 button
	if (PIND & MEM1_MASK) {	// Button not pressed
		tpress_mem1 = 0;
	}
	else {		// Button pressed
		if (tpress_mem1 == BTN_SHORT_PRESS) {	// Take action for short press
			timer_read_mem1(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_mem1 == MEM_LONG_PRESS) {	// Take action for long press
			if (state == STATE_STOPPED) {
				state = STATE_SET_MEM1;
				ep_upd_flg |= EPD_UPD_MEM;	// Write MEM1 to display
				ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
				flg |= FLG_UPD;		// Set flag to update display
			}
		}

		if (tpress_mem1 <= MEM_LONG_PRESS)	// Increment count until over threshold (then stop)
			tpress_mem1++;
	}

	// Check MEM2 button
	if (PIND & MEM2_MASK) {	// Button not pressed
		tpress_mem2 = 0;
	}
	else {		// Button pressed
		if (tpress_mem2 == BTN_SHORT_PRESS) {	// Take action for short press
			timer_read_mem2(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_mem2 == MEM_LONG_PRESS) {	// Take action for long press
			if (state == STATE_STOPPED) {
				state = STATE_SET_MEM2;
				ep_upd_flg |= EPD_UPD_MEM;	// Write MEM2 to display
				ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
				flg |= FLG_UPD;		// Set flag to update display
			}
		}
		
		if (tpress_mem2 <= MEM_LONG_PRESS)	// Increment count until over threshold (then stop)
			tpress_mem2++;
	}

	// Check MEM3 button
	if (PIND & MEM3_MASK) {	// Button not pressed
		tpress_mem3 = 0;
	}
	else {		// Button pressed
		if (tpress_mem3 == BTN_SHORT_PRESS) {	// Take action for short press
			timer_read_mem3(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_mem3 == MEM_LONG_PRESS) {	// Take action for long press
			if (state == STATE_STOPPED) {
				state = STATE_SET_MEM3;
				ep_upd_flg |= EPD_UPD_MEM;	// Write MEM3 to display
				ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
				flg |= FLG_UPD;		// Set flag to update display
			}
		}
		
		if (tpress_mem3 <= MEM_LONG_PRESS)	// Increment count until over threshold (then stop)
			tpress_mem3++;
	}

	// Check SEC01 button
	if (PINA & SEC01_MASK) {	// Button not pressed
		tpress_sec01 = 0;
	}
	else {		// Button pressed
		if (tpress_sec01 == BTN_SHORT_PRESS)	{	// Take action for short press
			timer_inc_sec01(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_sec01 == HMS_LONG_PRESS) {	// Take action for long press
			timer_inc_sec01(&count_time);
			tpress_sec01 = BTN_SHORT_PRESS;		// Reset so that action will repeat
			flg |= FLG_UPD;					// Set flag to update display
		}

		// Increment count until over threshold (and repeat)
		tpress_sec01++;
	}

	// Check SEC10 button
	if (PINA & SEC10_MASK) {	// Button not pressed
		tpress_sec10 = 0;
	}
	else {		// Button pressed
		if (tpress_sec10 == BTN_SHORT_PRESS)	{	// Take action for short press
			timer_inc_sec10(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_sec10 == HMS_LONG_PRESS) {	// Take action for long press
			timer_inc_sec10(&count_time);
			tpress_sec10 = BTN_SHORT_PRESS;		// Reset so that action will repeat
			flg |= FLG_UPD;					// Set flag to update display
		}

		// Increment count until over threshold (and repeat)
		tpress_sec10++;
	}

	// Check MIN01 button
	if (PINA & MIN01_MASK) {	// Button not pressed
		tpress_min01 = 0;
	}
	else {		// Button pressed
		if (tpress_min01 == BTN_SHORT_PRESS)	{	// Take action for short press
			timer_inc_min01(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_min01 == HMS_LONG_PRESS) {	// Take action for long press
			timer_inc_min01(&count_time);
			tpress_min01 = BTN_SHORT_PRESS;		// Reset so that action will repeat
			flg |= FLG_UPD;					// Set flag to update display
		}

		// Increment count until over threshold (and repeat)
		tpress_min01++;
	}

	// Check MIN10 button
	if (PINA & MIN10_MASK) {	// Button not pressed
		tpress_min10 = 0;
	}
	else {		// Button pressed
		if (tpress_min10 == BTN_SHORT_PRESS)	{	// Take action for short press
			timer_inc_min10(&count_time);
			flg |= FLG_UPD;					// Set flag to update display
		}

		if (tpress_min10 == HMS_LONG_PRESS) {	// Take action for long press
			timer_inc_min10(&count_time);
			tpress_min10 = BTN_SHORT_PRESS;		// Reset so that action will repeat
			flg |= FLG_UPD;					// Set flag to update display
		}

		// Increment count until over threshold (and repeat)
		tpress_min10++;
	}

	// Check HR button
	if (PINA & HR_MASK) {	// Button not pressed
		tpress_hr = 0;
	}
	else {		// Button pressed
		if (tpress_hr == BTN_SHORT_PRESS) {	// Take action for short press
			timer_inc_hr01(&count_time);
			flg |= FLG_UPD;				// Set flag to update display
		}

		if (tpress_hr == HMS_LONG_PRESS) {	// Take action for long press
			timer_inc_hr01(&count_time);
			tpress_hr = BTN_SHORT_PRESS;		// Reset so that action will repeat
			flg |= FLG_UPD;				// Set flag to update display
		}

		// Increment count until over threshold (and repeat)
		tpress_hr++;
	}

}

void start_button_func(void) {
	switch (state) {
		case STATE_IDLE_SLEEP:
			// Wake up
			state = STATE_STOPPED;
			break;
		case STATE_STOPPED:
			TCNT2 = 0;	// Reset timer2 count
			state = STATE_ACTIVE;
			break;
		case STATE_SET_MEM1:
			timer_write_mem1(&count_time);	// Store current count in MEM1
			state = STATE_STOPPED;
			ep_upd_flg |= EPD_UPD_MEM;	// Remove MEM1 set from display
			ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
			flg |= FLG_UPD;		// Set flag to update display
			break;
		case STATE_SET_MEM2:
			timer_write_mem2(&count_time);	// Store current count in MEM2
			state = STATE_STOPPED;
			ep_upd_flg |= EPD_UPD_MEM;	// Remove MEM2 set from display
			ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
			flg |= FLG_UPD;		// Set flag to update display
			break;
		case STATE_SET_MEM3:
			timer_write_mem3(&count_time);	// Store current count in MEM3
			state = STATE_STOPPED;
			ep_upd_flg |= EPD_UPD_MEM;	// Remove MEM3 set from display
			ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
			flg |= FLG_UPD;		// Set flag to update display
			break;
		case STATE_ACTIVE:
			state = STATE_STOPPED;	// Pause counting
			break;
		case STATE_END_BEEP:
			// Stop beeping
		case STATE_END_DONE:
			// Clear done indication
			flg |= FLG_UPD;		// Set flag to update display
			state = STATE_STOPPED;
			break;
	}
}

void clear_button_short_func(void) {
	switch (state) {
		case STATE_IDLE_SLEEP:
			// Wake up
			state = STATE_STOPPED;
		case STATE_STOPPED:
			timer_clear(&count_time);	// Clear timer count
			flg |= FLG_UPD;		// Set flag to update display
			break;
		case STATE_SET_MEM1:
		case STATE_SET_MEM2:
		case STATE_SET_MEM3:
			state = STATE_STOPPED;
			ep_upd_flg |= EPD_UPD_MEM;
			ep_upd_flg2 |= ep_upd_flg;	// Copy changes to other buffer too
			flg |= FLG_UPD;		// Set flag to update display
			break;
		case STATE_ACTIVE: break;
		case STATE_END_BEEP:
			// Stop beeping
		case STATE_END_DONE:
			// Clear done indication
			flg |= FLG_UPD;		// Set flag to update display
			state = STATE_STOPPED;
			break;
	}
}

void clear_button_long_func(void) {
	flg |= FLG_TURNOFF;		// Clear display and go to low-power state
}
