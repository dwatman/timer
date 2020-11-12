#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"
#include "buffers.h"

circ_buf_spi buf_spi;
circ_buf_uart buf_uart;

extern volatile enum spi_state_e spi_state;		// State of SPI transfers
extern volatile enum uart_state_e uart_state;	// State of UART transfers

// Report how many bytes are in the UART buffer
uint16_t inbuf_uart(circ_buf_uart *buf) {
	return (buf->head - buf->tail) & (UART_BUF_SIZE-1);	// Wrap to buffer limits
}

// Write 1 byte to the UART buffer
uint8_t tobuf_uart(circ_buf_uart *buf, char data) {
	if (buf->head == buf->tail)	// Buffer full
		return 0;	// No data written

	buf->data[buf->head++] = data;	// Add to buffer
	buf->head &= (UART_BUF_SIZE-1);		// Wrap to buffer limits
	return 1;	// Byte written
}

// Read 1 byte from the UART buffer
uint8_t getbuf_uart(circ_buf_uart *buf) {
	uint16_t tmp;

	if (inbuf_uart(buf)) {
		tmp = buf->data[buf->tail++];
		buf->tail &= (UART_BUF_SIZE-1);	// Wrap to buffer limits
	}
	else tmp = 0;	// Return 0 if buffer is empty

	return tmp;
}

void uart_add_buf(char *data, uint8_t length) {
	uint8_t i, n;

	cli();	// Disable interrupts

	//if (uart_state == UART_STATE_OFF) {	// UART disabled, need to turn it on first
		// Enable UART
	//}

	for (i=0; i<length; i++) {
		n = tobuf_uart(&buf_uart, data[i]);	// Add to buffer
		if (n == 0) break;					// Exit loop if buffer is full
	}
	uart_state = UART_STATE_ACTIVE;

	sei();	// Enable interrupts

	// UCSR0B |= (1<<UDRIE0);	// Enable interrupt on Data Register Empty (will happen immediately if idle)

}

/*
void spi_add_buf(uint8_t data, uint8_t type) {
	if (spi_buf.last >= (SPI_BUF_SIZE-1))
		return;	// Don't overrun buffer

	//cli();	// Disable interrupts
	spi_buf.type[spi_buf.last] = type;	// Set type
	spi_buf.data[spi_buf.last] = data;	// Add data to buffer
	spi_buf.last++;
	//sei();	// Enable interrupts

	SPCR |= (1<<SPIE);	// Enable interrupt
}
*/
