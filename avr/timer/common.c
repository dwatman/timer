#include <avr/io.h>
#include "common.h"

extern volatile unsigned int time_ms;

void delay_ms(unsigned int delay) {
	unsigned int now, end;
	
	now = time_ms;
	if (TCNT1 >= 500) now++;	// delay another ms if count is past halfway

	end = now + delay;			// find end time (overflow is ok)
	
	while (time_ms != end);		// wait until the end time is reached
}
