#ifndef TIMER_FUNCS_H_
#define TIMER_FUNCS_H_

typedef struct {
	unsigned char hr01;
	unsigned char min10;
	unsigned char min01;
	unsigned char sec10;
	unsigned char sec01;
} timer_t;

void timer_clear(timer_t *timer);
void timer_set_mem1(timer_t *timer);
void timer_set_mem2(timer_t *timer);
void timer_set_mem3(timer_t *timer);
void timer_check_digits(timer_t *timer);

#endif
