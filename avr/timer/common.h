#ifndef COMMON_H_
#define COMMON_H_

#define BUSY_PIN	1
#define RST_PIN		2
#define CMD_PIN		3
#define CS_PIN		4
#define SDI_PIN		5
#define SCK_PIN		7

#define BUSY_MASK	(1<<BUSY_PIN)
#define RST_MASK	(1<<RST_PIN)
#define CMD_MASK	(1<<CMD_PIN)
#define CS_MASK		(1<<CS_PIN)
#define SDI_MASK	(1<<SDI_PIN)
#define SCK_MASK	(1<<SCK_PIN)

#define DIGIT_SMALL		0
#define DIGIT_BIG		1
#define DIGIT_COLON		2
	
typedef struct {
	unsigned char type;
	unsigned char x_start;
	unsigned char x_end;
	unsigned int y_start;
	unsigned int y_end;
	unsigned int num_bytes;
} digit_t;

void delay_ms(unsigned int delay);


#endif
