#include "chip.h"
#include "my_i2c.h"


static void i2c_state_handling(I2C_ID_T id)
{
  if (Chip_I2C_IsMasterActive(id)) {
	Chip_I2C_MasterStateHandler(id);
  } else {
	Chip_I2C_SlaveStateHandler(id);
  }
}

void I2C_IRQHandler(void)
{
  i2c_state_handling(I2C0);
}

uint32_t i2c_Config_Request(TS_I2C_INFO *i2c_data)
{
	Chip_SYSCTL_PeriphReset(RESET_I2C0);

	Chip_I2C_Init(i2c_data->id);
	Chip_I2C_SetClockRate(i2c_data->id, i2c_data->speed);

	if(!i2c_data->mode) {
		Chip_I2C_SetMasterEventHandler((i2c_data->id), Chip_I2C_EventHandler);
		NVIC_EnableIRQ(I2C0_IRQn);
	} else {
		NVIC_DisableIRQ(I2C0_IRQn);
		Chip_I2C_SetMasterEventHandler((i2c_data->id), Chip_I2C_EventHandlerPolling);
	}

	return 0;
}

uint32_t i2c_Receive_Data_Request(TS_I2C_INFO *i2c_data, uint8_t *rcv_buff, uint32_t rcv_length)
{
  I2C_XFER_T xfer;
  int read_count;

  xfer.slaveAddr = i2c_data->address;
  xfer.rxBuff    = rcv_buff;
  xfer.rxSz      = rcv_length;
  read_count = Chip_I2C_MasterRead(i2c_data->id, xfer.slaveAddr, xfer.rxBuff, xfer.rxSz);
  return (uint32_t)read_count;
}

uint32_t i2c_Send_Data_Request(TS_I2C_INFO *i2c_data, uint8_t *snd_buff, uint32_t snd_length)
{
  int write_count;

  write_count = Chip_I2C_MasterSend(i2c_data->id, i2c_data->address, snd_buff, snd_length);
  return (uint32_t)write_count;
}

uint32_t i2c_Tx_Rx_Data_Request(TS_I2C_INFO *i2c_data, uint8_t *snd_buff, uint32_t snd_length, uint8_t *rcv_buff, uint32_t rcv_length)
{
  I2C_XFER_T xfer;

  xfer.slaveAddr = i2c_data->address;
  xfer.txBuff    = snd_buff;
  xfer.txSz      = snd_length;
  xfer.rxBuff    = rcv_buff;
  xfer.rxSz      = rcv_length;
  Chip_I2C_MasterTransfer(i2c_data->id, &xfer);
  return xfer.status;
}
