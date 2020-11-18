#include "main.h"
#include "font.h"

#define COUNT_ANODES 26
#define COUNT_CATHODES 24

#define BRIGHT_MAX 32
#define REFRESH_RATE 100

extern uint32_t LED[COUNT_CATHODES];
extern uint8_t BRIGHT[COUNT_CATHODES][COUNT_ANODES];

typedef struct
{
    uint32_t data[COUNT_ANODES * COUNT_CATHODES / 4 + COUNT_CATHODES];
} LED_SavedStateType;

extern void effect_start(TIM_HandleTypeDef * _htim);
extern void effect_stop(void);
extern void effect_fill_special(uint8_t dir);
extern void effect_boot(void);
extern void effect_fill(uint8_t k);
extern void effect_print(char * text);
extern void effect_decreasebright(uint32_t time, uint8_t value);
extern void effect_removebright(uint32_t time);
extern void effect_smile(void);
extern void effect_savestate(LED_SavedStateType * data);
extern void effect_restorestate(LED_SavedStateType * data);
extern void effect_putch(char ch, uint8_t x, uint8_t y);

