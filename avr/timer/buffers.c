#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "common.h"
#include "buffers.h"

circ_buf_uart uart_buf;

extern volatile enum uart_state_e uart_state;	// State of UART transfers

// Report how many bytes are in the UART buffer
uint16_t inbuf_uart(circ_buf_uart *buf) {
	return (buf->head - buf->tail) & (UART_BUF_SIZE-1);	// Wrap to buffer limits
}

// Write 1 byte to the UART buffer
uint8_t tobuf_uart(circ_buf_uart *buf, char data) {

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {		// Make sure the buffer doesn't change while modifying

		if (inbuf_uart(buf) == UART_BUF_SIZE-1)	// Buffer full
			return 0;							// No data written

		buf->data[buf->head] = data;					// Add data to buffer
		buf->head = (++buf->head) & (UART_BUF_SIZE-1);	// Increment, wrap to buffer limits

	}
	return 1;	// Byte written
}

// Read 1 byte from the UART buffer
uint8_t getbuf_uart(circ_buf_uart *buf) {
	uint8_t tmp;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {		// Make sure the buffer doesn't change while modifying

		if (inbuf_uart(buf)) {
			tmp = buf->data[buf->tail];						// Get oldest data
			buf->tail = (++buf->tail) & (UART_BUF_SIZE-1);	// Increment, wrap to buffer limits
		}
		else tmp = 0;	// Return 0 if buffer is empty

	}
	return tmp;
}

void uart_add_buf(char *data, uint8_t length) {
	uint8_t i, n;

	//if (uart_state == UART_STATE_OFF) {	// UART disabled, need to turn it on first
		// Enable UART
	//}

	for (i=0; i<length; i++) {
		n = tobuf_uart(&uart_buf, data[i]);	// Add to buffer
		if (n == 0) break;					// Exit loop if buffer is full
	}
	uart_state = UART_STATE_ACTIVE;

	UCSR0B |= (1<<UDRIE0);	// Enable interrupt on Data Register Empty (will happen immediately if idle)
}
