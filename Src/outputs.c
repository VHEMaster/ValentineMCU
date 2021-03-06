#include "outputs.h"
#include "effects.h"

static volatile float fChargingLevel = 1.f;
static LED_SavedStateType state_charging;

void out_updatecharginglevel(float level)
{
  if(level < 0) fChargingLevel = 0;
  else if (level > 1.f) fChargingLevel = 1.f;
  else fChargingLevel = level;
}

void out_charging(void)
{
  uint8_t charging_tick = 1;
  int8_t menu = -1;
  effect_fill(0);
  for(int y = 0; y < COUNT_CATHODES; y++)
    for(int x = 0; x < COUNT_ANODES; x++)
      BRIGHT[y][x] = BRIGHT_MAX / 6;

  for(int x = 7; x <= 17; x++)
  {
    LED[7] |= 1 << (x);
    LED[11] |= 1 << (x);
  }
  for(int y = 8; y <= 10; y++)
  {
    LED[y] |= (1 << 7) | (1 << 17) | (1 << 18);
  }

  for(int y = 8; y <= 10; y++)
    for(int x = 8; x <= 16; x++)
      BRIGHT[y][x] = BRIGHT_MAX / 12;

  effect_savestate(&state_charging);

  while(1)
  {
    if(fChargingLevel >= 1.f)
    {
      if(menu != 1)
        effect_restorestate(&state_charging);
      menu = 1;

      for(int y = 8; y <= 10; y++)
        for(int x = 8; x <= 16; x++)
          LED[y] |= (1 << x);

      effect_circle(800, 0.f, 360.f, 60.f);
      effect_fill_special(1);
      effect_decreasebright(1000, BRIGHT_MAX / 6);
      while(fChargingLevel >= 1.f)
        osDelay(100);
    }
    else
    {
      if(menu != 0)
      {
        charging_tick = 0;
        effect_restorestate(&state_charging);
      }
      if(++charging_tick >= 10)
        charging_tick = 0;
      int level = fChargingLevel * 9.f; //0-8
      if(level >= 8) level = 8;
      else if(level < 0) level = 0;
      if(charging_tick >= 5) level++;
      for(int y = 8; y <= 10; y++)
      {
        for(int x = 8; x < level + 8; x++)
          LED[y] |= (1 << x);
        for(int x = level + 8; x <= 16; x++)
          LED[y] &= ~(1 << x);
      }
      osDelay(100);
      if(menu != 0)
      {
        effect_circle(1000, 0.f, 360.f * 3, 60.f);
      }
      menu = 0;
    }
  }

}


void out_batterylow(void)
{
  effect_fill(0);
  for(int y = 0; y < COUNT_CATHODES; y++)
    for(int x = 0; x < COUNT_ANODES; x++)
      BRIGHT[y][x] = BRIGHT_MAX / 6;

  for(int x = 7; x <= 17; x++)
  {
    LED[7] |= 1 << x;
    LED[11] |= 1 << x;
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
    BRIGHT[y][x] = BRIGHT_MAX / 4;
    LED[y] |= 1 << x;
    osDelay(800 / 9);
  }

  osDelay(2000);
  effect_fill(0);
}

static LED_SavedStateType state;
static LED_SavedStateType state_nolet;
void out_main(void)
{
  effect_boot();
  effect_backlightcircle(1200, 0, 720, 50);
  effect_circle(1200, 0, 720, 70);
  effect_fill_special(3);
  osDelay(1000);
  effect_decreasebright(800, BRIGHT_MAX / 4);
  osDelay(1500);
  effect_fill(0);
  osDelay(1000);
  effect_print("????????, ?????? :?",90);
  effect_fill_special(3);
  effect_smile1();
  osDelay(700);
  effect_decreasebright(1000, BRIGHT_MAX / 4);
  osDelay(700);
  effect_fill_special(3);
  osDelay(50);
  effect_decreasebright(1000, BRIGHT_MAX / 4);
  effect_fill_special(3);
  osDelay(50);
  effect_decreasebright(1200, BRIGHT_MAX / 4);
  effect_fill_special(3);
  osDelay(1000);
  effect_decreasebright(800, BRIGHT_MAX / 4);
  osDelay(1000);

  effect_fill(0);
  osDelay(1000);
  effect_print("?????? ?????? ? ?? ???? ? ???? ????? ? ????? ??????",90);
  effect_fill_special(3);
  osDelay(1000);

  effect_print("???? ?????????? ?????????, ?? ?? ?????? ?? ????",90);
  osDelay(1000);
  effect_fill_special(4);
  osDelay(500);
  effect_fill_special(2);
  effect_decreasebright(500, BRIGHT_MAX / 4);
  osDelay(1500);
  effect_fill(0);

  while(1)
  {

    osDelay(1000);
    effect_print("?????? ????? ??????? ?????, ????????? ????????",90);
    effect_fill_special(1);
    osDelay(500);
    effect_print("????????? ????? ? ??? ? ???????, ?????? ??????",90);
    effect_fill_special(2);
    effect_print("??????? ???? ????????, ????? ???? ???, ??????",90);
    effect_fill_special(2);
    effect_print("? ??????????, ???????, ?? ? ????? ????? - ??? ???",90);
    effect_smile();
    effect_decreasebright(1500, BRIGHT_MAX / 4);
    osDelay(1000);
    effect_fill_special(3);
    osDelay(1000);
    effect_fill_special(4);
    osDelay(2000);
    effect_fill_special(2);
    osDelay(1000);
    effect_savestate(&state_nolet);
    effect_savestate(&state);

    effect_putch('?',9,5);
    osDelay(600);
    effect_restorestate(&state);
    effect_putch('?',8,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('?',7,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('?',6,5);
    osDelay(300);
    effect_savestate(&state);

    effect_putch('?',10,5);
    osDelay(600);
    effect_restorestate(&state);
    effect_putch('?',11,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('?',12,5);
    osDelay(100);
    effect_restorestate(&state);
    effect_putch('?',13,5);
    osDelay(500);

    for(int i=0;i<2;i++)
    {
      effect_restorestate(&state_nolet);
      effect_putch('?',13,5);
      effect_putch('?',6,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',12,5);
      effect_putch('?',7,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',11,5);
      effect_putch('?',8,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',10,5);
      effect_putch('?',9,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',9,5);
      effect_putch('?',10,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',8,5);
      effect_putch('?',11,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',7,5);
      effect_putch('?',12,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',6,5);
      effect_putch('?',13,5);
      osDelay(1000);

      effect_restorestate(&state_nolet);
      effect_putch('?',7,5);
      effect_putch('?',12,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',8,5);
      effect_putch('?',11,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',9,5);
      effect_putch('?',10,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',10,5);
      effect_putch('?',9,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',11,5);
      effect_putch('?',8,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',12,5);
      effect_putch('?',7,5);
      osDelay(70);
      effect_restorestate(&state_nolet);
      effect_putch('?',13,5);
      effect_putch('?',6,5);
      osDelay(1000);
    }

    effect_savestate(&state);
    effect_fill_special(5);
    osDelay(1000);
    effect_decreasebright(1500, BRIGHT_MAX / 4);

    osDelay(500);
    effect_fill_special(3);
    osDelay(500);
    effect_fill_special(5);
    osDelay(1000);
    effect_decreasebright(1500, BRIGHT_MAX / 4);
    osDelay(500);
    effect_fill_special(3);
    osDelay(500);
    effect_fill_special(5);
    osDelay(1000);
    effect_decreasebright(1500, BRIGHT_MAX / 4);
    osDelay(1000);
    effect_fill_special(2);

    osDelay(500);
    effect_print("??????, ????????, ???,",90);
    effect_fill_special(5);
    effect_print("????? ?, ?????, ????!",90);
    osDelay(500);
    effect_print("???? ??????",90);
    effect_decreasebright(1500, BRIGHT_MAX / 4);
    osDelay(500);

    effect_fill_special(1);
    effect_fill_special(2);
    effect_fill_special(2);
    effect_fill_special(2);
    effect_fill_special(2);
    effect_fill_special(4);
    effect_decreasebright(1500, BRIGHT_MAX / 4);

    osDelay(2000);
    effect_fill(0);


  }
}
