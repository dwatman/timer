#ifndef BUFFERS_H_
#define BUFFERS_H_

#define UART_BUF_SIZE	256	// (power of 2, max 32768)

typedef struct {
	volatile uint16_t head;	// Next FREE spot
	volatile uint16_t tail;	// Oldest DATA
	char data[UART_BUF_SIZE];
} circ_buf_uart;

uint16_t inbuf_uart(circ_buf_uart *buf);
uint8_t tobuf_uart(circ_buf_uart *buf, char data);
uint8_t getbuf_uart(circ_buf_uart *buf);
void uart_add_buf(char *data, uint8_t length);

#endif
