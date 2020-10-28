#ifndef COMMON_H_
#define COMMON_H_

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

typedef struct {
	unsigned char type;
	unsigned char x_start;
	unsigned char x_end;
	unsigned int y_start;
	unsigned int y_end;
	unsigned int num_bytes;
} digit_t;

enum state_e {
	STATE_IDLE_SLEEP,	// Not in use, minimise power
	STATE_STOPPED,		// In use but not counting (time being set etc)
	STATE_ACTIVE		// In use, counting
};

void init_pins(void);
void init_pin_interrupts(void);
void delay_ms(unsigned int delay);

#endif
