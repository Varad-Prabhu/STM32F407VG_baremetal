#ifndef WWDG_H
#define WWDG_H

#include "stm32f407.h"

/* Counter limits */
/* Counter value must be between 0x40 and 0x7F */
#define WWDG_COUNTER_MAX      0x7FU
#define WWDG_COUNTER_MIN      0x40U

/* Window limits */
#define WWDG_WINDOW_MAX       0x7FU
#define WWDG_WINDOW_MIN       0x40U

void WWDG_Init(uint8_t prescaler, uint8_t window, uint8_t counter);

void WWDG_Refresh(uint8_t counter);

#endif
