#include "main.h"
#include "font.h"
#include "effects.h"
#include <string.h>

extern RNG_HandleTypeDef hrng;
static TIM_HandleTypeDef * htim;

uint32_t LED[COUNT_CATHODES];
uint8_t BRIGHT[COUNT_CATHODES][COUNT_ANODES] __attribute__ ((aligned (4)));

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

static uint32_t LEDS_COMPLETE_DATA[COUNT_CATHODES][BRIGHT_MAX];

volatile static int8_t irq_counter = 0;
volatile static int8_t irq_brightcounter = -1;

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
    htim->Instance->ARR = 100000000 / REFRESH_RATE / (BRIGHT_MAX + 1) / COUNT_CATHODES;
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

static void update_led_data(void)
{
  //Probably we should do it outside of ISR? 19968 iterations seems too lot.
  uint32_t data = 0;
  for(int c = 0; c < COUNT_CATHODES; c++)
  {
    for(int b = 0; b < BRIGHT_MAX; b++)
    {
      data = LED[c] & TABLE_AND[c];
      for(int a = 0; a < COUNT_ANODES; a++)
          data &= ~((BRIGHT[c][a] > b) << a);
      LEDS_COMPLETE_DATA[c][b] = data;
    }
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
        update_led_data();
      }
    }
    else
    {
      CathodeEnable(irq_counter);
      AnodesEnable(LEDS_COMPLETE_DATA[irq_counter][irq_brightcounter]);
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
					if(((LED[i]>>j)&1) == 0)
						notfilled[notfilledcnt++] = (i*COUNT_ANODES)+j;
			if(notfilledcnt == 0) break;
			HAL_RNG_GenerateRandomNumber(&hrng, &random);
			random %= notfilledcnt;

			LED[notfilled[random]/COUNT_ANODES] |= 1<<(notfilled[random]%COUNT_ANODES);
			//DelayUs((notfilledcnt*notfilledcnt)/50.0f);
			osDelay(2);
		}
	}
	else if(dir == 3)
	{
		LED[2]  |= 0x003000;
		BRIGHT[2][12] = BRIGHT[2][13] = BRIGHT_MAX;
		osDelay(15);
		LED[1]  |= 0x004800;
		BRIGHT[1][11] = BRIGHT[1][14] = BRIGHT_MAX;
		osDelay(15);
		LED[1]  |= 0x00CC00;
		BRIGHT[1][10] = BRIGHT[1][15] = BRIGHT_MAX;
		osDelay(15);
		LED[0]  |= 0x10200;
		BRIGHT[0][9] = BRIGHT[0][16] = BRIGHT_MAX;
		osDelay(15);
		LED[0]  |= 0x30300;
		BRIGHT[0][8] = BRIGHT[0][17] = BRIGHT_MAX;
		osDelay(15);
		LED[0]  |= 0x70380;
		BRIGHT[0][7] = BRIGHT[0][18] = BRIGHT_MAX;
		osDelay(15);
		LED[0]  |= 0xF03C0;
		BRIGHT[0][6] = BRIGHT[0][19] = BRIGHT_MAX;
		osDelay(15);
		LED[1]  |= 0x10CC20;
		BRIGHT[1][5] = BRIGHT[1][20] = BRIGHT_MAX;
		osDelay(15);
		LED[1]  |= 0x30CC30;
		BRIGHT[1][4] = BRIGHT[1][21] = BRIGHT_MAX;
		osDelay(15);
		LED[2]  |= 0x403008;
		BRIGHT[2][3] = BRIGHT[2][22] = BRIGHT_MAX;
		osDelay(15);
		LED[3]  |= 0x800004;
		BRIGHT[3][2] = BRIGHT[3][23] = BRIGHT_MAX;
		osDelay(15);
		LED[4]  |= 0x1000002;
		BRIGHT[4][1] = BRIGHT[4][24] = BRIGHT_MAX;
		osDelay(15);
		LED[5]  |= 0x1000002;
		BRIGHT[5][1] = BRIGHT[5][24] = BRIGHT_MAX;
		osDelay(15);
		LED[6]  |= 0x2000001;
		BRIGHT[6][0] = BRIGHT[6][25] = BRIGHT_MAX;
		osDelay(15);
		LED[7]  |= 0x2000001;
		BRIGHT[7][0] = BRIGHT[7][25] = BRIGHT_MAX;
		osDelay(15);
		LED[8]  |= 0x2000001;
		BRIGHT[8][0] = BRIGHT[8][25] = BRIGHT_MAX;
		osDelay(15);
		LED[9]  |= 0x2000001;
		BRIGHT[9][0] = BRIGHT[9][25] = BRIGHT_MAX;
		osDelay(15);
		LED[10] |= 0x1000002;
		BRIGHT[10][1] = BRIGHT[10][24] = BRIGHT_MAX;
		osDelay(15);
		LED[11] |= 0x1000002;
		BRIGHT[11][1] = BRIGHT[11][24] = BRIGHT_MAX;
		osDelay(15);
		LED[12] |= 0x800004;
		BRIGHT[12][2] = BRIGHT[12][23] = BRIGHT_MAX;
		osDelay(15);
		LED[13] |= 0x400008;
		BRIGHT[13][3] = BRIGHT[13][22] = BRIGHT_MAX;
		osDelay(15);
		LED[14] |= 0x200010;
		BRIGHT[14][4] = BRIGHT[14][21] = BRIGHT_MAX;
		osDelay(15);
		LED[15] |= 0x100020;
		BRIGHT[15][5] = BRIGHT[15][20] = BRIGHT_MAX;
		osDelay(15);
		LED[16] |= 0x80040;
		BRIGHT[16][6] = BRIGHT[16][19] = BRIGHT_MAX;
		osDelay(15);
		LED[17] |= 0x40080;
		BRIGHT[17][7] = BRIGHT[17][18] = BRIGHT_MAX;
		osDelay(15);
		LED[18] |= 0x20100;
		BRIGHT[18][8] = BRIGHT[18][17] = BRIGHT_MAX;
		osDelay(15);
		LED[19] |= 0x10200;
		BRIGHT[19][9] = BRIGHT[19][16] = BRIGHT_MAX;
		osDelay(15);
		LED[20] |= 0x8400;
		BRIGHT[20][10] = BRIGHT[20][15] = BRIGHT_MAX;
		osDelay(15);
		LED[21] |= 0x4800;
		BRIGHT[21][11] = BRIGHT[21][14] = BRIGHT_MAX;
		osDelay(15);
		LED[22] |= 0x3000;
		BRIGHT[22][12] = BRIGHT[22][13] = BRIGHT_MAX;
		osDelay(15);
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
	for(i=0;i<COUNT_CATHODES;i++,data++)
		*data = LED[i];
  for(i=0;i<(COUNT_CATHODES*COUNT_ANODES/4);i++,data++)
		*data = ((uint32_t*)BRIGHT)[i];
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
