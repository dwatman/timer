#include "timer_funcs.h"
#include "common.h"

void inc_timer(timer_t *timer) {
	timer->sec01++;

	// Carry overflows through the digits
	if (timer->sec01 > 9) { timer->sec01 = 0; timer->sec10++; }
	if (timer->sec10 > 5) { timer->sec10 = 0; timer->min01++; }
	if (timer->min01 > 9) { timer->min01 = 0; timer->min10++; }
	if (timer->min10 > 5) { timer->min10 = 0; timer->hr01++; }

	// If the hour has overflowed then reset everything to max
	if (timer->hr01 > 9) {
		timer->sec01 = 9;
		timer->sec10 = 5;
		timer->min01 = 9;
		timer->min10 = 5;
		timer->hr01 = 9;
	}
}
