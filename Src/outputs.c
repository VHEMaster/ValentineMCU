#include "outputs.h"
#include "effects.h"

void out_batterylow(void)
{
  effect_fill(0);
  for(int y = 0; y < COUNT_CATHODES; y++)
    for(int x = 0; x < COUNT_ANODES; x++)
      BRIGHT[y][x] = BRIGHT_MAX / 3;

  for(int x = 7; x <= 17; x++)
  {
    LED[7] |= 1 << (x + 7);
    LED[11] |= 1 << (x + 7);
  }
  for(int y = 8; y <= 10; y++)
  {
    LED[y] |= (1 << 7) | (1 << 17) | (1 << 18);
  }

  for(int y = 8; y <= 10; y++)
    LED[y] |= (1 << 8);
  osDelay(300);
  for(int y = 8; y <= 10; y++)
    LED[y] &= ~(1 << 8);
  osDelay(300);
  for(int y = 8; y <= 10; y++)
    LED[y] |= (1 << 8);
  osDelay(300);
  for(int y = 8; y <= 10; y++)
    LED[y] &= ~(1 << 8);
  osDelay(300);
  for(int y = 8; y <= 10; y++)
    LED[y] |= (1 << 8);
  osDelay(300);
  for(int y = 8; y <= 10; y++)
    LED[y] &= ~(1 << 8);
  osDelay(300);

  for(int i = 0; i < 9; i++)
  {
    uint8_t x = 16 - i;
    uint8_t y = 5 + i;
    BRIGHT[y][x] = BRIGHT_MAX / 1.5f;
    LED[y] |= 1 << x;
    osDelay(600 / 9);
  }

  osDelay(1000);
  effect_stop();
}

static LED_SavedStateType state;
static LED_SavedStateType state_nolet;
void out_main(void)
{
  effect_boot();
  effect_fill_special(3);
  osDelay(1000);
  effect_decreasebright(800, BRIGHT_MAX / 3);
  osDelay(1500);
  effect_fill(0);
  osDelay(1000);
  effect_print("Приветик, Натуся :з");
  effect_fill_special(3);
  effect_smile();
  effect_decreasebright(1000, BRIGHT_MAX / 3);
  osDelay(700);
  effect_fill_special(3);
  osDelay(50);
  effect_decreasebright(1000, BRIGHT_MAX / 3);
  effect_fill_special(3);
  osDelay(50);
  effect_decreasebright(1200, BRIGHT_MAX / 3);
  effect_fill_special(3);
  osDelay(50);
  effect_decreasebright(1400, BRIGHT_MAX / 3);
  effect_fill_special(3);
  osDelay(50);
  effect_decreasebright(1600, BRIGHT_MAX / 3);
  effect_fill_special(3);
  osDelay(1000);
  effect_decreasebright(800, BRIGHT_MAX / 3);
  osDelay(1500);

  effect_fill(0);
  osDelay(1000);
  effect_print("");
  effect_fill_special(3);
  osDelay(1000);

  effect_print("");
  osDelay(1000);
  effect_fill_special(4);
  osDelay(500);
  effect_decreasebright(1500, BRIGHT_MAX / 3);
  osDelay(1500);
  effect_fill(0);

  while(1)
  {

    osDelay(1000);
    effect_print("");
    effect_fill_special(3);
    osDelay(500);
    effect_print("");
    effect_print("");
    effect_print("");
    effect_smile();
    effect_decreasebright(1500, BRIGHT_MAX / 3);
    osDelay(1000);
    effect_fill_special(3);
    osDelay(1000);
    effect_fill_special(4);
    osDelay(2000);
    effect_fill(0);

    osDelay(1000);
    effect_fill_special(3);
    osDelay(1000);
    effect_savestate(&state_nolet);
    effect_savestate(&state);

    effect_putch('М',9,5);
    osDelay(600);
    effect_restorestate(&state);
    effect_putch('М',8,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('М',7,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('М',6,5);
    osDelay(300);
    effect_savestate(&state);

    effect_putch('Н',10,5);
    osDelay(600);
    effect_restorestate(&state);
    effect_putch('Н',11,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('Н',12,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('Н',13,5);
    osDelay(500);

    for(int i=0;i<2;i++)
    {
      effect_restorestate(&state_nolet);
      effect_putch('Н',13,5);
      effect_putch('М',6,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',12,5);
      effect_putch('М',7,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',11,5);
      effect_putch('М',8,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',10,5);
      effect_putch('М',9,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',9,5);
      effect_putch('М',10,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',8,5);
      effect_putch('М',11,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',7,5);
      effect_putch('М',12,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',6,5);
      effect_putch('М',13,5);
      osDelay(1000);

      effect_restorestate(&state_nolet);
      effect_putch('Н',7,5);
      effect_putch('М',12,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',8,5);
      effect_putch('М',11,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',9,5);
      effect_putch('М',10,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',10,5);
      effect_putch('М',9,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',11,5);
      effect_putch('М',8,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',12,5);
      effect_putch('М',7,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('Н',13,5);
      effect_putch('М',6,5);
      osDelay(1000);
    }

    effect_savestate(&state);
    effect_fill_special(5);
    osDelay(1000);
    effect_decreasebright(1500, BRIGHT_MAX / 3);

    osDelay(500);
    effect_fill_special(3);
    osDelay(500);
    effect_fill_special(5);
    osDelay(1000);
    effect_decreasebright(1500, BRIGHT_MAX / 3);
    osDelay(500);
    effect_fill_special(3);
    osDelay(500);
    effect_fill_special(5);
    osDelay(1000);
    effect_decreasebright(1500, BRIGHT_MAX / 3);

    osDelay(2000);
    effect_fill(0);


  }
}
