#include "main.h"
#include "font.h"
#include "effects.h"
#include <string.h>
#include <math.h>

extern RNG_HandleTypeDef hrng;
static TIM_HandleTypeDef * htim;

uint32_t LED[COUNT_CATHODES];
uint8_t BRIGHT[COUNT_CATHODES][COUNT_ANODES] __attribute__ ((aligned (4)));

#define EDGE_HEARTH_SIZE 60
const uint8_t EDGE_HEARTH_ANODES[EDGE_HEARTH_SIZE] = { 12,11,10,9,8,7,6,5,4,3,2,1,1,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,24,25,25,25,25,24,24,23,22,21,20,19,18,17,16,15,14,13 };
const uint8_t EDGE_HEARTH_CATHODES[EDGE_HEARTH_SIZE] = { 2,1,1,0,0,0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,0,0,0,0,1,1,2 };

static uint32_t TABLE_AND[COUNT_CATHODES] =
{
    0xF03C0,
    0x3FCFF0,
    0x7FFFF8,
    0xFFFFFC,
    0x1FFFFFE,
    0x1FFFFFE,
    0x3FFFFFF,
    0x3FFFFFF,
    0x3FFFFFF,
    0x3FFFFFF,
    0x1FFFFFE,
    0x1FFFFFE,
    0xFFFFFC,
    0x7FFFF8,
    0x3FFFF0,
    0x1FFFE0,
    0xFFFC0,
    0x7FF80,
    0x3FF00,
    0x1FE00,
    0xFC00,
    0x7800,
    0x3000,
    0x3FFFFFF
};

static int8_t irq_counter = 0;
static int8_t irq_brightcounter = -1;

static inline void AnodesEnable(uint32_t data)
{
  uint16_t data_l = data & 0xFFFF;
  uint16_t data_l_inv = data_l ^ 0xFFFF;
  uint16_t data_m = (data >> 16) & 0x3FF;
  uint16_t data_m_inv = data_m ^ 0x3FF;

  GPIOB->BSRR = (data_l << 16) | data_l_inv;
  GPIOD->BSRR = (data_m << 16) | data_m_inv;
}

static inline void AnodesDisable(void)
{
  GPIOB->BSRR = 0xFFFF;
  GPIOD->BSRR = 0x3FF;
}

static inline void CathodesDisable(void)
{
  GPIOC->BSRR = 0xFFFF0000;
  GPIOE->BSRR = 0x00FF0000;
}

static inline void CathodeEnable(uint8_t index)
{
  uint16_t data_l = (1 << index) & 0xFFFF;
  uint16_t data_l_inv = data_l ^ 0xFFFF;
  uint16_t data_m = ((index >= 16) ? (1 << (index - 16)) : 0) & 0xFF;
  uint16_t data_m_inv = data_m ^ 0xFF;

  GPIOC->BSRR = (data_l_inv << 16) | data_l;
  GPIOE->BSRR = (data_m_inv << 16) | data_m;
}

static volatile uint8_t started = 0;

void effect_start(TIM_HandleTypeDef * _htim)
{
  if(!started)
  {
    htim = _htim;
    htim->Instance->ARR = HAL_RCC_GetPCLK1Freq() * 2.f / REFRESH_RATE / (BRIGHT_MAX + 1) / COUNT_CATHODES;
    irq_counter = 0;
    irq_brightcounter = -1;
    AnodesDisable();
    CathodesDisable();
    effect_fill(0);
    HAL_GPIO_WritePin(MCU_MATRIX_EN_GPIO_Port, MCU_MATRIX_EN_Pin, GPIO_PIN_SET);
    HAL_TIM_Base_Start_IT(htim);
    started = 1;
  }
}
void effect_stop(void)
{
  if(started)
  {
    HAL_GPIO_WritePin(MCU_MATRIX_EN_GPIO_Port, MCU_MATRIX_EN_Pin, GPIO_PIN_RESET);
    HAL_TIM_Base_Stop_IT(htim);
    effect_fill(0);
    AnodesDisable();
    CathodesDisable();
    irq_counter = 0;
    irq_brightcounter = -1;
    started = 0;
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM7)
  {
    if(++irq_brightcounter >= BRIGHT_MAX)
    {
      AnodesDisable();
      CathodesDisable();
      irq_brightcounter = -1;
      if(++irq_counter >= COUNT_CATHODES)
      {
        irq_counter = 0;
        //VSync event? :)
      }

    }
    else
    {
      CathodeEnable(irq_counter);

      uint32_t data = LED[irq_counter] & TABLE_AND[irq_counter];
      for(int a = 0; a < COUNT_ANODES; a++)
          data &= ~((BRIGHT[irq_counter][a] > irq_brightcounter) << a);

      AnodesEnable(data);
    }
  }
}

static void SetRowSpecBrightness(uint8_t row, uint32_t data, uint8_t br)
{
	uint8_t i = 0;
	for(i=0;i<COUNT_ANODES;i++)
	{
		if(((data>>i)&1) == 1) BRIGHT[row][i] = br;
	}
}


void effect_fill(uint8_t k)
{
	uint8_t i,j;
	for(i=0;i<COUNT_CATHODES;i++)
		LED[i] = k*0x3FFFFFF;
	for(i=0;i<COUNT_CATHODES;i++)
		for(j=0;j<COUNT_ANODES;j++)
			BRIGHT[i][j] = BRIGHT_MAX;
}

void effect_boot(void)
{
	effect_fill(0);
	osDelay(1000);
	
	LED[22] = 0x1000;
	osDelay(1000);
	LED[22] = 0x2000;
	osDelay(1000);
	LED[22] = 0x1000;
	osDelay(1000);
	LED[22] = 0x2000;
	osDelay(1000);
	LED[22] = 0x1000;
	osDelay(1000);
	LED[22] = 0x2000;
	osDelay(1000);
	effect_fill(0);
	osDelay(2000);
}

char tempstr[255] = {' ',' ',' ',' ',' ',0};
uint32_t original[7];
uint32_t original_br[7][COUNT_ANODES];


void effect_putch(char ch, uint8_t x, uint8_t y)
{
	uint8_t k,l;
	
	for(k=0;k<7;k++)
	{
		original[k] = LED[k+y];
		for(l=0;l<COUNT_ANODES;l++)
			original_br[k][l] = BRIGHT[k+y][l];
	}
	
	for(k=0;k<7;k++)
	{
		LED[k+y] = original[k];
		LED[k+y] |= font_get(ch,k)<<x;
	}
}


void effect_print(char * text)
{
	uint8_t i,j,k,l;
	uint8_t length = strlen(text);
	for(i=0;i<length;i++)
		tempstr[i+4] = text[i];
	for(i+=4;i<sizeof(tempstr);i++)
		tempstr[i] = 0;
	tempstr[length+4] = 0;
	length = strlen(tempstr);
	
	for(k=0;k<7;k++)
	{
		original[k] = LED[k+5];
		for(i=0;i<COUNT_ANODES;i++)
			original_br[k][i] = BRIGHT[k+5][i];
	}
	
	
	for(i=0;i<length+1;i++)
	{
		for(j=0;j<6;j++)
		{
			for(k=0;k<7;k++)
			{
				LED[k+5] = original[k];
				LED[k+5] |= font_get(tempstr[i],k)>>j;
				for(l=0;l<4;l++)
					LED[k+5] |= ((font_get(tempstr[i+l+1],k))<<((6-j)+(l*6)));
			}
			osDelay(90);
		}
	}
}


LED_SavedStateType circlesaving;

void effect_circle(int time, float anglestart, float angles, float length)
{
  //To radians

  anglestart /= 360.0f;
  angles /= 360.0f;
  length /= 360.0f;

  float delay = 1000 / REFRESH_RATE * 1.5f;
  float divider = time / delay;
  float anglestep = angles / divider + length / divider;
  float curfrom = 0;
  float curto = 0;
  for(float i = 0; i <= divider; i += 1.f)
  {
    effect_savestate(&circlesaving);
    curto += anglestep;
    curfrom = curto - length;
    if(curfrom < 0.f) curfrom = 0;
    if(curto > angles) curfrom = angles;
    effect_setpercentfromto(curfrom + anglestart, curto + anglestart);
    osDelay(delay);
    effect_restorestate(&circlesaving);
  }
}

/*
dir = 0 - just fill;
dir = 1 - cockwhise
2 - reverse
3 - from top to bottom
4 - Random fill
*/
uint16_t notfilled[COUNT_ANODES*COUNT_CATHODES];
void effect_fill_special(uint8_t dir)
{
  float percent;
	if(dir == 0)
	{
		LED[0]  |= 0xF03C0;
		LED[1]  |= 0x30CC30;
		LED[2]  |= 0x403008;
		LED[3]  |= 0x800004;
		LED[4]  |= 0x1000002;
		LED[5]  |= 0x1000002;
		LED[6]  |= 0x2000001;
		LED[7]  |= 0x2000001;
		LED[8]  |= 0x2000001;
		LED[9]  |= 0x2000001;
		LED[10] |= 0x1000002;
		LED[11] |= 0x1000002;
		LED[12] |= 0x800004;
		LED[13] |= 0x400008;
		LED[14] |= 0x200010;
		LED[15] |= 0x100020;
		LED[16] |= 0x80040;
		LED[17] |= 0x40080;
		LED[18] |= 0x20100;
		LED[19] |= 0x10200;
		LED[20] |= 0x8400;
		LED[21] |= 0x4800;
		LED[22] |= 0x3000;
	}
	else if(dir == 1)
	{
    int time = 500;
    float delay = 1000 / REFRESH_RATE * 1.5f;
    float divider = time / delay;
    for(float i = 0; i <= divider; i += 1.f)
    {
      percent = i / divider;
      effect_setbacklightpercent(percent);
      effect_setpercent(percent);
      osDelay(delay);
    }
	}
	else if(dir == 4)
	{
		uint16_t notfilledcnt;
		uint32_t random = 0;
		uint8_t i,j;
		while(1)
		{
			notfilledcnt = 0;
			for(i=0;i<COUNT_CATHODES;i++)
				for(j=0;j<COUNT_ANODES;j++)
					if(((TABLE_AND[i] & 1) << j) == 1 && ((LED[i]>>j)&1) == 0)
						notfilled[notfilledcnt++] = (i*COUNT_ANODES)+j;
			if(notfilledcnt == 0) break;
			HAL_RNG_GenerateRandomNumber(&hrng, &random);
			random %= notfilledcnt;

      LED[notfilled[random]/COUNT_ANODES] |= 1<<(notfilled[random]%COUNT_ANODES);
      BRIGHT[notfilled[random]/COUNT_ANODES][notfilled[random]%COUNT_ANODES] = BRIGHT_MAX;
			//DelayUs((notfilledcnt*notfilledcnt)/50.0f);
			osDelay(4);
		}
	}
	else if(dir == 3)
	{
	  int time = 500;
	  float delay = 1000 / REFRESH_RATE * 1.5f;
	  float divider = time / delay;
	  for(float i = 0; i <= divider; i += 1.f)
	  {
	    percent = i / divider;
      effect_setbacklighttwohalfspercent(percent);
      effect_settwohalfspercent(percent);
      osDelay(delay);
	  }
	}
	else if(dir == 5)
	{
		LED[15]  |= 0x3000;
		SetRowSpecBrightness(15,0x3000,BRIGHT_MAX);
		osDelay(100);
		LED[14]  |= 0x4800;
		SetRowSpecBrightness(14,0x4800,BRIGHT_MAX);
		osDelay(100);
		LED[15]  |= 0xB400;
		SetRowSpecBrightness(15,0xB400,BRIGHT_MAX);
		osDelay(100);
		LED[16]  |= 0x8400;
		SetRowSpecBrightness(16,0x8400,BRIGHT_MAX);
		osDelay(100);
		LED[17]  |= 0x4800;
		SetRowSpecBrightness(17,0x4800,BRIGHT_MAX);
		osDelay(100);
		LED[18]  |= 0x3000;
		SetRowSpecBrightness(18,0x3000,BRIGHT_MAX);
		osDelay(100);
	}
}

void effect_smile(void)
{
		LED[5]  |= 0x10000;
		osDelay(50);
		LED[5]  |= 0x18000;
		osDelay(50);
		LED[5]  |= 0x1C000;
		osDelay(50);
		LED[6]  |= 0x04000;
		osDelay(50);
		LED[7]  |= 0x04000;
		osDelay(50);
		LED[7]  |= 0x0C000;
		osDelay(50);
		LED[7]  |= 0x1C000;
		osDelay(50);
		LED[6]  |= 0x14000;
	
		osDelay(100);
	
		LED[5]  |= 0x1C800;
		osDelay(50);
		LED[5]  |= 0x1CC00;
		osDelay(50);
		LED[5]  |= 0x1CE00;
		osDelay(50);
		LED[6]  |= 0x14200;
		osDelay(50);
		LED[7]  |= 0x1C200;
		osDelay(50);
		LED[7]  |= 0x1C600;
		osDelay(50);
		LED[7]  |= 0x1CE00;
		osDelay(50);
		LED[6]  |= 0x14A00;
		
		osDelay(100);
		
		LED[12]  |= 0x3000;
		osDelay(150);
		LED[11]  |= 0x4800;
		osDelay(150);
		LED[10]  |= 0x8400;
		osDelay(150);
		LED[9]  |= 0x10200;
		osDelay(1000);
		
		
		LED[9]  &= ~0x10200;
		osDelay(150);
		LED[10]  &= ~0x8400;
		osDelay(1000);
		
		LED[10]  |= 0x8400;
		osDelay(150);
		LED[9]  |= 0x10200;
		osDelay(1000);
		
		LED[9]  &= ~0x10200;
		osDelay(150);
		LED[10]  &= ~0x8400;
		osDelay(1000);
		
		LED[10]  |= 0x8400;
		osDelay(150);
		LED[9]  |= 0x10200;
		osDelay(1000);
}
void effect_savestate(LED_SavedStateType * state)
{
	uint16_t i = 0;
	uint32_t * data = state->data;
	for(i=0;i<COUNT_CATHODES;i++)
		*data++ = LED[i];
  for(i=0;i<(COUNT_CATHODES*COUNT_ANODES/4);i++)
		*data++ = ((uint32_t*)BRIGHT)[i];
}

void effect_restorestate(LED_SavedStateType * state)
{
	uint16_t i = 0;
  uint32_t * data = state->data;
	for(i=0;i<COUNT_CATHODES;i++,data++)
		LED[i] = *data;
	for(i=0;i<(COUNT_CATHODES*COUNT_ANODES/4);i++,data++)
		((uint32_t*)BRIGHT)[i] = *data;
}

void effect_decreasebright(uint32_t time, uint8_t value)
{
  uint8_t t,i,j;
  t = BRIGHT_MAX;
  while(t > value)
  {
    osDelay(time/(BRIGHT_MAX - value));
    t--;
    for(i=0;i<COUNT_CATHODES;i++)
      for(j=0;j<COUNT_ANODES;j++)
        if(BRIGHT[i][j] > t)
          BRIGHT[i][j] = t;

  }
}

void effect_removebright(uint32_t time)
{
  uint8_t t,i,j;
  t = BRIGHT_MAX;
  while(t > 1)
  {
    osDelay(time/BRIGHT_MAX);
    t--;
    for(i=0;i<COUNT_CATHODES;i++)
      for(j=0;j<COUNT_ANODES;j++)
        if(BRIGHT[i][j] > 1)
          BRIGHT[i][j] = t;

  }
}

void effect_setbacklightpercent(float percent)
{
  uint8_t index = 0;
  uint8_t brightness = 0;
  if(percent < 0) index = brightness = 0;
  else if(percent >= 1.f) index = COUNT_ANODES - 1, brightness = BRIGHT_MAX;
  else
  {
    index = percent * COUNT_ANODES;
    brightness = fmodf(percent, 1.f / COUNT_ANODES) * COUNT_ANODES * BRIGHT_MAX;
  }

  if(index >= COUNT_ANODES) index = COUNT_ANODES - 1;

  for(int i = 0; i < index; i++)
  {
    LED[COUNT_CATHODES - 1] |= (1 << i);
    BRIGHT[COUNT_CATHODES - 1][i] = BRIGHT_MAX;
  }
  LED[COUNT_CATHODES - 1] |= (1 << index);
  BRIGHT[COUNT_CATHODES - 1][index] = brightness;
  //for(int i = index + 1; i < COUNT_ANODES; i++)
  //  BRIGHT[COUNT_CATHODES - 1][i] = 0;

  LED[COUNT_CATHODES - 1] = 0x3FFFFFF;
}

void effect_setbacklighttwohalfspercent(float percent)
{
  uint8_t index = 0;
  uint8_t brightness = 0;
  if(percent < 0) index = brightness = 0;
  else if(percent >= 1.f) index = (COUNT_ANODES / 2) - 1, brightness = BRIGHT_MAX;
  else
  {
    index = percent * COUNT_ANODES;
    brightness = fmodf(percent, 1.f / (COUNT_ANODES / 2)) * (COUNT_ANODES / 2) * BRIGHT_MAX;
  }

  if(index >= (COUNT_ANODES / 2)) index = (COUNT_ANODES / 2) - 1;

  for(int i = 0; i < index; i++)
  {
    LED[COUNT_CATHODES - 1] |= (1 << i) | (1 << (COUNT_ANODES - 1 - i));
    BRIGHT[COUNT_CATHODES - 1][i] = BRIGHT[COUNT_CATHODES - 1][COUNT_ANODES - 1 - i] = BRIGHT_MAX;
  }
  LED[COUNT_CATHODES - 1] |= (1 << index) | (1 << (COUNT_ANODES - 1 - index));
  BRIGHT[COUNT_CATHODES - 1][index] = BRIGHT[COUNT_CATHODES - 1][COUNT_ANODES - 1 - index] = brightness;
  //for(int i = index + 1; i < COUNT_ANODES / 2; i++)
  //  BRIGHT[COUNT_CATHODES - 1][i] = BRIGHT[COUNT_CATHODES - 1][COUNT_ANODES - 1 - i] = 0;

}

void effect_settwohalfspercent(float percent)
{
  const int count = EDGE_HEARTH_SIZE / 2;
  uint8_t index = 0;
  uint8_t brightness = 0;

  if(percent < 0) index = brightness = 0;
  else if(percent >= 1.f) index = count - 1, brightness = BRIGHT_MAX;
  else
  {
    index = percent * count;
    brightness = fmodf(percent, 1.f / count) * count * BRIGHT_MAX;
  }

  for(int i = 0; i < index; i++)
  {
    LED[EDGE_HEARTH_CATHODES[i]] |= 1 << EDGE_HEARTH_ANODES[i];
    LED[EDGE_HEARTH_CATHODES[EDGE_HEARTH_SIZE - 1 - i]] |= 1 << EDGE_HEARTH_ANODES[EDGE_HEARTH_SIZE - 1 - i];
    BRIGHT[EDGE_HEARTH_CATHODES[i]][EDGE_HEARTH_ANODES[i]] =
        BRIGHT[EDGE_HEARTH_CATHODES[EDGE_HEARTH_SIZE - 1 - i]][EDGE_HEARTH_ANODES[EDGE_HEARTH_SIZE - 1 - i]] = BRIGHT_MAX;
  }
  LED[EDGE_HEARTH_CATHODES[index]] |= 1 << EDGE_HEARTH_ANODES[index];
  LED[EDGE_HEARTH_CATHODES[EDGE_HEARTH_SIZE - 1 - index]] |= 1 << EDGE_HEARTH_ANODES[EDGE_HEARTH_SIZE - 1 - index];
  BRIGHT[EDGE_HEARTH_CATHODES[index]][EDGE_HEARTH_ANODES[index]] =
      BRIGHT[EDGE_HEARTH_CATHODES[EDGE_HEARTH_SIZE - 1 - index]][EDGE_HEARTH_ANODES[EDGE_HEARTH_SIZE - 1 - index]] = brightness;
  //for(int i = index + 1; i < count; i++)
  //  BRIGHT[EDGE_HEARTH_CATHODES[i]][EDGE_HEARTH_ANODES[i]] =
  //    BRIGHT[EDGE_HEARTH_CATHODES[EDGE_HEARTH_SIZE - 1 - index]][EDGE_HEARTH_ANODES[EDGE_HEARTH_SIZE - 1 - index]] = 0;

}

void effect_setpercent(float percent)
{
  const int count = EDGE_HEARTH_SIZE;
  uint8_t index = 0;
  uint8_t brightness = 0;

  if(percent < 0) index = brightness = 0;
  else if(percent >= 1.f) index = count - 1, brightness = BRIGHT_MAX;
  else
  {
    index = percent * count;
    brightness = fmodf(percent, 1.f / count) * count * BRIGHT_MAX;
  }

  for(int i = 0; i < index; i++)
  {
    LED[EDGE_HEARTH_CATHODES[i]] |= 1 << EDGE_HEARTH_ANODES[i];
    BRIGHT[EDGE_HEARTH_CATHODES[i]][EDGE_HEARTH_ANODES[i]] = BRIGHT_MAX;
  }
  LED[EDGE_HEARTH_CATHODES[index]] |= 1 << EDGE_HEARTH_ANODES[index];
  BRIGHT[EDGE_HEARTH_CATHODES[index]][EDGE_HEARTH_ANODES[index]] = brightness;
  //for(int i = index + 1; i < count; i++)
  //  BRIGHT[EDGE_HEARTH_CATHODES[i]][EDGE_HEARTH_ANODES[i]]  = 0;

}

void effect_setbacklightpercentfromto(float percentfrom, float percentto)
{
  const int count = COUNT_ANODES;
  uint8_t indexfrom = 0;
  uint8_t brightnessfrom = 0;
  uint8_t indexto = 0;
  uint8_t brightnessto = 0;

  indexfrom = percentfrom * count;
  brightnessfrom = fmodf(percentfrom, 1.f / count) * count * BRIGHT_MAX;
  indexto = percentto * count;
  brightnessto = fmodf(percentto, 1.f / count) * count * BRIGHT_MAX;

  if(indexfrom == indexto)
  {
    LED[COUNT_CATHODES - 1] |= (1 << (indexfrom % count));
    BRIGHT[COUNT_CATHODES - 1][(indexfrom % count)] = (brightnessfrom + brightnessto) / 2;
  }
  else
  {
    LED[COUNT_CATHODES - 1] |= (1 << (indexfrom % count));
    BRIGHT[COUNT_CATHODES - 1][(indexfrom % count)] = brightnessfrom;

    for(int i = indexfrom + 1; i < indexto; i++)
    {
      LED[COUNT_CATHODES - 1] |= (1 << (i % count));
      BRIGHT[COUNT_CATHODES - 1][(i % count)] = BRIGHT_MAX;
    }

    LED[COUNT_CATHODES - 1] |= (1 << (indexto % count));
    BRIGHT[COUNT_CATHODES - 1][(indexto % count)] = brightnessto;
  }
}

void effect_setpercentfromto(float percentfrom, float percentto)
{
  const int count = EDGE_HEARTH_SIZE;
  uint8_t indexfrom = 0;
  uint8_t brightnessfrom = 0;
  uint8_t indexto = 0;
  uint8_t brightnessto = 0;

  indexfrom = percentfrom * count;
  brightnessfrom = fmodf(percentfrom, 1.f / count) * count * BRIGHT_MAX;
  indexto = percentto * count;
  brightnessto = fmodf(percentto, 1.f / count) * count * BRIGHT_MAX;


  if(indexfrom == indexto)
  {
    LED[EDGE_HEARTH_CATHODES[indexfrom % count]] |= 1 << EDGE_HEARTH_ANODES[indexfrom % count];
    BRIGHT[EDGE_HEARTH_CATHODES[indexfrom % count]][EDGE_HEARTH_ANODES[indexfrom % count]] = (brightnessfrom + brightnessto) / 2;
  }
  else
  {

    LED[EDGE_HEARTH_CATHODES[indexfrom % count]] |= 1 << EDGE_HEARTH_ANODES[indexfrom % count];
    BRIGHT[EDGE_HEARTH_CATHODES[indexfrom % count]][EDGE_HEARTH_ANODES[indexfrom % count]] = brightnessfrom;

    for(int i = indexfrom + 1; i < indexto; i++)
    {
      LED[EDGE_HEARTH_CATHODES[i % count]] |= 1 << EDGE_HEARTH_ANODES[i % count];
      BRIGHT[EDGE_HEARTH_CATHODES[i % count]][EDGE_HEARTH_ANODES[i % count]] = BRIGHT_MAX;
    }

    LED[EDGE_HEARTH_CATHODES[indexto % count]] |= 1 << EDGE_HEARTH_ANODES[indexto % count];
    BRIGHT[EDGE_HEARTH_CATHODES[indexto % count]][EDGE_HEARTH_ANODES[indexto % count]] = brightnessto;
  }

}
