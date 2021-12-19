#include <stdio.h>		// For sprintf
#include <string.h>		// For strlen
#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"
#include "epaper.h"
#include "timer_funcs.h"
#include "buffers.h"
#include "buttons.h"

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
	Timer 1		1 ms counter, changes to 250 us for beep
	Timer 2		RTC counter

Interrupts:
	TIMER1_COMPA	millisecond timer, beep timer
	TIMER2_OVF		RTC timer
	INT0			CLR button
	INT1			START button
	PCINT0			Digit buttons
	PCINT1			EPD BUSY signal
	PCINT3			Memory buttons
*/

extern digit_t digit_hr01, digit_chm, digit_min10, digit_min01, digit_cms, digit_sec10, digit_sec01;
extern digit_t digit_mem, digit_bat;

timer_t count_time;		// Counter time (volatile?)

volatile enum spi_state_e spi_state;	// State of SPI transfers

char uart_tmp[64];						// Temporary buffer for writing strings before adding to buffer
circ_buf_uart uart_buf;					// Buffer for queueing UART data
volatile enum uart_state_e uart_state;	// State of UART transfers

volatile uint16_t time_ms;		// For general timing
volatile uint8_t flg;			// General purpose flags
volatile enum state_e state;	// Operating state

volatile uint16_t ep_upd_flg;	// Indicates which areas of the display need updating
volatile uint16_t ep_upd_flg2;	// For the other image buffer in the display
volatile enum epd_state_e epd_state;	// State of display

// For generating the beep
volatile uint8_t beep_cycle;	// 1 sec beeping cycle counter
volatile uint8_t beep_period;	// pulse period for the beep (variable)

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

	init_gfx_data();

	time_ms = 0;
	flg = FLG_UPD;					// Update display on start
	state = STATE_STOPPED;

	ep_upd_flg = EPD_UPD_ALL;		// All areas need updating
	ep_upd_flg2 = EPD_UPD_ALL;		// Both buffers
	epd_state = EPD_STATE_DSLEEP;	// Display starts disabled

	sei();		// Enable interrupts

	ep_full_clear();

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
				ep_set_num(&digit_cms, 0);
			}
			if (ep_upd_flg & EPD_UPD_CHM) {		// Update colon between hours and minutes
				ep_upd_flg &= ~EPD_UPD_CHM;		// Clear flag immediately so new changes will be detected
				ep_set_num(&digit_chm, 0);
			}
			if (ep_upd_flg & EPD_UPD_S01) {		// Update seconds 01
				ep_upd_flg &= ~EPD_UPD_S01;		// Clear flag immediately so new changes will be detected
				ep_set_num(&digit_sec01, count_time.sec01);
			}
			if (ep_upd_flg & EPD_UPD_S10) {		// Update seconds 10
				ep_upd_flg &= ~EPD_UPD_S10;		// Clear flag immediately so new changes will be detected
				ep_set_num(&digit_sec10, count_time.sec10);
			}
			if (ep_upd_flg & EPD_UPD_M01) {		// Update minutes 01
				ep_upd_flg &= ~EPD_UPD_M01;		// Clear flag immediately so new changes will be detected
				ep_set_num(&digit_min01, count_time.min01);
			}
			if (ep_upd_flg & EPD_UPD_M10) {		// Update minutes 10
				ep_upd_flg &= ~EPD_UPD_M10;		// Clear flag immediately so new changes will be detected
				ep_set_num(&digit_min10, count_time.min10);
			}
			if (ep_upd_flg & EPD_UPD_H01) {		// Update hours 01
				ep_upd_flg &= ~EPD_UPD_H01;		// Clear flag immediately so new changes will be detected
				ep_set_num(&digit_hr01, count_time.hr01);
			}
			if (ep_upd_flg & EPD_UPD_MEM) {		// Update MEMx text
				ep_upd_flg &= ~EPD_UPD_MEM;		// Clear flag immediately so new changes will be detected
				switch (state) {
					case STATE_SET_MEM1: ep_set_num(&digit_mem, 1); break;
					case STATE_SET_MEM2: ep_set_num(&digit_mem, 2); break;
					case STATE_SET_MEM3: ep_set_num(&digit_mem, 3); break;
					default: ep_set_num(&digit_mem, 0);	// Clear MEMx text if not in MEM edit state
				}
			}
			if (ep_upd_flg & EPD_UPD_BAT) {		// Update LOWBAT text
				ep_upd_flg &= ~EPD_UPD_BAT;		// Clear flag immediately so new changes will be detected
				// TODO: Add check for battery level
				//ep_set_num(&digit_bat, 0);	// Clear text
				ep_set_num(&digit_bat, 1);	// Write text
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
		if (flg & FLG_TURNOFF) {
			// Fully refresh the display to all white
			ep_full_clear();
			epd_state = EPD_STATE_DSLEEP;

			sprintf(uart_tmp, "\nmain loop start\n");
			uart_add_buf(uart_tmp, strlen(uart_tmp));
			
			// go into low-power state
			//state = STATE_IDLE_SLEEP;
			
			flg &= ~FLG_TURNOFF;	// Clear flag
		}
		
	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect) {
	static uint8_t beep_count = 0;
	static uint8_t beep_phase = 0;

	time_ms++;

	if (time_ms%10 == 0) check_buttons();	// Check buttons for time pressed

	if (state == STATE_END_BEEP) {	// Beeping
		// Speaker is active on even numbers
		if (beep_cycle%2 == 0) {	// Cycles last for 1 second
			
			if (beep_count >= beep_period) {	// End of pulse
				beep_count = 0;
				beep_phase++;

				if ((beep_period > 1) && (beep_phase%BEEP_CYCLE_LEN == (BEEP_CYCLE_LEN-1)))
					beep_period--;
			}
			else {	// Counting pulse time
				beep_count++;
			}
			
			if (beep_phase%2 == 0)
				PORTD |= BUZZER_MASK;	// Buzzer on
			else
				PORTD &= ~BUZZER_MASK;	// Buzzer off 

		}
		else {
			PORTD &= ~BUZZER_MASK;	// Buzzer off for quiet period
			beep_phase = 0;
			beep_count = 0;
			beep_period = BEEP_INIT_PER;
		}
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
			beep_cycle = 0;
			beep_period = BEEP_INIT_PER;
			OCR1A = 249;	// Speed up timer for beeping (250 us tick)
			state = STATE_END_BEEP;
		}
		flg |= FLG_UPD;		// Set flag to update display
	}
	else if (state == STATE_END_BEEP) {	// Beeping
		if (beep_cycle >= (BEEP_CYCLES-1)*2) {
			//PORTC &= ~MOFF_MASK;	// Enable microphone
			//PORTC |= MOFF_MASK;	// Disable microphone
			PORTD &= ~BUZZER_MASK;	// Buzzer off
			OCR1A = 999;			// Restore timer speed for ms counting
			state = STATE_END_DONE;			
		}
		else {
			beep_cycle++;
		}		

	}
	else if (state == STATE_END_DONE) {	// Finished beeping
		state = STATE_STOPPED;
	}

	//PORTC ^= LED_MASK;
}

// External Interrupt Request 0 (CLR button, press only)
ISR(INT0_vect) {



	//PORTC ^= LED_MASK;
}

// External Interrupt Request 1 (START button, press only)
ISR(INT1_vect) {

	sprintf(uart_tmp, "BTN_START\n");
	uart_add_buf(uart_tmp, strlen(uart_tmp));



	//PORTC ^= LED_MASK;
}

// Pin Change Interrupt Request 1 (EPD busy signal)
ISR(PCINT1_vect) {
}

// Pin Change Interrupt Request 0 (digit buttons, press and release)
ISR(PCINT0_vect) {
	uint8_t buttons;

	buttons = PINA;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (SEC01_MASK | SEC10_MASK | MIN01_MASK | MIN10_MASK | HR_MASK);	// Mask for button pins

	if (buttons) {
		// Wake up ms timer for debounce timing
	}

}

// Pin Change Interrupt Request 3 (memory buttons, press and release)
ISR(PCINT3_vect) {
	uint8_t buttons;

	buttons = PIND;				// Read button inputs
	buttons = buttons ^ 0xFF;	// Invert as buttons are active low
	buttons = buttons & (MEM1_MASK | MEM2_MASK | MEM3_MASK);	// Mask for button pins

	if (buttons) {
		// Wake up ms timer for debounce timing
	}
	
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
