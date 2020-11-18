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
  uint8_t tx[6] = {0x90,0,0,0,0,0};
  uint8_t rx[6];
  SPI_NSS_ON();
  HAL_SPI_TransmitReceive_DMA(hspi, tx, rx, sizeof(tx));
  waitTxRxCplt();
  SPI_NSS_OFF();

  if(rx[4] != 0xBF)
    return HAL_ERROR;
  if(rx[5] != 0x4A)
    return HAL_ERROR;

  return HAL_OK;
}

static inline void SPI_Read(uint32_t address, uint32_t size, uint8_t * buffer)
{
  uint8_t tx[5] = {0x0B, (address >> 16) & 0xFF, (address >> 8) & 0xFF, address & 0xFF, 0xFF};
  SPI_NSS_ON();
  HAL_SPI_Transmit_DMA(hspi, tx, sizeof(tx));
  waitTxCplt();
  HAL_SPI_Receive_DMA(hspi, buffer, size);
  waitRxCplt();
  SPI_NSS_OFF();
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

