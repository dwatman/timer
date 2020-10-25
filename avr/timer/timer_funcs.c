#include "timer_funcs.h"
#include "common.h"

void timer_clear(timer_t *timer) {
	timer->hr01 = 0;
	timer->min10 = 0;
	timer->min01 = 0;
	timer->sec10 = 0;
	timer->sec01 = 0;
}

void timer_set_mem1(timer_t *timer) {
	timer->hr01 = 0;
	timer->min10 = 0;
	timer->min01 = 0;
	timer->sec10 = 1;
	timer->sec01 = 1;
}

void timer_set_mem2(timer_t *timer) {
	timer->hr01 = 0;
	timer->min10 = 0;
	timer->min01 = 1;
	timer->sec10 = 2;
	timer->sec01 = 3;
}

void timer_set_mem3(timer_t *timer) {
	timer->hr01 = 0;
	timer->min10 = 1;
	timer->min01 = 0;
	timer->sec10 = 0;
	timer->sec01 = 0;
}

void timer_check_digits(timer_t *timer) {
	// Digits cycle back to zero on overflow
	if (timer->sec01 > 9) { timer->sec01 = 0; }
	if (timer->sec10 > 5) { timer->sec10 = 0; }
	if (timer->min01 > 9) { timer->min01 = 0; }
	if (timer->min10 > 5) { timer->min10 = 0; }

	// Alternate behaviour, carry overflows through the digits
	// if (timer->sec01 > 9) { timer->sec01 = 0; timer->sec10++; }
	// if (timer->sec10 > 5) { timer->sec10 = 0; timer->min01++; }
	// if (timer->min01 > 9) { timer->min01 = 0; timer->min10++; }
	// if (timer->min10 > 5) { timer->min10 = 0; timer->hr01++; }

	// If the hour has overflowed then reset everything to max
	if (timer->hr01 > 9) {
		timer->sec01 = 9;
		timer->sec10 = 5;
		timer->min01 = 9;
		timer->min10 = 5;
		timer->hr01 = 9;
	}
}
