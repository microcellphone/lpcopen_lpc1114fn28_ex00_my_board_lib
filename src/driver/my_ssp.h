#ifndef __MY_SSP_H__
#define __MY_SSP_H__

#include "chip.h"

#define POLLING_MODE        0
#if POLLING_MODE
#define INTERRUPT_MODE      0
#else
#define INTERRUPT_MODE      1
#endif

#define BUFFER_SIZE                         (0x100)
#define SSP_DATA_BITS                       (SSP_BITS_8)
#define SSP_DATA_BIT_NUM(databits)          (databits + 1)
#define SSP_DATA_BYTES(databits)            (((databits) > SSP_BITS_8) ? 2 : 1)

enum SSP_SPEED {SSP_SLOW, SSP_FAST};
enum CS_TYPE  {CS_AUTO, CS_GPIO};

extern SSP_ConfigFormat ssp_format;
extern Chip_SSP_DATA_SETUP_T xf_setup;
extern uint8_t isXferCompleted;

extern void SPI_Config_Request(uint32_t bitlen, uint32_t bitrate, uint32_t spitype);
extern void  SPI_Tx_Rx_Data(uint8_t *txdata, uint8_t tx_length, uint8_t *rxdata, uint8_t rx_length);
extern uint32_t SPI_TxRxData(uint32_t txdata);
extern void SPI_TxData(uint32_t txdata);
extern uint32_t SPI_RxData(void);

#endif // __MY_SSP_H__
