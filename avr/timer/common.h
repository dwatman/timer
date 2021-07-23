#ifndef COMMON_H_
#define COMMON_H_

// Adjustable parameters
#define BTN_SHORT_PRESS	5		// 50 ms threshold for short press (all buttons)
#define HMS_LONG_PRESS	50		// 500 ms threshold for hour/minute/second buttons to activate fast counting
#define MEM_LONG_PRESS	100		// 1000 ms threshold for memory buttons to activate edit mode

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
#define GFX_DIGIT_SMALL		0
#define GFX_DIGIT_BIG		1
#define GFX_DIGIT_COLON		2
#define GFX_MEM_TXT			3

// Main loop flags
#define FLG_UPD		0x01
#define FLG_TURNOFF	0x10

// Bit masks for tracking which parts of the display need updating
#define EPD_UPD_S01	0x0001	// Seconds 1 digit
#define EPD_UPD_S10	0x0002	// Seconds 10 digit
#define EPD_UPD_M01	0x0004	// Minutes 1 digit
#define EPD_UPD_M10	0x0008	// Minutes 10 digit
#define EPD_UPD_H01	0x0010	// Hours 1 digit
#define EPD_UPD_CMS	0x0020	// Colon between minutes and seconds
#define EPD_UPD_CHM	0x0040	// Colon between hours and minutes
#define EPD_UPD_MEM	0x0080	// MEMx edit mode marker
//#define EPD_UPD_BAT	0x	// Low battery indication
//#define EPD_UPD_END	0x	// Alarm end
#define EPD_UPD_WHI	0x8000	// White background
#define EPD_UPD_ALL	0x80FF	// All parts of the display

#define SPI_TYPE_CMD	0
#define SPI_TYPE_DATA	1

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
	STATE_SET_MEM1,		// Setting MEM1 time
	STATE_SET_MEM2,		// Setting MEM2 time
	STATE_SET_MEM3,		// Setting MEM3 time
	STATE_ACTIVE,		// In use, counting
	STATE_END_BEEP,		// Timer expired, making alarm noise
	STATE_END_DONE		// Timer expired, alarm noise finished, showing end display
};

// EPD state
enum epd_state_e {
	EPD_STATE_DSLEEP,		// Deep sleep mode, minimise power
	EPD_STATE_INIT,			// Initialising
	EPD_STATE_IDLE_FULL,	// Ready for commands (full refresh mode)
	EPD_STATE_IDLE_PART,	// Ready for commands (partial refresh mode)
	EPD_STATE_ACTIVE,		// Transfer(s) in progress
	EPD_STATE_BUSY			// Busy internally (eg refreshing display)
};

// SPI state
enum spi_state_e {
	SPI_STATE_OFF,		// Disabled for lower power
	SPI_STATE_IDLE,		// Ready, no transfer in progress
	SPI_STATE_ACTIVE	// Transfer(s) in progress
	//SPI_STATE_DONE		// All queued transfers complete
};

// UART state
enum uart_state_e {
	UART_STATE_OFF,		// Disabled for lower power
	UART_STATE_IDLE,	// Ready, no transfer in progress
	UART_STATE_ACTIVE	// Transfer(s) in progress
	//UART_STATE_DONE		// All queued transfers complete
};

void init_pins(void);
void init_pin_interrupts(void);
uint8_t check_lowbat(void);
void delay_ms(uint16_t delay);

#endif
