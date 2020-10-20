#include <avr/io.h>
#include "common.h"

extern volatile unsigned int time_ms;

void init_pins(void) {
	DDRA |= MBLANK_MASK;	// Set pin to output for MIC_BLANK

	PORTA |= SEC01_MASK;	// Enable pullup for button
	PORTA |= SEC10_MASK;	// Enable pullup for button
	PORTA |= MIN01_MASK;	// Enable pullup for button
	PORTA |= MIN10_MASK;	// Enable pullup for button
	PORTA |= HR_MASK; 		// Enable pullup for button

	DDRB |= RST_MASK;		// set pin to output for RST#
	DDRB |= CMD_MASK;		// set pin to output for DAT/CMD#
	DDRB |= CS_MASK;		// set pin to output for CS#
	DDRB |= MOSI_MASK;		// set pin to output for MOSI
	DDRB |= SCK_MASK;		// set pin to output for SCK

	DDRC |= LED_MASK;		// set pin to output for LED
	DDRC |= MOFF_MASK;		// set pin to output for MIC_OFF

	DDRD |= TXD_MASK;		// set pin to output for TXD
	DDRD |= BUZZER_MASK;	// set pin to output for BUZZER

	PORTD |= CLR_MASK;		// Enable pullup for button
	PORTD |= START_MASK;	// Enable pullup for button
	PORTD |= MEM3_MASK;		// Enable pullup for button
	PORTD |= MEM2_MASK;		// Enable pullup for button
	PORTD |= MEM1_MASK;		// Enable pullup for button
}

void delay_ms(unsigned int delay) {
	unsigned int now, end;

	now = time_ms;
	if (TCNT1 >= 500) now++;	// delay another ms if count is past halfway

	end = now + delay;			// find end time (overflow is ok)

	while (time_ms != end);		// wait until the end time is reached
}
