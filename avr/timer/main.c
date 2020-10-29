#include <stdio.h>		// For sprintf
#include <string.h>		// For strlen
#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "epaper.h"
#include "timer_funcs.h"

/*
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
	1	I	BUSY		PCINT9
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

Timer usage:
	Timer 1		1ms counter?
	Timer 2		RTC counter

Interrupts:
	TIMER1_COMPA	millisecond timer?
	TIMER2_OVF		RTC timer
	INT0			CLR button
	INT1			START button
	PCINT0			Digit buttons
	PCINT1			EPD BUSY signal
	PCINT3			Memory buttons
*/

extern digit_t hr01, chm, min10, min01, cms, sec10, sec01;

timer_t count_time;	// ? volatile

spi_buf_t spi_buf;		// Buffer for queueing SPI data
uart_buf_t uart_buf;	// Buffer for queueing UART data

volatile uint16_t time_ms;		// For general timing
volatile uint8_t flg;			// General purpose flags
volatile enum state_e state;	// Operating state

int main(void) {
	init_pins();			// Pin setup (direction and pullups)
	init_pin_interrupts();	// Set interrupts for buttons

	// TIMER1 setup (1 ms interrupt)
	TCCR1B = (1<<WGM12) | (1<<CS11);	// CTC mode, prescaler /8 (1 MHz count)
	OCR1A = 999;						// MAX set for 1 ms overflow
	TIMSK1 = (1<<OCIE1A);				// Enable interrupt on compare match

	// TIMER2 setup (async from external 32.768kHz crystal)
	ASSR = (1<<AS2);				// Set async mode
	TCCR2B = (1<<CS22) | (1<<CS20);	// Prescaler /128 (256 Hz count)
	while (ASSR & (1<<TCR2BUB));	// Wait for buffer sync
	TIFR2 = 0xFF;					// Clear possible interrupt flags
	TIMSK2 = (1<<TOIE2);			// Enable interrupt on overflow

	// SPI setup
	SPCR = (1<<SPE) | (1<<MSTR);	// /4 prescaler (2 MHz), master mode
	SPSR = (1<<SPI2X);				// 2x speed (4 MHz)

	// USART SETUP
	UCSR0B = (1<<TXEN0);				// Enable USART transmitter
	UCSR0A = (1<<U2X0);					// Use 2x transmission mode
	UBRR0 = 25;							// Approx 38400 (+0.16%)
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	// 8 data bits, 1 stop bit, no parity

	// Initial state
	PORTB |= CS_MASK;		// CS high
	//PORTC |= MOFF_MASK;	// Disable microphone

	timer_clear(&count_time);	// Clear timer count

	// Clear SPI buffer
	spi_buf.data[0] = 0;
	spi_buf.next = 1;
	spi_buf.last = 0;

	// Clear UART buffer
	uart_buf.data[0] = 0;
	uart_buf.next = 1;
	uart_buf.last = 0;

	init_digits();

	time_ms = 0;
	flg = 0;
	state = STATE_STOPPED;

	sei();		// Enable interrupts

	ep_init_hw();			// Initialise display for full refresh
	delay_ms(1);
	ep_set_all_white();		// Clear display buffer
	delay_ms(1);
	ep_update_display();	// Update display (full refresh)
	ep_set_all_white();		// Clear second display buffer
	ep_deepsleep();			// Enter deep sleep mode

	sprintf(uart_buf.data, "test %u\n", 7);
	cli();	// Disable interrupts
	uart_buf.next = 0;
	uart_buf.last = strlen(uart_buf.data) - 1;
	sei();	// Enable interrupts
	UCSR0B |= (1<<UDRIE0);	// Enable interrupt on Data Register Empty (will happen immediately)

	while (1) {
		if (flg & FLG_UPD) {
			ep_init_part();		// Initialise display for partial refresh
			delay_ms(1);
			ep_set_num(&sec01, count_time.sec01);
			ep_set_num(&sec10, count_time.sec10);
			ep_set_num(&min01, count_time.min01);
			ep_set_num(&min10, count_time.min10);
			ep_set_num(&hr01, count_time.hr01);
			ep_update_display_partial();	// Update display (partial refresh)
			ep_deepsleep();		// Enter deep sleep mode

			flg &= ~FLG_UPD;	// Clear flag
		}
	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	time_ms++;

	//if (time_ms%1000 == 0) flg |= FLAG_1S;

	//if (time_ms%1000 == 0) PORTC ^= LED_MASK;
}

// Timer/Counter2 Overflow
ISR(TIMER2_OVF_vect) {
	PORTC ^= LED_MASK;
}

// External Interrupt Request 0 (CLR button)
ISR(INT0_vect) {
	timer_clear(&count_time);	// Clear timer count
	flg |= FLG_UPD;		// Set flag to update display

	PORTC ^= LED_MASK;
}

// External Interrupt Request 1 (START button)
ISR(INT1_vect) {
	PORTC ^= LED_MASK;
}

// Pin Change Interrupt Request 0 (digit buttons)
ISR(PCINT0_vect) {
	uint8_t buttons;

	buttons = PINA;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (SEC01_MASK | SEC10_MASK | MIN01_MASK | MIN10_MASK | HR_MASK);	// Mask for button pins

	switch (buttons) {
		case SEC01_MASK: count_time.sec01++; timer_check_digits(&count_time); break;
		case SEC10_MASK: count_time.sec10++; timer_check_digits(&count_time); break;
		case MIN01_MASK: count_time.min01++; timer_check_digits(&count_time); break;
		case MIN10_MASK: count_time.min10++; timer_check_digits(&count_time); break;
		case HR_MASK:    count_time.hr01++;  timer_check_digits(&count_time); break;
		default: return;	// Do nothing if no buttons or multiple buttons are pressed
	}

	// This only runs if a valid case occurred
	flg |= FLG_UPD;		// Set flag to update display

	PORTC ^= LED_MASK;
}

// Pin Change Interrupt Request 1 (EPD busy signal)
ISR(PCINT1_vect) {
}

// Pin Change Interrupt Request 3 (memory buttons)
ISR(PCINT3_vect) {
	uint8_t buttons;

	buttons = PIND;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (MEM1_MASK | MEM2_MASK | MEM3_MASK);	// Mask for button pins

	switch (buttons) {
		case MEM1_MASK: timer_read_mem1(&count_time); break;
		case MEM2_MASK: timer_read_mem2(&count_time); break;
		case MEM3_MASK: timer_read_mem3(&count_time); break;
		default: return;	// Do nothing if no buttons or multiple buttons are pressed
	}

	// This only runs if a valid case occurred
	flg |= FLG_UPD;		// Set flag to update display

	PORTC ^= LED_MASK;
}

// SPI Serial Transfer Complete
ISR(SPI_STC_vect) {



}

// USART0 Data Register Empty
ISR(USART0_UDRE_vect) {
	// Send the next byte
	if (uart_buf.next <= uart_buf.last) {
		UDR0 = uart_buf.data[uart_buf.next];
		uart_buf.next++;
	}

	// Disable this interrupt after last byte is loaded
	if (uart_buf.next > uart_buf.last)
		UCSR0B &= ~(1<<UDRIE0);	// Disable interrupt on Data Register Empty
}
