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
	3	I	BTN_SEC01	PCINT3
	4	I	BTN_SEC10	PCINT4
	5	I	BTN_MIN01	PCINT5
	6	I	BTN_MIN10	PCINT6
	7	I	BTN_HR		PCINT7
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
	2	I	BTN_CLR		INT0
	3	I	BTN_START	INT1
	4	I	BTN_MEM3	PCINT28
	5	I	BTN_MEM2	PCINT29
	6	I	BTN_MEM1	PCINT30
	7	O	BUZZER

Interrupts:
	TIMER1_COMPA	millisecond timer?
	TIMER2_COMPA	RTC timer
*/

extern digit_t hr01, chm, min10, min01, cms, sec10, sec01;

timer_t timer;

volatile unsigned int time_ms;	// For general timing
volatile unsigned char flg;		// General purpose flags
volatile enum state_e state;	// Operating state

int main(void) {
	int i;

	init_pins();			// Pin setup (direction and pullups)
	init_pin_interrupts();	// Set interrupts for buttons

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

	timer_clear(&timer);	// Clear timer count

	init_digits();

	time_ms = 0;
	flg = 0;
	state = STATE_STOPPED;

	sei();		// Enable interrupts

	while (PIND & START_MASK);	// wait for start button

	ep_init_hw();		// Initialise display for full refresh
	delay_ms(1);
	ep_set_all_white();
	delay_ms(1);
	ep_update_display();	// Update display (full refresh)
	ep_set_all_white();

	while (PIND & START_MASK);	// wait for start button

	ep_init_part();		// Initialise display for partial refresh
	delay_ms(1);

	for (i=0; i<70; i++) {
		if ((i == 0) || (i == 1)) {	// Do twice to cover both buffers
			ep_set_num(&cms, 0);	// Draw colon between minutes and seconds
			ep_set_num(&chm, 0);	// Draw colon between hours and minutes
		}
		ep_set_num(&sec01, timer.sec01);
		ep_set_num(&sec10, timer.sec10);
		ep_set_num(&min01, timer.min01);
		ep_set_num(&min10, timer.min10);
		ep_set_num(&hr01, timer.hr01);
		ep_update_display_partial();	// Update display (partial refresh)
		timer.sec01++;
		timer_check_digits(&timer);
	}

	while (PIND & START_MASK);	// wait for start button

	ep_init_hw();	// Initialise display for full refresh
	ep_set_all_white();
	delay_ms(1);

	ep_update_display();	// Update display (full refresh)
	delay_ms(1);

	delay_ms(5000);

	ep_deepsleep();		// Enter deep sleep mode


	while (1) {

	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	time_ms++;

	//if (time_ms%1000 == 0) flg |= FLAG_1S;

	if (time_ms%1000 == 0) PORTC ^= LED_MASK;
}

// External Interrupt Request 0 (CLR button)
ISR(INT0_vect) {
	timer_clear(&timer);	// Clear timer count

	PORTC ^= LED_MASK;
}

// External Interrupt Request 1 (START button)
ISR(INT1_vect) {
	PORTC ^= LED_MASK;
}

// Pin Change Interrupt Request 0 (digit buttons)
ISR(PCINT0_vect) {
	unsigned char buttons;

	buttons = PINA;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (SEC01_MASK | SEC10_MASK | MIN01_MASK | MIN10_MASK | HR_MASK);	// Mask for button pins

	switch (buttons) {
		case SEC01_MASK: timer.sec01++; timer_check_digits(&timer); break;
		case SEC10_MASK: timer.sec10++; timer_check_digits(&timer); break;
		case MIN01_MASK: timer.min01++; timer_check_digits(&timer); break;
		case MIN10_MASK: timer.min10++; timer_check_digits(&timer); break;
		case HR_MASK:    timer.hr01++;  timer_check_digits(&timer); break;
		default: return;	// Do nothing if no buttons or multiple buttons are pressed
	}

	// This only runs if a valid case occurred
	PORTC ^= LED_MASK;
}

// Pin Change Interrupt Request 3 (memory buttons)
ISR(PCINT3_vect) {
	unsigned char buttons;

	buttons = PIND;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (MEM1_MASK | MEM2_MASK | MEM3_MASK);	// Mask for button pins

	switch (buttons) {
		case MEM1_MASK: timer_set_mem1(&timer); break;
		case MEM2_MASK: timer_set_mem2(&timer); break;
		case MEM3_MASK: timer_set_mem3(&timer); break;
		default: return;	// Do nothing if no buttons or multiple buttons are pressed
	}

	// This only runs if a valid case occurred
	PORTC ^= LED_MASK;
}
