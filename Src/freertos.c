#include "FreeRTOS.h"
#include "task.h"
#include "effects.h"
#include "outputs.h"
#include "main.h"
#include "sst25vf032b.h"
#include "mp3common.h"
#include <string.h>
#include <math.h>
#include "arm_math.h"

#define MP3_BUFFER_SIZE 2304
#define SAMPLING_BUFFER_SIZE (MP3_BUFFER_SIZE * 2)

#define FLASH_BUFFER_SIZE 2048
#define FLASH_BUFFER_COUNT 16

#define ADC_BUFFER_SIZE 512
#define ADC_KOFF 0.005f
#define INS_KOFF 0.002f
#define ADC_VREF 3.42f
#define ADC_DIVIDER 2.0f

#define INTERNAL_BUFFER_SIZE (FLASH_BUFFER_SIZE * 8)


#define BUFFER_HALF_CPLT 1
#define BUFFER_FULL_CPLT 2

extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi4;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim8;
extern IWDG_HandleTypeDef hiwdg;

static uint8_t FlashBuffers[FLASH_BUFFER_COUNT][FLASH_BUFFER_SIZE] __attribute__((aligned(32)));
static uint16_t DacBufferL[SAMPLING_BUFFER_SIZE] __attribute__((aligned(32)));
static uint16_t DacBufferR[SAMPLING_BUFFER_SIZE] __attribute__((aligned(32)));
static uint16_t AdcBuffer[ADC_BUFFER_SIZE] __attribute__((aligned(32)));
static uint8_t BufferInternal[INTERNAL_BUFFER_SIZE];
static int16_t Mp3Buffer[MP3_BUFFER_SIZE];
static int16_t StereoBufferL[MP3_BUFFER_SIZE/2];
static int16_t StereoBufferR[MP3_BUFFER_SIZE/2];
static uint32_t DacPointerL = 0;
static uint32_t DacPointerR = 0;
static float adcVBATraw = 0.f;
static float adcCHRGraw = 0.f;

static volatile float pwrCharged = 0.f;
static volatile float pwrCharging = 0.f;
static volatile float pwrUsbVbus = 0.f;
static volatile float adcVBAT = 0.f;
static volatile float adcCHRG = 0.f;

static osMessageQueueId_t queueDacL;
static osMessageQueueId_t queueDacR;
static osMessageQueueId_t queueSpiResetTx;
static osMessageQueueId_t queueSpiResetRx;
static osMessageQueueId_t queueBufferNumber;

static osThreadId_t displayTaskHandle;

static void StartDisplayTask(void *argument);

typedef enum
{
  CS_Undefined = -1,
  CS_Working,
  CS_BatteryLow,
  CS_Charging,
  CS_Charged,
  CS_BatteryWarning,
} CurStatus_t;

static CurStatus_t currentStatus = CS_Undefined;


const osThreadAttr_t displayTask_attributes = {
  .name = "displayTask",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 4096 * 4
};


static inline void ADC_Handle(uint16_t * data, uint32_t size)
{
  uint32_t vbat = 0;
  uint32_t chrg = 0;
  float isUsbVbus = HAL_GPIO_ReadPin(USB_VBUS_GPIO_Port, USB_VBUS_Pin) == GPIO_PIN_SET ? 1.f : 0.f;
  float isCharging = HAL_GPIO_ReadPin(MCU_CHRG_GPIO_Port, MCU_CHRG_Pin) == GPIO_PIN_RESET ? 1.f : 0.f;
  float isCharged = HAL_GPIO_ReadPin(MCU_STBY_GPIO_Port, MCU_STBY_Pin) == GPIO_PIN_RESET ? 1.f : 0.f;

  SCB_InvalidateDCache_by_Addr((uint32_t*)data, size * sizeof(uint16_t));
  for(int i = 0; i < size;)
  {
    vbat += data[i++];
    chrg += data[i++];
  }

  adcVBATraw = adcVBATraw * (1.0f - ADC_KOFF) + vbat * ADC_KOFF;
  adcCHRGraw = adcCHRGraw * (1.0f - ADC_KOFF) + chrg * ADC_KOFF;

  adcVBAT = adcVBATraw / size / 65536.0f * 2.0f * ADC_DIVIDER * ADC_VREF;
  adcCHRG = adcCHRGraw / size / 65536.0f * 2.0f * ADC_DIVIDER * ADC_VREF;

  pwrUsbVbus = pwrUsbVbus * (1.0f - INS_KOFF) + isUsbVbus * INS_KOFF;
  pwrCharging = pwrCharging * (1.0f - INS_KOFF) + isCharging * INS_KOFF;
  pwrCharged = pwrCharged * (1.0f - INS_KOFF) + isCharged * INS_KOFF;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    ADC_Handle(&AdcBuffer[ADC_BUFFER_SIZE / 2], ADC_BUFFER_SIZE / 2);
  }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    ADC_Handle(&AdcBuffer[0], ADC_BUFFER_SIZE / 2);
  }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if((GPIO_Pin & USB_VBUS_Pin) == USB_VBUS_Pin)
  {
    if(HAL_GPIO_ReadPin(USB_VBUS_GPIO_Port, USB_VBUS_Pin) == GPIO_PIN_RESET)
    {
      HAL_GPIO_WritePin(MCU_CE_GPIO_Port, MCU_CE_Pin, GPIO_PIN_RESET);
      pwrCharging = 0.0f;
      pwrCharged = 0.0f;
      pwrUsbVbus = 0.0f;
    }
  }
}

#define IS_USB_VBUS (pwrUsbVbus > 0.7f)
#define IS_CHARGING (pwrCharging > 0.7f)
#define IS_CHARGED (pwrCharged > 0.7f)

void StartDisplayTask(void *argument)
{
  effect_start(&htim7);
  CurStatus_t status = *(CurStatus_t *)argument;

  if(status == CS_Working)
    out_main();
  else if(status == CS_BatteryLow)
    out_batterylow();
  else if(status == CS_Charging)
    out_charging();
  else if(status == CS_Charged)
    out_charging();

  effect_stop();
  taskENTER_CRITICAL();
  displayTaskHandle = NULL;
  vTaskDelete(NULL);
  taskEXIT_CRITICAL();

  while(1) osDelay(1000);
}


void StartControlTask(void *argument)
{
  const uint32_t period = 100;
  CurStatus_t oldStatus = CS_Undefined;
  uint32_t low_period = 0;
  uint32_t usb_period = 0;
  float temp;

  HAL_GPIO_WritePin(MCU_CE_GPIO_Port, MCU_CE_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)AdcBuffer, ADC_BUFFER_SIZE);
  HAL_TIM_Base_Start(&htim4);

  while(1)
  {
    HAL_IWDG_Refresh(&hiwdg);
    if(currentStatus != oldStatus)
    {
      oldStatus = currentStatus;
      if((oldStatus != CS_Charging || currentStatus != CS_Charged) && (oldStatus != CS_Charged || currentStatus != CS_Charging))
      {
        if(displayTaskHandle != NULL)
        {
          vTaskDelete(displayTaskHandle);
          displayTaskHandle = NULL;
          effect_stop();
        }
      }

      if(currentStatus == CS_BatteryLow || currentStatus == CS_Charging || currentStatus == CS_Charged || currentStatus == CS_Working)
      {
        if(displayTaskHandle == NULL)
        {
          displayTaskHandle = osThreadNew(StartDisplayTask, &currentStatus, &displayTask_attributes);
        }
      }
    }

    osDelay(period);

    if(IS_USB_VBUS && adcVBAT < 2.0f)
    {
      if(usb_period >= 500)
      {
        HAL_GPIO_WritePin(MCU_CE_GPIO_Port, MCU_CE_Pin, GPIO_PIN_SET);
        if(IS_CHARGING && !IS_CHARGED)
        {
          temp = (adcCHRG - 3.8f) * 2.5f;
          if(temp > 1.f) temp = 0.98f;
          else if(temp < 0.f) temp = 0.f;
          out_updatecharginglevel(temp);
          currentStatus = CS_Charging;
          low_period = 0;
          continue;
        }
        else if(IS_CHARGED && !IS_CHARGING)
        {
          out_updatecharginglevel(1.f);
          currentStatus = CS_Charged;
          low_period = 0;
          continue;
        }

      }
      else
        usb_period += period;
    }
    else
    {
      HAL_GPIO_WritePin(MCU_CE_GPIO_Port, MCU_CE_Pin, GPIO_PIN_RESET);
      usb_period = 0;
    }

    if(adcVBAT < 3.5f)
    {
        if(low_period > 2000)
          currentStatus = CS_BatteryLow;
        low_period += period;

    }
    else
    {
      if(currentStatus == CS_Working)
        low_period = 0;
      else if(currentStatus != CS_BatteryLow)
        currentStatus = CS_Working;
    }
  }
}


static void MP3_PeriodElapsedCplt(DMA_HandleTypeDef *hdma)
{
  uint8_t message = BUFFER_FULL_CPLT;
  if(hdma == htim8.hdma[TIM_DMA_ID_CC1])
    osMessageQueuePut(queueDacL, &message, 0, 0);
  else if(hdma == htim8.hdma[TIM_DMA_ID_CC2])
    osMessageQueuePut(queueDacR, &message, 0, 0);
}

static void MP3_PeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma)
{
  uint8_t message = BUFFER_HALF_CPLT;
  if(hdma == htim8.hdma[TIM_DMA_ID_CC1])
    osMessageQueuePut(queueDacL, &message, 0, 0);
  else if(hdma == htim8.hdma[TIM_DMA_ID_CC2])
    osMessageQueuePut(queueDacR, &message, 0, 0);
}

static int samplerate = 0;
static int curbuffersize = 0;
static uint8_t isplaying = 0;

static void StopPlayback(void)
{
  if(isplaying)
  {
    taskENTER_CRITICAL();
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);

    HAL_DMA_Abort(htim8.hdma[TIM_DMA_ID_CC1]);
    HAL_DMA_Abort(htim8.hdma[TIM_DMA_ID_CC2]);

    __HAL_TIM_DISABLE_DMA(&htim8, TIM_DMA_CC1);
    __HAL_TIM_DISABLE_DMA(&htim8, TIM_DMA_CC2);

    TIM_CCxChannelCmd(htim8.Instance, TIM_DMA_CC1, TIM_CCx_DISABLE);
    TIM_CCxChannelCmd(htim8.Instance, TIM_DMA_CC2, TIM_CCx_DISABLE);

    __HAL_TIM_DISABLE(&htim8);

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

  taskENTER_CRITICAL();
  curbuffersize = buffersize;

  for(int i = 0; i < curbuffersize; i++)
    DacBufferL[i] = DacBufferR[i] = 4096;

  SCB_CleanDCache_by_Addr((uint32_t*)DacBufferL, curbuffersize * sizeof(uint16_t));
  SCB_CleanDCache_by_Addr((uint32_t*)DacBufferR, curbuffersize * sizeof(uint16_t));

  for(int i = 0; i < MP3_BUFFER_SIZE; i++)
    Mp3Buffer[i] = 0;

  isplaying = 1;
  DacPointerL = 0;
  DacPointerR = 0;

  htim8.hdma[TIM_DMA_ID_CC1]->XferCpltCallback = MP3_PeriodElapsedCplt;
  htim8.hdma[TIM_DMA_ID_CC2]->XferCpltCallback = MP3_PeriodElapsedCplt;

  htim8.hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback = MP3_PeriodElapsedHalfCplt;
  htim8.hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback = MP3_PeriodElapsedHalfCplt;

  HAL_DMA_Start_IT(htim8.hdma[TIM_DMA_ID_CC1], (uint32_t)DacBufferL, (uint32_t)&(htim1.Instance->CCR1), curbuffersize);
  HAL_DMA_Start_IT(htim8.hdma[TIM_DMA_ID_CC2], (uint32_t)DacBufferR, (uint32_t)&(htim1.Instance->CCR2), curbuffersize);

  __HAL_TIM_ENABLE_DMA(&htim8, TIM_DMA_CC1);
  __HAL_TIM_ENABLE_DMA(&htim8, TIM_DMA_CC2);

  TIM_CCxChannelCmd(htim8.Instance, TIM_DMA_CC1, TIM_CCx_ENABLE);
  TIM_CCxChannelCmd(htim8.Instance, TIM_DMA_CC2, TIM_CCx_ENABLE);
  __HAL_TIM_ENABLE(&htim8);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  taskEXIT_CRITICAL();


}

static HMP3Decoder mp3Handler;
static MP3FrameInfo mp3Info;

extern const float HighPassFirData[512];

static arm_fir_instance_f32 fir_l;
static arm_fir_instance_f32 fir_r;
float fir_dataI[2304];
float fir_dataO[2304];

float FirStateL[(sizeof(HighPassFirData) / sizeof(HighPassFirData[0]))+(MP3_BUFFER_SIZE/2)-1];
float FirStateR[(sizeof(HighPassFirData) / sizeof(HighPassFirData[0]))+(MP3_BUFFER_SIZE/2)-1];

static void InitHighPassFilter(int samplescount)
{
  static int samplescnt = 0;
  if(samplescnt != samplescount)
  {
    samplescnt = samplescount;
    arm_fir_init_f32(&fir_l, sizeof(HighPassFirData) / sizeof(HighPassFirData[0]), (float*)HighPassFirData, FirStateL, samplescount);
    arm_fir_init_f32(&fir_r, sizeof(HighPassFirData) / sizeof(HighPassFirData[0]), (float*)HighPassFirData, FirStateR, samplescount);
  }
}

static void HighPassFilter(int16_t * bufferL, int16_t * bufferR, int samplescount)
{
  for(int i = 0; i < samplescount; i++)
    fir_dataI[i] = bufferL[i];
  arm_fir_f32(&fir_l, fir_dataI, fir_dataO, samplescount);
  for(int i = 0; i < samplescount; i++)
    bufferL[i] = fir_dataO[i];

  if(bufferL != bufferR)
  {
    for(int i = 0; i < samplescount; i++)
      fir_dataI[i] = bufferR[i];
    arm_fir_f32(&fir_r, fir_dataI, fir_dataO, samplescount);
    for(int i = 0; i < samplescount; i++)
      bufferR[i] = fir_dataO[i];
  }
}

void StartPlaybackTask(void *argument)
{
  uint8_t * mp3buffer;
  int size = 0;
  int lastfill = 0;
  int32_t syncword;
  uint8_t message = 0;
  osStatus_t status;
  uint16_t * initialpntl;
  uint16_t * initialpntr;
  CurStatus_t oldstatus = CS_Undefined;

  mp3Handler = MP3InitDecoder();
  queueDacL = osMessageQueueNew(1, sizeof(uint8_t), NULL);
  queueDacR = osMessageQueueNew(1, sizeof(uint8_t), NULL);
  queueSpiResetRx = osMessageQueueNew(2, sizeof(uint8_t), NULL);
  queueSpiResetTx = osMessageQueueNew(2, sizeof(uint8_t), NULL);
  queueBufferNumber = osMessageQueueNew(FLASH_BUFFER_COUNT - 2, sizeof(uint8_t), NULL);
  osMessageQueuePut(queueSpiResetTx, &message, 0, 50);

  while(1)
  {
    if(oldstatus != currentStatus)
    {
      oldstatus = currentStatus;
      if(oldstatus != CS_Working)
      {
        StopPlayback();
        status = osMessageQueuePut(queueSpiResetTx, &message, 0, 50);
        for(int i = 0; i < curbuffersize; i++)
          DacBufferL[i] = DacBufferR[i] = 4096;
        SCB_CleanDCache_by_Addr((uint32_t*)DacBufferL, curbuffersize * sizeof(uint16_t));
        SCB_CleanDCache_by_Addr((uint32_t*)DacBufferR, curbuffersize * sizeof(uint16_t));
        size = 0;
        do
        {
          status = osMessageQueueGet(queueSpiResetRx, &message, 0, 1000);
        } while(status != osOK);
      }
    }

    if(oldstatus != CS_Working)
      osDelay(10);
    else
    {

      if(size <= INTERNAL_BUFFER_SIZE - FLASH_BUFFER_SIZE)
      {
        memcpy(&BufferInternal[0], &BufferInternal[lastfill - size], size);
        while(size <= INTERNAL_BUFFER_SIZE - FLASH_BUFFER_SIZE)
        {
          do
          {
            status = osMessageQueueGet(queueBufferNumber, &message, 0, 50);
          } while(status != osOK || message >= FLASH_BUFFER_COUNT);
          memcpy(&BufferInternal[size], FlashBuffers[message], FLASH_BUFFER_SIZE);
          size += FLASH_BUFFER_SIZE;
        }
        lastfill = size;
        mp3buffer = BufferInternal;
      }

      syncword = MP3FindSyncWord(mp3buffer, size);
      if(syncword <= ERR_MP3_INDATA_UNDERFLOW)
      {
        if(mp3buffer[0] == 0xFF && mp3buffer[1] == 0xFF &&
            mp3buffer[2] == 0xFF && mp3buffer[3] == 0xFF)
        {
          status = osMessageQueuePut(queueSpiResetTx, &message, 0, 50);
          for(int i = 0; i < curbuffersize; i++)
            DacBufferL[i] = DacBufferR[i] = 4096;
          SCB_CleanDCache_by_Addr((uint32_t*)DacBufferL, curbuffersize * sizeof(uint16_t));
          SCB_CleanDCache_by_Addr((uint32_t*)DacBufferR, curbuffersize * sizeof(uint16_t));
          do
          {
            status = osMessageQueueGet(queueSpiResetRx, &message, 0, 1000);
          } while(status != osOK);
        }
        size = 0;
        continue;
      }
      if(syncword > 0)
      {
        mp3buffer += syncword;
        size -= syncword;
        continue;
      }

      syncword = MP3GetNextFrameInfo(mp3Handler, &mp3Info, mp3buffer);
      if(syncword != ERR_MP3_NONE)
      {
        if(size > 2)
        {
          mp3buffer += 2;
          size -= 2;
        }
        continue;
      }

      syncword = MP3Decode(mp3Handler, &mp3buffer, &size, Mp3Buffer, 0);
      if(syncword != ERR_MP3_NONE)
      {
        continue;
      }

      MP3GetLastFrameInfo(mp3Handler, &mp3Info);

      if(samplerate != mp3Info.samprate)
      {
        samplerate = mp3Info.samprate;
        htim8.Instance->ARR = roundf((float)HAL_RCC_GetPCLK2Freq() * 2.f / (float)samplerate - 1.f);
      }

      if(samplerate == 0 || mp3Info.nChans <= 0)
        continue;


      if(mp3Info.nChans == 1)
        StartPlayback(mp3Info.outputSamps * 2);
      else if(mp3Info.nChans == 2)
        StartPlayback(mp3Info.outputSamps);
      else continue;

      InitHighPassFilter(mp3Info.outputSamps / mp3Info.nChans);

      status = osMessageQueueGet(queueDacL, &message, NULL, 200);
      if(status != osOK) continue;
      else if(message == BUFFER_HALF_CPLT) DacPointerL = 0;
      else if(message == BUFFER_FULL_CPLT) DacPointerL = curbuffersize / 2;

      status = osMessageQueueGet(queueDacR, &message, NULL, 200);
      if(status != osOK) continue;
      else if(message == BUFFER_HALF_CPLT) DacPointerR = 0;
      else if(message == BUFFER_FULL_CPLT) DacPointerR = curbuffersize / 2;

      initialpntl = &DacBufferL[DacPointerL];
      initialpntr = &DacBufferR[DacPointerR];

      if(mp3Info.nChans == 1)
      {
        HighPassFilter(Mp3Buffer, Mp3Buffer, mp3Info.outputSamps);

        for(int i = 0; i < mp3Info.outputSamps; i++)
        {
          DacBufferL[DacPointerL++] = (Mp3Buffer[i] + 0x8000) >> 3;
          DacBufferR[DacPointerR++] = (0x10000 - (Mp3Buffer[i] + 0x8000)) >> 3;
        }
      }

      else if(mp3Info.nChans == 2)
      {
        int samples = mp3Info.outputSamps / 2;
        for(int i = 0, j = 0; i < samples; i++)
        {
          StereoBufferL[i] = Mp3Buffer[j++];
          StereoBufferR[i] = Mp3Buffer[j++];
        }

        HighPassFilter(StereoBufferL, StereoBufferR, samples);

        for(int i = 0; i < samples; i++)
        {
          DacBufferL[DacPointerL++] = (StereoBufferL[i] + 0x8000) >> 3;
          DacBufferR[DacPointerR++] = (0x10000 - (StereoBufferR[i] + 0x8000)) >> 3;
        }
      }

      if(DacPointerL >= curbuffersize) DacPointerL = 0;
      if(DacPointerR >= curbuffersize) DacPointerR = 0;

      SCB_CleanDCache_by_Addr((uint32_t*)initialpntl, curbuffersize / 2 * sizeof(uint16_t));
      SCB_CleanDCache_by_Addr((uint32_t*)initialpntr, curbuffersize / 2 * sizeof(uint16_t));
    }
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
          osMessageQueueReset(queueBufferNumber);
          status = osMessageQueuePut(queueSpiResetRx, &message, 0, 50);
        }
      }
    } while(status != osOK);

    if(++bufferIndex >= FLASH_BUFFER_COUNT)
      bufferIndex = 0;

  }
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef * hspi)
{
  if(hspi == &hspi4)
  {
    SST25_ErrorCallback(hspi);
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

