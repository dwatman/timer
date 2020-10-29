#ifndef TIMER_FUNCS_H_
#define TIMER_FUNCS_H_

#define EEPROM_MEM1_ADDR	0x0000	// Start address of MEM1 data (5 bytes)
#define EEPROM_MEM2_ADDR	0x0008	// Start address of MEM2 data (5 bytes)
#define EEPROM_MEM3_ADDR	0x0010	// Start address of MEM3 data (5 bytes)

typedef struct {
	uint8_t hr01;
	uint8_t min10;
	uint8_t min01;
	uint8_t sec10;
	uint8_t sec01;
} timer_t;

void timer_clear(timer_t *timer);
void timer_read_mem1(timer_t *timer);
void timer_read_mem2(timer_t *timer);
void timer_read_mem3(timer_t *timer);
void timer_write_mem1(timer_t *timer);
void timer_write_mem2(timer_t *timer);
void timer_write_mem3(timer_t *timer);
void timer_check_digits(timer_t *timer);

#endif
