#include "main.h"

extern HAL_StatusTypeDef SST25_Init(SPI_HandleTypeDef * _hspi);
extern void SST25_Read(uint32_t address, uint32_t size, uint8_t * buffer);

extern void SST25_ErrorCallback(SPI_HandleTypeDef * _hspi);
extern void SST25_TxCpltCallback(SPI_HandleTypeDef * _hspi);
extern void SST25_RxCpltCallback(SPI_HandleTypeDef * _hspi);
extern void SST25_TxRxCpltCallback(SPI_HandleTypeDef * _hspi);
