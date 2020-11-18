#include "main.h"
#include "font.h"


extern uint32_t LED[23];
extern uint8_t BRIGHT[23][26] __attribute__ ((aligned (4)));

void effect_fill_special(uint8_t dir);
void effect_boot(void);
void effect_fill(uint8_t k);
void effect_print(char * text);
void effect_decreasebright(uint32_t time);
void effect_smile(void);
void effect_savestate(uint32_t * data);
void effect_restorestate(uint32_t * data);
void effect_putch(char ch, uint8_t x, uint8_t y);

