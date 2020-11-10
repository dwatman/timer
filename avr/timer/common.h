#ifndef COMMON_H_
#define COMMON_H_

// Adjustable parameters
#define DEBOUNCE_TIMEOUT	110		// (ms) Ignore all buttons for a short time after each press (long for display update time?)
#define MEM_LONG_PRESS		1000	// (ms) Hold time for memory buttons to activate edit mode
#define HMS_LONG_PRESS		500		// (ms) Hold time for hour/minute/second buttons to activate fast counting

// PORTA pin definitions
#define MBLANK_PIN	2
#define SEC01_PIN	3
#define SEC10_PIN	4
#define MIN01_PIN	5
#define MIN10_PIN	6
#define HR_PIN		7

#define MBLANK_MASK	(1<<MBLANK_PIN)
#define SEC01_MASK	(1<<SEC01_PIN)
#define SEC10_MASK	(1<<SEC10_PIN)
#define MIN01_MASK	(1<<MIN01_PIN)
#define MIN10_MASK	(1<<MIN10_PIN)
#define HR_MASK		(1<<HR_PIN)

// PORTB pin definitions
#define LOWBAT_PIN	0
#define BUSY_PIN	1
#define RST_PIN		2
#define CMD_PIN		3
#define CS_PIN		4

#define LOWBAT_MASK	(1<<LOWBAT_PIN)
#define BUSY_MASK	(1<<BUSY_PIN)
#define RST_MASK	(1<<RST_PIN)
#define CMD_MASK	(1<<CMD_PIN)
#define CS_MASK		(1<<CS_PIN)
#define MOSI_MASK	(1<<MOSI_BIT)	// MOSI_BIT predefined
#define SCK_MASK	(1<<SCK_BIT)	// SCK_BIT predefined

// PORTC pin definitions
#define LED_PIN		0
#define MOFF_PIN	1

#define LED_MASK	(1<<LED_PIN)
#define MOFF_MASK	(1<<MOFF_PIN)

// PORTD pin definitions
#define CLR_PIN		2
#define START_PIN	3
#define MEM3_PIN	4
#define MEM2_PIN	5
#define MEM1_PIN	6
#define BUZZER_PIN	7

#define TXD_MASK	(1<<TXD_BIT)	// TXD_BIT predefined
#define CLR_MASK	(1<<CLR_PIN)
#define START_MASK	(1<<START_PIN)
#define MEM3_MASK	(1<<MEM3_PIN)
#define MEM2_MASK	(1<<MEM2_PIN)
#define MEM1_MASK	(1<<MEM1_PIN)
#define BUZZER_MASK	(1<<BUZZER_PIN)

// Digit types
#define DIGIT_SMALL		0
#define DIGIT_BIG		1
#define DIGIT_COLON		2

// Main loop flags
#define FLG_UPD	0x01

// Bit masks for tracking which parts of the display have need updating
#define BIT_S01	0x01
#define BIT_S10	0x02
#define BIT_M01	0x04
#define BIT_M10	0x08
#define BIT_H01	0x10
#define BIT_CMS	0x20
#define BIT_CHM	0x40
#define BIT_WHI	0x80

#define SPI_BUF_SIZE	64
#define UART_BUF_SIZE	64

#define SPI_TYPE_CMD	0
#define SPI_TYPE_DATA	1

typedef struct {
	uint8_t type[SPI_BUF_SIZE];	// Command(0)/Data(1) for each byte in the buffer
	uint8_t data[SPI_BUF_SIZE];	// Buffer for queueing multiple bytes
	volatile uint8_t next;		// Next byte to send
	uint8_t last;				// Last byte to send
} spi_buf_t;

typedef struct {
	char data[SPI_BUF_SIZE];	// Buffer for queueing multiple bytes
	volatile uint8_t next;		// Next byte to send
	uint8_t last;				// Last byte to send
} uart_buf_t;

typedef struct {
	uint8_t type;
	uint8_t x_start;
	uint8_t x_end;
	uint16_t y_start;
	uint16_t y_end;
	uint16_t num_bytes;
} digit_t;

enum state_e {
	STATE_IDLE_SLEEP,	// Not in use, minimise power
	STATE_STOPPED,		// In use but not counting (time being set etc)
	STATE_ACTIVE		// In use, counting
};

// EPD state
enum epd_state_e {
	EPD_STATE_DSLEEP,	// Deep sleep mode, minimise power
	EPD_STATE_INIT,		// Initialising
	EPD_STATE_IDLE,		// Ready for commands
	EPD_STATE_ACTIVE,	// Transfer(s) in progress
	EPD_STATE_BUSY		// Busy internally (eg refreshing display)
};

// SPI state
enum spi_state_e {
	SPI_STATE_IDLE,		// Ready, no transfer in progress
	SPI_STATE_ACTIVE,	// Transfer(s) in progress
	SPI_STATE_DONE		// All queued transfers complete
};

void init_pins(void);
void init_pin_interrupts(void);
void delay_ms(uint16_t delay);

#endif
