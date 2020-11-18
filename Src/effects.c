#include "font.h"
#include "effects.h"
#include <string.h>


uint32_t LED[23];
uint8_t BRIGHT[23][26];

#define GPIO_SetBit(PORT,PIN) (PORT->BSRR = PIN)
#define GPIO_ResetBit(PORT,PIN) (PORT->BSRR = PIN<<16)

#define GPIO_SetX(PORT,PIN,X) (PORT->BSRR = PIN<<(16*((X)&1)))

volatile static uint8_t irq_counter = 0;
volatile static uint8_t irq_brightcounter = 0;

static inline void AnodesEnable(uint32_t data)
{
  uint16_t data_l = data & 0xFFFF;
  uint16_t data_l_inv = data_l ^ 0xFFFF;
  uint16_t data_m = (data >> 16) & 0x3FF;
  uint16_t data_m_inv = data_m ^ 0x3FF;
  GPIOB->BSRR = (data_l << 16) | data_l_inv;
  GPIOD->BSRR = (data_m << 16) | data_m_inv;
}

static inline void AnodesDisable(uint32_t data)
{
  GPIOB->BSRR = 0xFFFF;
  GPIOD->BSRR = 0x3FF;
}


static void setbr(uint8_t row, uint32_t data, uint8_t br)
{
	uint8_t i = 0;
	for(i=0;i<26;i++)
	{
		if(((data>>i)&1) == 1) BRIGHT[row][i] = br;
	}
}

static void AppendAnodes(void) {
	GPIO_SetX(X0_GPIO_Port, X0_Pin, BRIGHT[irq_counter][0]>irq_brightcounter ? LED[irq_counter] : 0);

	DelayUs(15);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7)
	{

	}
}

void effect_fill(uint8_t k)
{
	uint8_t i,j;
	for(i=0;i<23;i++)
		LED[i] = k*0x3FFFFFF;
	for(i=0;i<23;i++)
		for(j=0;j<26;j++)
			BRIGHT[i][j] = 250;
}

void effect_boot(void)
{
	effect_fill(0);
	DelayMs(1000);
	
	LED[22] = 0x1000;
	DelayMs(1000);
	LED[22] = 0x2000;
	DelayMs(1000);
	LED[22] = 0x1000;
	DelayMs(1000);
	LED[22] = 0x2000;
	DelayMs(1000);
	LED[22] = 0x1000;
	DelayMs(1000);
	LED[22] = 0x2000;
	DelayMs(1000);
	effect_fill(0);
	DelayMs(2000);
}

char tempstr[255] = {' ',' ',' ',' ',' ',0};
uint32_t original[7];
uint32_t original_br[7][26];


void effect_putch(char ch, uint8_t x, uint8_t y)
{
	uint8_t k,l;
	
	for(k=0;k<7;k++)
	{
		original[k] = LED[k+y];
		for(l=0;l<26;l++)
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
		for(i=0;i<26;i++)
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
			DelayMs(60);
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
uint16_t notfilled[26*23];
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
		uint16_t random = 0;
		uint8_t i,j;
		while(1)
		{
			notfilledcnt = 0;
			for(i=0;i<23;i++)
				for(j=0;j<26;j++)
					if(((LED[i]>>j)&1) == 0)
						notfilled[notfilledcnt++] = (i*26)+j; 
			if(notfilledcnt == 0) break;
			random = (Delay_Tick/14)%notfilledcnt;
			LED[notfilled[random]/26] |= 1<<(notfilled[random]%26);
			DelayUs((notfilledcnt*notfilledcnt)/50.0f);
		}
	}
	else if(dir == 3)
	{
		LED[2]  |= 0x003000;
		BRIGHT[2][12] |= BRIGHT[2][13] |= 250;
		DelayMs(15);
		LED[1]  |= 0x004800;
		BRIGHT[1][11] |= BRIGHT[1][14] |= 250;
		DelayMs(15);
		LED[1]  |= 0x00CC00;
		BRIGHT[1][10] |= BRIGHT[1][15] |= 250;
		DelayMs(15);
		LED[0]  |= 0x10200;
		BRIGHT[0][9] |= BRIGHT[0][16] |= 250;
		DelayMs(15);
		LED[0]  |= 0x30300;
		BRIGHT[0][8] |= BRIGHT[0][17] |= 250;
		DelayMs(15);
		LED[0]  |= 0x70380;
		BRIGHT[0][7] |= BRIGHT[0][18] |= 250;
		DelayMs(15);
		LED[0]  |= 0xF03C0;
		BRIGHT[0][6] |= BRIGHT[0][19] |= 250;
		DelayMs(15);
		LED[1]  |= 0x10CC20;
		BRIGHT[1][5] |= BRIGHT[1][20] |= 250;
		DelayMs(15);
		LED[1]  |= 0x30CC30;
		BRIGHT[1][4] |= BRIGHT[1][21] |= 250;
		DelayMs(15);
		LED[2]  |= 0x403008;
		BRIGHT[2][3] |= BRIGHT[2][22] |= 250;
		DelayMs(15);
		LED[3]  |= 0x800004;
		BRIGHT[3][2] |= BRIGHT[3][23] |= 250;
		DelayMs(15);
		LED[4]  |= 0x1000002;
		BRIGHT[4][1] |= BRIGHT[4][24] |= 250;
		DelayMs(15);
		LED[5]  |= 0x1000002;
		BRIGHT[5][1] |= BRIGHT[5][24] |= 250;
		DelayMs(15);
		LED[6]  |= 0x2000001;
		BRIGHT[6][0] |= BRIGHT[6][25] |= 250;
		DelayMs(15);
		LED[7]  |= 0x2000001;
		BRIGHT[7][0] |= BRIGHT[7][25] |= 250;
		DelayMs(15);
		LED[8]  |= 0x2000001;
		BRIGHT[8][0] |= BRIGHT[8][25] |= 250;
		DelayMs(15);
		LED[9]  |= 0x2000001;
		BRIGHT[9][0] |= BRIGHT[9][25] |= 250;
		DelayMs(15);
		LED[10] |= 0x1000002;
		BRIGHT[10][1] |= BRIGHT[10][24] |= 250;
		DelayMs(15);
		LED[11] |= 0x1000002;
		BRIGHT[11][1] |= BRIGHT[11][24] |= 250;
		DelayMs(15);
		LED[12] |= 0x800004;
		BRIGHT[12][2] |= BRIGHT[12][23] |= 250;
		DelayMs(15);
		LED[13] |= 0x400008;
		BRIGHT[13][3] |= BRIGHT[13][22] |= 250;
		DelayMs(15);
		LED[14] |= 0x200010;
		BRIGHT[14][4] |= BRIGHT[14][21] |= 250;
		DelayMs(15);
		LED[15] |= 0x100020;
		BRIGHT[15][5] |= BRIGHT[15][20] |= 250;
		DelayMs(15);
		LED[16] |= 0x80040;
		BRIGHT[16][6] |= BRIGHT[16][19] |= 250;
		DelayMs(15);
		LED[17] |= 0x40080;
		BRIGHT[17][7] |= BRIGHT[17][18] |= 250;
		DelayMs(15);
		LED[18] |= 0x20100;
		BRIGHT[18][8] |= BRIGHT[18][17] |= 250;
		DelayMs(15);
		LED[19] |= 0x10200;
		BRIGHT[19][9] |= BRIGHT[19][16] |= 250;
		DelayMs(15);
		LED[20] |= 0x8400;
		BRIGHT[20][10] |= BRIGHT[20][15] |= 250;
		DelayMs(15);
		LED[21] |= 0x4800;
		BRIGHT[21][11] |= BRIGHT[21][14] |= 250;
		DelayMs(15);
		LED[22] |= 0x3000;
		BRIGHT[22][12] |= BRIGHT[22][13] |= 250;
		DelayMs(15);
	}
	else if(dir == 5)
	{
		LED[15]  |= 0x3000;
		setbr(15,0x3000,250);
		DelayMs(100);
		LED[14]  |= 0x4800;
		setbr(14,0x4800,250);
		DelayMs(100);
		LED[15]  |= 0xB400;
		setbr(15,0xB400,250);
		DelayMs(100);
		LED[16]  |= 0x8400;
		setbr(16,0x8400,250);
		DelayMs(100);
		LED[17]  |= 0x4800;
		setbr(17,0x4800,250);
		DelayMs(100);
		LED[18]  |= 0x3000;
		setbr(18,0x3000,250);
		DelayMs(100);
	}
}

void effect_smile(void)
{
		LED[5]  |= 0x10000;
		DelayMs(50);
		LED[5]  |= 0x18000;
		DelayMs(50);
		LED[5]  |= 0x1C000;
		DelayMs(50);
		LED[6]  |= 0x04000;
		DelayMs(50);
		LED[7]  |= 0x04000;
		DelayMs(50);
		LED[7]  |= 0x0C000;
		DelayMs(50);
		LED[7]  |= 0x1C000;
		DelayMs(50);
		LED[6]  |= 0x14000;
	
		DelayMs(100);
	
		LED[5]  |= 0x1C800;
		DelayMs(50);
		LED[5]  |= 0x1CC00;
		DelayMs(50);
		LED[5]  |= 0x1CE00;
		DelayMs(50);
		LED[6]  |= 0x14200;
		DelayMs(50);
		LED[7]  |= 0x1C200;
		DelayMs(50);
		LED[7]  |= 0x1C600;
		DelayMs(50);
		LED[7]  |= 0x1CE00;
		DelayMs(50);
		LED[6]  |= 0x14A00;
		
		DelayMs(100);
		
		LED[12]  |= 0x3000;
		DelayMs(150);
		LED[11]  |= 0x4800;
		DelayMs(150);
		LED[10]  |= 0x8400;
		DelayMs(150);
		LED[9]  |= 0x10200;
		DelayMs(1000);
		
		
		LED[9]  &= ~0x10200;
		DelayMs(150);
		LED[10]  &= ~0x8400;
		DelayMs(1000);
		
		LED[10]  |= 0x8400;
		DelayMs(150);
		LED[9]  |= 0x10200;
		DelayMs(1000);
		
		LED[9]  &= ~0x10200;
		DelayMs(150);
		LED[10]  &= ~0x8400;
		DelayMs(1000);
		
		LED[10]  |= 0x8400;
		DelayMs(150);
		LED[9]  |= 0x10200;
		DelayMs(1000);
}
void effect_savestate(uint32_t * data)
{
	uint16_t i = 0;
	for(i=0;i<26;i++,data++)
		*data = LED[i];
	for(i=0;i<(598/4);i++,data++)
		*data = ((uint32_t*)BRIGHT)[i];
}
void effect_restorestate(uint32_t * data)
{
	uint16_t i = 0;
	for(i=0;i<26;i++,data++)
		LED[i] = *data;
	for(i=0;i<(598/4);i++,data++)
		((uint32_t*)BRIGHT)[i] = *data;
}

void effect_decreasebright(uint32_t time)
{
	uint8_t t,i,j;
	uint32_t DeltaTime = Delay_Tick;
	time *= 1000;
	t = 8;
	while(t > 1) 
	{
		if((Delay_Tick-DeltaTime)*DelayDivider > (time/8))
		{
			t--;
			DeltaTime = Delay_Tick;
			for(i=0;i<23;i++)
				for(j=0;j<26;j++)
					if(BRIGHT[i][j] > 1)
						BRIGHT[i][j] = t;
		} 
	}
}
