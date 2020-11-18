/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "effects.h"
#include "outputs.h"
#include "main.h"
#include "sst25vf032b.h"
#include "mp3common.h"
#include <string.h>

#define MP3_BUFFER_SIZE 2304
#define SAMPLING_BUFFER_SIZE (MP3_BUFFER_SIZE * 2)

#define FLASH_BUFFER_SIZE 2048
#define FLASH_BUFFER_COUNT 16

#define INTERNAL_BUFFER_SIZE (FLASH_BUFFER_SIZE * 8)

static uint8_t FlashBuffers[FLASH_BUFFER_COUNT][FLASH_BUFFER_SIZE];

#define BUFFER_HALF_CPLT 1
#define BUFFER_FULL_CPLT 2

static uint16_t DacBufferL[SAMPLING_BUFFER_SIZE] __attribute__((aligned(32)));
static uint16_t DacBufferR[SAMPLING_BUFFER_SIZE] __attribute__((aligned(32)));
static int16_t Mp3Buffer[MP3_BUFFER_SIZE];
static volatile uint32_t DacPointerL = 0;
static volatile uint32_t DacPointerR = 0;

extern SPI_HandleTypeDef hspi4;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
static osMessageQueueId_t queueDacL;
static osMessageQueueId_t queueDacR;
static osMessageQueueId_t queueSpiResetTx;
static osMessageQueueId_t queueSpiResetRx;
static osMessageQueueId_t queueBufferNumber;


void StartControlTask(void *argument)
{

  effect_start(&htim7);



}


static HMP3Decoder MP3;
static MP3FrameInfo MP3Info;
static int samplerate = 0;
static int curbuffersize = 0;
static uint8_t isplaying = 0;


static void MP3_PeriodElapsedCplt(DMA_HandleTypeDef *hdma)
{
  uint8_t message = BUFFER_FULL_CPLT;
  if(hdma == htim4.hdma[TIM_DMA_ID_UPDATE])
    osMessageQueuePut(queueDacL, &message, 0, 0);
  else if(hdma == htim6.hdma[TIM_DMA_ID_UPDATE])
    osMessageQueuePut(queueDacR, &message, 0, 0);
}

static void MP3_PeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma)
{
  uint8_t message = BUFFER_HALF_CPLT;
  if(hdma == htim4.hdma[TIM_DMA_ID_UPDATE])
    osMessageQueuePut(queueDacL, &message, 0, 0);
  else if(hdma == htim6.hdma[TIM_DMA_ID_UPDATE])
    osMessageQueuePut(queueDacR, &message, 0, 0);
}

static void StopPlayback(void)
{
  if(isplaying)
  {
    taskENTER_CRITICAL();
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

    HAL_DMA_Abort(htim4.hdma[TIM_DMA_ID_UPDATE]);
    HAL_DMA_Abort(htim6.hdma[TIM_DMA_ID_UPDATE]);

    __HAL_TIM_DISABLE_DMA(&htim4, TIM_DMA_UPDATE);
    __HAL_TIM_DISABLE_DMA(&htim6, TIM_DMA_UPDATE);
    __HAL_TIM_DISABLE(&htim4);
    __HAL_TIM_DISABLE(&htim6);
    taskEXIT_CRITICAL();

    osMessageQueueReset(queueDacL);
    osMessageQueueReset(queueDacR);
    isplaying = 0;
  }
}

static void StartPlayback(uint32_t buffersize)
{
  if(isplaying)
  {
    if(buffersize == curbuffersize)
      return;
    StopPlayback();
  }

  for(int i = 0; i < curbuffersize; i++)
    DacBufferL[i] = DacBufferR[i] = 512;

  for(int i = 0; i < MP3_BUFFER_SIZE; i++)
    Mp3Buffer[i] = 0;

  taskENTER_CRITICAL();
  DacPointerL = 0;
  DacPointerR = 0;

  htim4.hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = MP3_PeriodElapsedCplt;
  htim6.hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = MP3_PeriodElapsedCplt;

  htim4.hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = MP3_PeriodElapsedHalfCplt;
  htim6.hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = MP3_PeriodElapsedHalfCplt;

  HAL_DMA_Start_IT(htim4.hdma[TIM_DMA_ID_UPDATE], (uint32_t)DacBufferL, (uint32_t)&htim1.Instance->CCR1, buffersize);
  HAL_DMA_Start_IT(htim6.hdma[TIM_DMA_ID_UPDATE], (uint32_t)DacBufferR, (uint32_t)&htim1.Instance->CCR2, buffersize);

  __HAL_TIM_ENABLE_DMA(&htim4, TIM_DMA_UPDATE);
  __HAL_TIM_ENABLE_DMA(&htim6, TIM_DMA_UPDATE);
  __HAL_TIM_ENABLE(&htim4);
  __HAL_TIM_ENABLE(&htim6);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  taskEXIT_CRITICAL();

}

void StartPlaybackTask(void *argument)
{
  uint8_t * buffer = pvPortMalloc(INTERNAL_BUFFER_SIZE);
  uint8_t * mp3buffer;
  int size = 0;
  int32_t syncword;
  uint8_t message = 0;
  osStatus_t status;
  uint16_t * initialpntl;
  uint16_t * initialpntr;

  MP3 = MP3InitDecoder();
  queueDacL = osMessageQueueNew(16, sizeof(uint8_t), NULL);
  queueDacR = osMessageQueueNew(16, sizeof(uint8_t), NULL);
  queueSpiResetRx = osMessageQueueNew(2, sizeof(uint8_t), NULL);
  queueSpiResetTx = osMessageQueueNew(2, sizeof(uint8_t), NULL);
  queueBufferNumber = osMessageQueueNew(FLASH_BUFFER_COUNT - 1, sizeof(uint8_t), NULL);
  osMessageQueuePut(queueSpiResetTx, &message, 0, 50);

  for(;;)
  {
    while(size <= INTERNAL_BUFFER_SIZE - FLASH_BUFFER_SIZE)
    {
      memcpy(&buffer[0], &buffer[INTERNAL_BUFFER_SIZE - size], size);
      do
      {
        status = osMessageQueueGet(queueBufferNumber, &message, 0, 50);
      } while(status != osOK || message >= FLASH_BUFFER_COUNT);
      memcpy(&buffer[size], FlashBuffers[message], FLASH_BUFFER_SIZE);
      size += FLASH_BUFFER_SIZE;
    }

    if(buffer[0] == 0xFF && buffer[1] == 0xFF &&
        buffer[2] == 0xFF && buffer[3] == 0xFF)
    {
      status = osMessageQueuePut(queueSpiResetTx, &message, 0, 50);
      for(int i = 0; i < curbuffersize; i++)
        DacBufferL[i] = DacBufferR[i] = 512;
      SCB_CleanDCache_by_Addr((uint32_t*)DacBufferL, curbuffersize * sizeof(uint16_t));
      SCB_CleanDCache_by_Addr((uint32_t*)DacBufferR, curbuffersize * sizeof(uint16_t));
      status = osMessageQueueGet(queueSpiResetRx, &message, 0, 1000);
      size = 0;
      continue;
    }

    syncword = MP3FindSyncWord(buffer, size);
    if(syncword <= ERR_MP3_INDATA_UNDERFLOW)
    {
      size = 0;
      continue;
    }
    if(syncword > 0)
    {
      size -= syncword;
      continue;
    }

    syncword = MP3GetNextFrameInfo(MP3, &MP3Info, buffer);
    if(syncword != ERR_MP3_NONE)
    {
      if(size > 2)
        size -= 2;
      continue;
    }

    mp3buffer = buffer;
    syncword = MP3Decode(MP3, &mp3buffer, &size, Mp3Buffer, 0);
    if(syncword != ERR_MP3_NONE)
    {
      if(size > 2)
        size -= 2;
      continue;
    }

    MP3GetLastFrameInfo(MP3, &MP3Info);

    if(samplerate != MP3Info.samprate)
    {
      samplerate = MP3Info.samprate;
      switch(samplerate)
      {
        case 96000 :
          htim4.Instance->ARR = 1041;
          htim6.Instance->ARR = 1041;
          break;
        case 48000 :
          htim4.Instance->ARR = 2082;
          htim6.Instance->ARR = 2082;
          break;
        case 44100 :
          htim4.Instance->ARR = 2267;
          htim6.Instance->ARR = 2267;
          break;
        case 22050 :
          htim4.Instance->ARR = 4534;
          htim6.Instance->ARR = 4534;
          break;
        case 11025 :
          htim4.Instance->ARR = 9069;
          htim6.Instance->ARR = 9069;
          break;
        default :
          samplerate = 0;
          break;
      }
    }

    if(samplerate == 0 || MP3Info.nChans <= 0)
      continue;

    if(MP3Info.nChans == 1)
      StartPlayback(MP3Info.outputSamps * 2);
    else if(MP3Info.nChans == 2)
      StartPlayback(MP3Info.outputSamps);
    else continue;

    status = osMessageQueueGet(queueDacL, &message, NULL, 150);
    if(status != osOK) continue;
    else if(message == BUFFER_HALF_CPLT) DacPointerL = 0;
    else if(message == BUFFER_FULL_CPLT) DacPointerL = curbuffersize / 2;

    status = osMessageQueueGet(queueDacR, &message, NULL, 150);
    if(status != osOK) continue;
    else if(message == BUFFER_HALF_CPLT) DacPointerR = 0;
    else if(message == BUFFER_FULL_CPLT) DacPointerR = curbuffersize / 2;

    initialpntl = &DacBufferL[DacPointerL];
    initialpntr = &DacBufferR[DacPointerR];

    if(MP3Info.nChans == 1)
      for(int i = 0; i < MP3Info.outputSamps;)
        DacBufferL[DacPointerL++] = DacBufferR[DacPointerR++] = (Mp3Buffer[i++] ^ 0x8000) >> 6;
    else if(MP3Info.nChans == 2)
    {
      for(int i = 0; i < MP3Info.outputSamps;)
      {
        DacBufferL[DacPointerL++] = (Mp3Buffer[i++] ^ 0x8000) >> 6;
        DacBufferR[DacPointerR++] = (Mp3Buffer[i++] ^ 0x8000) >> 6;
      }
    }

    if(DacPointerL >= curbuffersize) DacPointerL = 0;
    if(DacPointerR >= curbuffersize) DacPointerR = 0;

    SCB_CleanDCache_by_Addr((uint32_t*)initialpntl, curbuffersize / 2 * sizeof(uint16_t));
    SCB_CleanDCache_by_Addr((uint32_t*)initialpntr, curbuffersize / 2 * sizeof(uint16_t));

  }

}


void StartSpiReadTask(void *argument)
{
  uint32_t address = 0;
  uint8_t message = 0;
  uint8_t bufferIndex = 0;
  osStatus_t status;

  while(SST25_Init(&hspi4) != HAL_OK)
  {
    osDelay(100);
  }

  do
  {
    osDelay(10);
    status = osMessageQueueGet(queueSpiResetTx, &message, 0, 1000);
  } while(status != osOK);


  for(;;)
  {
    status = osMessageQueueGet(queueSpiResetTx, &message, 0, 0);
    if(status == osOK)
    {
      address = 0;
      status = osMessageQueuePut(queueSpiResetRx, &message, 0, 50);
    }

    SST25_Read(address, FLASH_BUFFER_SIZE, FlashBuffers[bufferIndex]);
    address += FLASH_BUFFER_SIZE;
    address &= 0x3FFFFF;
    do
    {
      status = osMessageQueuePut(queueBufferNumber, &bufferIndex, 0, 5);
      if(status != osOK)
      {
        status = osMessageQueueGet(queueSpiResetTx, &message, 0, 0);
        if(status == osOK)
        {
          address = 0;
          status = osMessageQueuePut(queueSpiResetRx, &message, 0, 50);
        }
      }
    } while(status != osOK);

    if(++bufferIndex >= FLASH_BUFFER_COUNT)
      bufferIndex = 0;

  }
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
  if(hspi == &hspi4)
  {
    SST25_TxCpltCallback(hspi);
  }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi)
{
  if(hspi == &hspi4)
  {
    SST25_RxCpltCallback(hspi);
  }
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi)
{
  if(hspi == &hspi4)
  {
    SST25_TxRxCpltCallback(hspi);
  }
}

