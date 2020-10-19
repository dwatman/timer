#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"

/*
Timer usage:
	Timer 1		1ms counter?
	Timer 2		RTC counter

Pin usage:
PORTA:
	0	AI	MIC_FAST
	1	AI	MIC_SLOW
	2	O	MIC_BLANK
	3	I	BTN_SEC01
	4	I	BTN_SEC10
	5	I	BTN_MIN01
	6	I	BTN_MIN10
	7	I	BTN_HR
PORTB:
	0	I	LOWBAT
	1	I	BUSY
	2	O	RST#
	3	O	DAT/CMD#
	4	O	CS#
	5	O	MOSI
	6	-
	7	O	SCK
PORTC:
	0	O	LED
	1	O	MIC_OFF
	2-5	IO	JTAG
	6-7	-	crystal
PORTD:
	0	I	RXD
	1	O	TXD
	2	I	BTN_CLR
	3	I	BTN_START
	4	I	BTN_MEM3
	5	I	BTN_MEM2
	6	I	BTN_MEM1
	7	O	BUZZER

Interrupts:
	TIMER1_COMPA	millisecond timer?
	TIMER2_COMPA	RTC timer
*/

volatile unsigned int time_ms;	// For general timing
volatile unsigned char flg;		// General purpose flags

int main(void) {
	// PIN SETUP
	DDRA |= MBLANK_MASK;	// Set pin to output for MIC_BLANK

	PORTA |= SEC01_MASK;	// Enable pullup for button
	PORTA |= SEC10_MASK;	// Enable pullup for button
	PORTA |= MIN01_MASK;	// Enable pullup for button
	PORTA |= MIN10_MASK;	// Enable pullup for button
	PORTA |= HR_MASK; 		// Enable pullup for button

	DDRB |= RST_MASK;	// set pin to output for RST#
	DDRB |= CMD_MASK;	// set pin to output for DAT/CMD#
	DDRB |= CS_MASK;	// set pin to output for CS#
	DDRB |= MOSI_MASK;	// set pin to output for MOSI
	DDRB |= SCK_MASK;	// set pin to output for SCK

	DDRC |= LED_MASK;	// set pin to output for LED
	DDRC |= MOFF_MASK;	// set pin to output for MIC_OFF

	DDRD |= TXD_MASK;		// set pin to output for TXD
	DDRD |= BUZZER_MASK;	// set pin to output for BUZZER

	PORTD |= CLR_MASK;		// Enable pullup for button
	PORTD |= START_MASK;	// Enable pullup for button
	PORTD |= MEM3_MASK;		// Enable pullup for button
	PORTD |= MEM2_MASK;		// Enable pullup for button
	PORTD |= MEM1_MASK;		// Enable pullup for button

	// INITIAL STATE
	PORTB |= RST_MASK;		// RST high
	PORTB |= CS_MASK;		// CS high

	//PORTC |= MOFF_MASK;	// Disable microphone


	// TIMER1 SETUP (1 ms interrupt)
	TCCR1B = (1<<WGM12) | (1<<CS11);	// CTC mode, prescaler /8 (1 MHz count)
	OCR1A = 999;						// MAX set for 1 ms overflow
	TIMSK1 = (1<<OCIE1A);				// Enable interrupt on compare match

	// SPI SETUP
	SPCR = (1<<SPE) | (1<<MSTR);	// /4 prescaler (2 MHz), master mode
	SPSR = (1<<SPI2X);				// 2x speed (4 MHz)

	time_ms = 0;
	flg = 0;

	sei();		// Enable interrupts


	while (1) {

	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	time_ms++;

	//if (time_ms%1000 == 0) flg |= FLAG_1S;

	if (time_ms%1000 == 0) PORTC ^= LED_MASK;
}
