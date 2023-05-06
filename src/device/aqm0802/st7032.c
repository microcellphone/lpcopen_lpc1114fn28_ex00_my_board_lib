#include "aqm0802.h"
#include "st7032.h"
//#include "type.h"


void ST7032_i2c_send_command(uint8_t cmd)
{
	int write_count;
	uint8_t st7032_command[I2C_SLAVE_BUF_SIZE + 1];

	st7032_command[0] = ST7032_RS_CMD_WRITE;
	st7032_command[1] = cmd;

	write_count = Chip_I2C_MasterSend(I2C0, I2C_SLAVE_AQM0802_ADDR>>1, st7032_command, 2);
	if(write_count!=2)while(1);
}

void ST7032_i2c_send_data(uint8_t data)
{
	int write_count;
	uint8_t st7032_data[I2C_SLAVE_BUF_SIZE + 1];

	st7032_data[0] = ST7032_RS_DATA_WRITE;
	st7032_data[1] = data;

	write_count = Chip_I2C_MasterSend(I2C0, I2C_SLAVE_AQM0802_ADDR>>1, st7032_data, 2);
	if(write_count!=2)while(1);
}
