#include "sst25vf032b.h"
#include "semphr.h"

#define SPI_NSS_ON() HAL_GPIO_WritePin(SPI4_NSS_GPIO_Port, SPI4_NSS_Pin, GPIO_PIN_RESET)
#define SPI_NSS_OFF() HAL_GPIO_WritePin(SPI4_NSS_GPIO_Port, SPI4_NSS_Pin, GPIO_PIN_SET)

static SPI_HandleTypeDef * hspi;
static osMutexId_t mutex;
static SemaphoreHandle_t semTx;
static SemaphoreHandle_t semRx;
static const osMutexAttr_t mutexAttrs = {
  .name = "MutexSST25",
  .attr_bits = osMutexRecursive,
  .cb_mem = NULL,
  .cb_size = 0
};

static uint8_t tx[32] __attribute__((aligned(32)));
static uint8_t rx[32] __attribute__((aligned(32)));

inline void SST25_TxCpltCallback(SPI_HandleTypeDef * _hspi)
{
  if(_hspi == hspi)
  {
    xSemaphoreGiveFromISR(semTx, pdFALSE);
  }
}

inline void SST25_RxCpltCallback(SPI_HandleTypeDef * _hspi)
{
  if(_hspi == hspi)
  {
    xSemaphoreGiveFromISR(semRx, pdFALSE);
  }
}

inline void SST25_TxRxCpltCallback(SPI_HandleTypeDef * _hspi)
{
  if(_hspi == hspi)
  {
    xSemaphoreGiveFromISR(semTx, pdFALSE);
    xSemaphoreGiveFromISR(semRx, pdFALSE);
  }
}

static inline void waitTxCplt()
{
  xSemaphoreTake(semTx, HAL_MAX_DELAY);
}

static inline void waitRxCplt()
{
  xSemaphoreTake(semRx, HAL_MAX_DELAY);
}

static inline void waitTxRxCplt()
{
  xSemaphoreTake(semRx, HAL_MAX_DELAY);
  xSemaphoreTake(semTx, HAL_MAX_DELAY);
}

static inline HAL_StatusTypeDef SPI_CheckChip(void)
{
  tx[0] = 0x90;
  tx[1] = 0;
  tx[2] = 0;
  tx[3] = 0;
  tx[4] = 0;
  tx[5] = 0;

  SCB_CleanDCache_by_Addr((uint32_t*)tx, 6);

  SPI_NSS_ON();
  HAL_SPI_TransmitReceive_DMA(hspi, tx, rx, 6);
  waitTxRxCplt();
  SPI_NSS_OFF();

  SCB_InvalidateDCache_by_Addr((uint32_t*)rx, 6);

  if(rx[4] != 0xBF)
    return HAL_ERROR;
  if(rx[5] != 0x4A)
    return HAL_ERROR;

  return HAL_OK;
}

static inline void SPI_Read(uint32_t address, uint32_t size, uint8_t * buffer)
{
  tx[0] = 0x0B;
  tx[1] = (address >> 16) & 0xFF;
  tx[2] = (address >> 8) & 0xFF;
  tx[3] = address & 0xFF;
  tx[4] = 0xFF;

  SCB_CleanDCache_by_Addr((uint32_t*)tx, 5);

  SPI_NSS_ON();
  HAL_SPI_Transmit_DMA(hspi, tx, 5);
  waitTxCplt();
  HAL_SPI_Receive_DMA(hspi, buffer, size);
  waitRxCplt();
  SPI_NSS_OFF();

  if((uint32_t)buffer % 32 == 0)
    SCB_InvalidateDCache_by_Addr((uint32_t*)buffer, size);
  else SCB_InvalidateDCache_by_Addr((uint32_t*)((uint32_t)buffer - ((uint32_t)buffer%32)), size + ((uint32_t)buffer%32));
}


HAL_StatusTypeDef SST25_Init(SPI_HandleTypeDef * _hspi)
{
  if(mutex == NULL)
    mutex = osMutexNew(&mutexAttrs);
  if(semTx == NULL)
    semTx = xSemaphoreCreateBinary();
  if(semRx == NULL)
    semRx = xSemaphoreCreateBinary();
  hspi = _hspi;

  osStatus status = osMutexAcquire(mutex, HAL_MAX_DELAY);
  if(status == osOK)
  {
    if(SPI_CheckChip() != HAL_OK)
      status = osError;
    osMutexRelease(mutex);
  }
  return status == osOK ? HAL_OK : HAL_ERROR;
}

void SST25_Read(uint32_t address, uint32_t size, uint8_t * buffer)
{
  osStatus status = osMutexAcquire(mutex, HAL_MAX_DELAY);
  if(status == osOK)
  {
    SPI_Read(address & 0x3FFFFF, size, buffer);
    osMutexRelease(mutex);
  }
}

