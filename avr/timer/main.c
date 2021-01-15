#include <stdio.h>		// For sprintf
#include <string.h>		// For strlen
#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "epaper.h"
#include "timer_funcs.h"
#include "buffers.h"

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

volatile enum spi_state_e spi_state;	// State of SPI transfers

char uart_tmp[64];						// Temporary buffer for writing strings before adding to buffer
circ_buf_uart uart_buf;					// Buffer for queueing UART data
volatile enum uart_state_e uart_state;	// State of UART transfers

volatile uint16_t time_ms;		// For general timing
volatile uint8_t flg;			// General purpose flags
volatile enum state_e state;	// Operating state

volatile uint8_t btn_debounce;	// Counter for debounce delay
volatile uint16_t btn_hold_ms;	// How long a button has been held for

volatile uint8_t ep_upd_flg;	// Indicates which areas of the display need updating
volatile uint8_t ep_upd_flg2;	// For the other image buffer in the display
volatile enum epd_state_e epd_state;	// State of display

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
	//PORTC |= MOFF_MASK;		// Disable microphone
	PORTD &= ~BUZZER_MASK;	// Buzzer off

	timer_clear(&count_time);	// Clear timer count

	// Set SPI state
	PORTB |= CS_MASK;		// CS high
	spi_state = SPI_STATE_IDLE;

	// Clear UART buffer
	uart_buf.head = 0;
	uart_buf.tail = 0;
	uart_state = UART_STATE_IDLE;

	init_digits();

	time_ms = 0;
	flg = 0;
	state = STATE_STOPPED;

	btn_debounce = 0;
	btn_hold_ms = 0;

	ep_upd_flg = EPD_UPD_ALL;		// All areas need updating
	ep_upd_flg2 = EPD_UPD_ALL;		// Both buffers
	epd_state = EPD_STATE_DSLEEP;	// Display starts disabled

	sei();		// Enable interrupts

	ep_init_hw();			// Initialise display for full refresh
	delay_ms(1);
	ep_set_all_white();		// Clear display buffer
	delay_ms(1);
	ep_update_display();	// Update display (full refresh)
	ep_set_all_white();		// Clear second display buffer
	ep_deepsleep();			// Enter deep sleep mode

	sprintf(uart_tmp, "\nmain loop start\n");
	uart_add_buf(uart_tmp, strlen(uart_tmp));

	sprintf(uart_tmp, "bat state %u\n", check_lowbat());
	uart_add_buf(uart_tmp, strlen(uart_tmp));

	while (1) {
		if (flg & FLG_UPD) {

			if (epd_state != EPD_STATE_IDLE_PART) {		// Initialise display for partial refresh
				ep_init_part();		// Initialise display for partial refresh
				epd_state = EPD_STATE_IDLE_PART;
				delay_ms(1);
			}

			if (ep_upd_flg & EPD_UPD_WHI) {		// Clear background to white
				ep_upd_flg &= ~EPD_UPD_WHI;		// Clear flag immediately so new changes will be detected
				ep_set_all_white();
			}
			if (ep_upd_flg & EPD_UPD_CMS) {		// Update colon between minutes and seconds
				ep_upd_flg &= ~EPD_UPD_CMS;		// Clear flag immediately so new changes will be detected
				ep_set_num(&cms, 0);
			}
			if (ep_upd_flg & EPD_UPD_CHM) {		// Update colon between hours and minutes
				ep_upd_flg &= ~EPD_UPD_CHM;		// Clear flag immediately so new changes will be detected
				ep_set_num(&chm, 0);
			}
			if (ep_upd_flg & EPD_UPD_S01) {		// Update seconds 01
				ep_upd_flg &= ~EPD_UPD_S01;		// Clear flag immediately so new changes will be detected
				ep_set_num(&sec01, count_time.sec01);
			}
			if (ep_upd_flg & EPD_UPD_S10) {		// Update seconds 10
				ep_upd_flg &= ~EPD_UPD_S10;		// Clear flag immediately so new changes will be detected
				ep_set_num(&sec10, count_time.sec10);
			}
			if (ep_upd_flg & EPD_UPD_M01) {		// Update minutes 01
				ep_upd_flg &= ~EPD_UPD_M01;		// Clear flag immediately so new changes will be detected
				ep_set_num(&min01, count_time.min01);
			}
			if (ep_upd_flg & EPD_UPD_M10) {		// Update minutes 10
				ep_upd_flg &= ~EPD_UPD_M10;		// Clear flag immediately so new changes will be detected
				ep_set_num(&min10, count_time.min10);
			}
			if (ep_upd_flg & EPD_UPD_H01) {		// Update hours 01
				ep_upd_flg &= ~EPD_UPD_H01;		// Clear flag immediately so new changes will be detected
				ep_set_num(&hr01, count_time.hr01);
			}

			ep_update_display_partial();	// Update display (partial refresh)

			// If there have been no display changes while refreshing, clear flag and sleep display
			// Otherwise this will run again next loop
			if (ep_upd_flg == 0) {
				ep_deepsleep();		// Enter deep sleep mode
				swap_upd_buffers();
				epd_state = EPD_STATE_DSLEEP;
				flg &= ~FLG_UPD;	// Clear flag
			}
		}
	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	time_ms++;

	if (btn_debounce > 0) btn_debounce--;	// Decrement debounce counter if active

	if (state == STATE_END_BEEP) {	// Beeping
		if (time_ms%4 < 2)
			PORTD |= BUZZER_MASK;	// Buzzer on
		else
			PORTD &= ~BUZZER_MASK;	// Buzzer off
	}

	//if (time_ms%1000 == 0) flg |= FLAG_1S;

	//if (time_ms%1000 == 0) PORTC ^= LED_MASK;
}

// Timer/Counter2 Overflow
ISR(TIMER2_OVF_vect) {
	uint8_t tmp;

	if (state == STATE_ACTIVE) {	// Counting down
		tmp = timer_count_down(&count_time);
		if (tmp != 0) {	// Count reached zero
			state = STATE_END_BEEP;
		}
		flg |= FLG_UPD;		// Set flag to update display
	}
	else if (state == STATE_END_BEEP) {	// Beeping
		//PORTC &= ~MOFF_MASK;	// Enable microphone
		//PORTC |= MOFF_MASK;	// Disable microphone
		PORTD &= ~BUZZER_MASK;	// Buzzer off
		state = STATE_END_DONE;
	}
	else if (state == STATE_END_DONE) {	// Finished beeping
		state = STATE_STOPPED;
	}

	//PORTC ^= LED_MASK;
}

// External Interrupt Request 0 (CLR button, press only)
ISR(INT0_vect) {
	if (btn_debounce > 0) return;			// Ignore buttons during debounce period
	else btn_debounce = DEBOUNCE_TIMEOUT;	// Otherwise set debounce timer

	switch (state) {
		case STATE_IDLE_SLEEP:
			// Wake up
			state = STATE_STOPPED;
		case STATE_STOPPED:
		case STATE_SET_MEM1:
		case STATE_SET_MEM2:
		case STATE_SET_MEM3:
			timer_clear(&count_time);	// Clear timer count
			flg |= FLG_UPD;		// Set flag to update display
			break;
		case STATE_ACTIVE: break;
		case STATE_END_BEEP:
			// Stop beeping
		case STATE_END_DONE:
			// Clear done indication
			state = STATE_STOPPED;
			break;
	}

	//PORTC ^= LED_MASK;
}

// External Interrupt Request 1 (START button, press only)
ISR(INT1_vect) {
	if (btn_debounce > 0) return;			// Ignore buttons during debounce period
	else btn_debounce = DEBOUNCE_TIMEOUT;	// Otherwise set debounce timer

	sprintf(uart_tmp, "BTN_START\n");
	uart_add_buf(uart_tmp, strlen(uart_tmp));

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
			// Remove MEM1 set from display
			flg |= FLG_UPD;		// Set flag to update display
			state = STATE_STOPPED;
			break;
		case STATE_SET_MEM2:
			timer_write_mem2(&count_time);	// Store current count in MEM2
			// Remove MEM2 set from display
			flg |= FLG_UPD;		// Set flag to update display
			state = STATE_STOPPED;
			break;
		case STATE_SET_MEM3:
			timer_write_mem3(&count_time);	// Store current count in MEM3
			// Remove MEM3 set from display
			flg |= FLG_UPD;		// Set flag to update display
			state = STATE_STOPPED;
			break;
		case STATE_ACTIVE:
			state = STATE_STOPPED;
			break;
		case STATE_END_BEEP:
			// Stop beeping
		case STATE_END_DONE:
			// Clear done indication
			state = STATE_STOPPED;
			break;
	}

	//PORTC ^= LED_MASK;
}

// Pin Change Interrupt Request 1 (EPD busy signal)
ISR(PCINT1_vect) {
}

// Pin Change Interrupt Request 0 (digit buttons, press and release)
ISR(PCINT0_vect) {
	uint8_t buttons;

	if (btn_debounce > 0) return;			// Ignore buttons during debounce period
	else btn_debounce = DEBOUNCE_TIMEOUT;	// Otherwise set debounce timer (on both edges to suppress short spikes too)

	buttons = PINA;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (SEC01_MASK | SEC10_MASK | MIN01_MASK | MIN10_MASK | HR_MASK);	// Mask for button pins

	switch (buttons) {
		case SEC01_MASK: timer_inc_sec01(&count_time); break;
		case SEC10_MASK: timer_inc_sec10(&count_time); break;
		case MIN01_MASK: timer_inc_min01(&count_time); break;
		case MIN10_MASK: timer_inc_min10(&count_time); break;
		case HR_MASK:    timer_inc_hr01(&count_time);  break;
		default: return;	// Do nothing if no buttons or multiple buttons are pressed (includes button release edges)
	}

	// This only runs if a valid case occurred
	flg |= FLG_UPD;		// Set flag to update display
}

// Pin Change Interrupt Request 3 (memory buttons, press and release)
ISR(PCINT3_vect) {
	uint8_t buttons;

	if (btn_debounce > 0) return;			// Ignore buttons during debounce period
	else btn_debounce = DEBOUNCE_TIMEOUT;	// Otherwise set debounce timer (on both edges to suppress short spikes too)

	buttons = PIND;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (MEM1_MASK | MEM2_MASK | MEM3_MASK);	// Mask for button pins

	switch (buttons) {
		case MEM1_MASK: timer_read_mem1(&count_time); break;
		case MEM2_MASK: timer_read_mem2(&count_time); break;
		case MEM3_MASK: timer_read_mem3(&count_time); break;
		default: return;	// Do nothing if no buttons or multiple buttons are pressed (includes button release edges)
	}

	// This only runs if a valid case occurred
	flg |= FLG_UPD;						// Set flag to update display

	//PORTC ^= LED_MASK;
}

// USART0 Data Register Empty
ISR(USART0_UDRE_vect) {
	if (inbuf_uart(&uart_buf) > 0) {
		UDR0 = getbuf_uart(&uart_buf);	// Send the next byte if available
	}
	else {							// Buffer is empty
		UCSR0B &= ~(1<<UDRIE0);			// Disable this interrupt
		uart_state = UART_STATE_IDLE;
	}
}
