#ifndef EPAPER_H_
#define EPAPER_H_

#include "common.h"

#define EP_RES_X 16	// 16x8bit = 128 px
#define EP_RES_Y 296
#define EP_TOTAL_BYTES (EP_RES_X*EP_RES_Y)	// 4736

// Starting X coordinates (display top to bottom)
#define DISP_STARTX_SEC		6
#define DISP_STARTX_MIN		4
#define DISP_STARTX_HR		4
#define DISP_STARTX_CLN		7	// Colons

// Starting Y coordinates (display right to left)
#define DISP_STARTY_S01		16
#define DISP_STARTY_S10		56
#define DISP_STARTY_CMS		97
#define DISP_STARTY_M01		112
#define DISP_STARTY_M10		168
#define DISP_STARTY_CHM		225
#define DISP_STARTY_H01		240

void init_gfx_data(void);
void ep_init_hw(void);
void ep_init_part(void);
void ep_set_all_white(void);
void ep_set_num(digit_t *digit, uint8_t val);
void ep_update_display(void);
void ep_update_display_partial(void);
void ep_deepsleep(void);
void ep_full_clear(void);

#endif
