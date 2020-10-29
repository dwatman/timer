#include <avr/io.h>
#include "common.h"

extern volatile uint16_t time_ms;

extern spi_buf_t spi_buf;	// Buffer for queueing SPI data
extern uart_buf_t uart_buf;	// Buffer for queueing UART data

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

void init_pin_interrupts(void) {
	// External interrupt pins (can wake from sleep)
	EICRA = (1<<ISC11) | (1<<ISC01);	// Set INT1 and INT0 (START/CLEAR buttons) sensitive to falling edge
	EIMSK = (1<<INT1) | (1<<INT0);		// Enable interrupts for INT1 and INT0 (START/CLEAR buttons)
	EIFR = (1<<INTF1) | (1<<INTF0);		// Clear interrupt flags

	// Pin change interrupts
	// Used: 3-7, 9, 28-30 = PC3, PC1, PC0
	PCICR = (1<<PCIE3) | (1<<PCIE1) | (1<<PCIE0);			// Enable pin change interrupts
	PCMSK3 = (1<<PCINT30) | (1<<PCINT29) | (1<<PCINT28);	// Unmask interrupts for MEM1-3 buttons
	PCMSK1 = (1<<PCINT9);									// Unmask interrupt for EDP BUSY signal
	PCMSK0 = (1<<PCINT7) | (1<<PCINT6) | (1<<PCINT5);		// Unmask interrupts for digit buttons
	PCMSK0 |= (1<<PCINT4) | (1<<PCINT3);					// More digit buttons
	PCIFR = (1<<PCIF3) | (1<<PCIF0);						// Clear interrupt flags
}

void delay_ms(uint16_t delay) {
	uint16_t now, end;

	now = time_ms;
	if (TCNT1 >= 500) now++;	// delay another ms if count is past halfway

	end = now + delay;			// find end time (overflow is ok)

	while (time_ms != end);		// wait until the end time is reached
}

void spi_add_buf(uint8_t data, uint8_t type) {
	if (spi_buf.last >= (SPI_BUF_SIZE-1))
		return;	// Don't overrun buffer

	cli();	// Disable interrupts
	spi_buf.type[spi_buf.last] = type;	// Set type
	spi_buf.data[spi_buf.last] = data;	// Add data to buffer
	spi_buf.last++;
	sei();	// Enable interrupts
}
