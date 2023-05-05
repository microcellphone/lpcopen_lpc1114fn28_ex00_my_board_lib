#include <my_ssp.h>

SSP_ConfigFormat ssp_format;
Chip_SSP_DATA_SETUP_T xf_setup;
uint8_t  isXferCompleted;

void SPI_Config_Request(uint32_t bitlen, uint32_t speed, uint32_t spitype)
{
  uint32_t i;
  uint32_t Dummy;

  if (spitype == CS_AUTO) {
	  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, (IOCON_FUNC1 | IOCON_MODE_INACT));
  } else {// if (spitype == CS_GPIO)
	  Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 2); // PIO0_2 out
	  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, (IOCON_FUNC0 | IOCON_MODE_INACT));
	  Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, true); // high
  }

  Chip_SSP_Init(LPC_SSP0);
  Chip_SSP_SetBitRate(LPC_SSP0, 1000000);

  ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
  ssp_format.bits        = SSP_DATA_BITS;
  ssp_format.clockMode   = SSP_CLOCK_MODE0;
  Chip_SSP_SetFormat(LPC_SSP0, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
  Chip_SSP_SetMaster(LPC_SSP0, 1);
  Chip_SSP_Enable(LPC_SSP0);

  NVIC_EnableIRQ(SSP0_IRQn);
  NVIC_SetPriority(SSP0_IRQn, 3);
}


void SSP0_IRQHandler(void)
{
	Chip_SSP_Int_Disable(LPC_SSP0);
	if (SSP_DATA_BYTES(ssp_format.bits) == 1) {
		Chip_SSP_Int_RWFrames8Bits(LPC_SSP0, &xf_setup);
	}	else {
		Chip_SSP_Int_RWFrames16Bits(LPC_SSP0, &xf_setup);
	}

	if ((xf_setup.rx_cnt != xf_setup.length) || (xf_setup.tx_cnt != xf_setup.length)) {
		Chip_SSP_Int_Enable(LPC_SSP0);
	}	else {
		isXferCompleted = 1;
	}
}

void  SPI_Tx_Rx_Data(uint8_t *txdata, uint8_t tx_length, uint8_t *rxdata, uint8_t rx_length)
{
  xf_setup.length  = tx_length;
  xf_setup.tx_data = txdata;
  xf_setup.rx_data = rxdata;
  xf_setup.tx_cnt  = 0;
  xf_setup.rx_cnt  = 0;

#if POLLING_MODE
  Chip_SSP_RWFrames_Blocking(LPC_SSP0, &xf_setup);
#elif INTERRUPT_MODE
  isXferCompleted  = 0;
  Chip_SSP_Int_FlushData(LPC_SSP0);/* flush dummy data from SSP FiFO */
  if (SSP_DATA_BYTES(ssp_format.bits) == 1) {
	  Chip_SSP_Int_RWFrames8Bits(LPC_SSP0, &xf_setup);
  } else {
	  Chip_SSP_Int_RWFrames16Bits(LPC_SSP0, &xf_setup);
  }
  Chip_SSP_Int_Enable(LPC_SSP0);			/* enable interrupt */
  while (!isXferCompleted);
#endif

	return;
}


uint32_t SPI_TxRxData(uint32_t txdata)
{
  uint32_t rxdata;

  xf_setup.length  = 1;
  xf_setup.tx_data = &txdata;
  xf_setup.rx_data = &rxdata;
  xf_setup.tx_cnt  = 0;
  xf_setup.rx_cnt  = 0;

#if POLLING_MODE
  Chip_SSP_RWFrames_Blocking(LPC_SSP0, &xf_setup);
#elif INTERRUPT_MODE
  Chip_SSP_Int_FlushData(LPC_SSP0);/* flush dummy data from SSP FiFO */
  if (SSP_DATA_BYTES(ssp_format.bits) == 1) {
	  Chip_SSP_Int_RWFrames8Bits(LPC_SSP0, &xf_setup);
  } else {
	  Chip_SSP_Int_RWFrames16Bits(LPC_SSP0, &xf_setup);
  }

  Chip_SSP_Int_Enable(LPC_SSP0);
	while (!isXferCompleted) {}
#endif

  return rxdata;
}


void SPI_TxData(uint32_t txdata)
{
    SPI_TxRxData(txdata);
}


uint32_t SPI_RxData(void)
{
    uint32_t rxdata;

    rxdata = SPI_TxRxData(0xff);
    return rxdata;
}


void SSP0_Send_Request(uint8_t dat)
{
  SPI_TxData(dat);
}

void SSP0_Send_Request16(uint16_t dat)
{
  SSP0_Send_Request((uint8_t)(dat>>8));
  SSP0_Send_Request((uint8_t)dat);
}
