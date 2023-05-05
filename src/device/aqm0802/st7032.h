#ifndef _ST7032_H
#define _ST7032_H 1

#define ST7032_RS_CMD_WRITE	  	0x00
#define ST7032_RS_DATA_WRITE	0x40

#define ST7032_CLEARDISPLAY		0x01
#define ST7032_RETURNHOME		0x02
#define ST7032_ENTRYMODESET	  	0x04
#define ST7032_DISPLAYCONTROL	0x08
#define ST7032_CURSORSHIFT		0x10
#define ST7032_FUNCTIONSET		0x20
#define ST7032_SETCGRAMADDR	  	0x40
#define ST7032_SETDDRAMADDR	  	0x80

#define ST7032_ENTRYRIGHT		0x00
#define ST7032_ENTRYLEFT			0x02
#define ST7032_ENTRYSHIFTINCREMENT	0x01
#define ST7032_ENTRYSHIFTDECREMENT	0x00

#define ST7032_DISP_ON			0x04
#define ST7032_DISP_OFF			0x00
#define ST7032_DISP_CURON		0x02
#define ST7032_DISP_CUROFF		0x00
#define ST7032_DISP_BLINK		0x01
#define ST7032_DISP_NOBLINK	  	0x00

#define ST7032_DISPLAYMOVE		0x08
#define ST7032_CURSORMOVE		0x00
#define ST7032_MOVERIGHT			0x04
#define ST7032_MOVELEFT			0x00

// Functions
#define ST7032_FUNC_8BITMODE	0x10
#define ST7032_FUNC_4BITMODE	0x00
#define ST7032_FUNC_INSTABLE	0x01
#define ST7032_FUNC_2LINE		0x08
#define ST7032_FUNC_1LINE		0x00
#define ST7032_FUNC_5x10DOTS	0x04
#define ST7032_FUNC_5x8DOTS		0x00

#define ST7032_IS_OSC			0x10
#define ST7032_IS_OSC_BSQ		0x08
#define ST7032_IS_OSC_BSP		0x00
#define ST7032_IS_CONTSET1		0x70
#define ST7032_IS_CONTSET2		0x5C
#define ST7032_IS_FOLLOWER		0x60
#define ST7032_IS_FOLLOWER_ON	0x08

#define contrast_upper(x)	((x >> 4) & 0x03)
#define contrast_lower(x)	(x & 0x0F)

void ST7032_i2c_send_data(uint8_t data);
void ST7032_i2c_send_command(uint8_t cmd);

#endif
