#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "epaper.h"
#include "timer_funcs.h"

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

extern digit_t hr01, chm, min10, min01, cms, sec10, sec01;

timer_t timer;

volatile unsigned int time_ms;	// For general timing
volatile unsigned char flg;		// General purpose flags

int main(void) {

	init_pins();	// Pin setup (direction and pullups)

	// TIMER1 setup (1 ms interrupt)
	TCCR1B = (1<<WGM12) | (1<<CS11);	// CTC mode, prescaler /8 (1 MHz count)
	OCR1A = 999;						// MAX set for 1 ms overflow
	TIMSK1 = (1<<OCIE1A);				// Enable interrupt on compare match

	// SPI setup
	SPCR = (1<<SPE) | (1<<MSTR);	// /4 prescaler (2 MHz), master mode
	SPSR = (1<<SPI2X);				// 2x speed (4 MHz)

	// Initial state
	PORTB |= CS_MASK;		// CS high
	//PORTC |= MOFF_MASK;	// Disable microphone

	// clear count
	timer.hr01 = 0;
	timer.min10 = 0;
	timer.min01 = 0;
	timer.sec10 = 0;
	timer.sec01 = 0;

	init_digits();

	time_ms = 0;
	flg = 0;

	sei();		// Enable interrupts

	while (PINC & START_MASK);	// wait for start button

	ep_init_hw();
	delay_ms(1);
	ep_set_all_white();
	delay_ms(1);
	ep_update_display();
	ep_set_all_white();

	while (1) {

	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	time_ms++;

	//if (time_ms%1000 == 0) flg |= FLAG_1S;

	if (time_ms%1000 == 0) PORTC ^= LED_MASK;
}
