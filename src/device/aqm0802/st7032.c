#include "chip.h"
#include "type.h"
#include "aqm0802.h"
#include <my_i2c.h>
#include "st7032.h"


static uint8_t aqm0802_data[I2C_SLAVE_BUF_SIZE + 1];

void ST7032_i2c_send_command(uint8_t cmd)
{
	int write_count;

	aqm0802_data[0] = ST7032_RS_CMD_WRITE;
	aqm0802_data[1] = cmd;

	write_count = Chip_I2C_MasterSend(I2C0, I2C_SLAVE_AQM0802_ADDR>>1, aqm0802_data, 2);
	if(write_count!=2)while(1);
}

void ST7032_i2c_send_data(uint8_t data)
{
	int write_count;

	aqm0802_data[0] = ST7032_RS_DATA_WRITE;
	aqm0802_data[1] = data;

	write_count = Chip_I2C_MasterSend(I2C0, I2C_SLAVE_AQM0802_ADDR>>1, aqm0802_data, 2);
	if(write_count!=2)while(1);
}
