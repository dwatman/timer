#ifndef TIMER_H_
#define TIMER_H_

typedef struct {
	unsigned char hr01;
	unsigned char min10;
	unsigned char min01;
	unsigned char sec10;
	unsigned char sec01;
} timer_t;

void inc_timer(timer_t *timer);

#endif