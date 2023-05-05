#include <string.h>
#include "my_lpc1114fn28.h"

/************************** PRIVATE DEFINTIONS ***********************/
//======================================
#define OLED_X_MAX	128-1
#define OLED_Y_MAX	64-1
/* Display Contol Macros */
#define SSD1306_CS_SET()	  Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, true);    //CHIP SELECT
#define SSD1306_CS_CLR()	  Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, false);
#define SSD1306_RES_SET()  Chip_GPIO_SetPinState(LPC_GPIO, 1, 5, true);   //P0.24
#define SSD1306_RES_CLR()  Chip_GPIO_SetPinState(LPC_GPIO, 1, 5, false);
#define SSD1306_DC_SET()	  Chip_GPIO_SetPinState(LPC_GPIO, 1, 4, true);   //P0.23
#define SSD1306_DC_CLR()	  Chip_GPIO_SetPinState(LPC_GPIO, 1, 4, false);

/* Potiner Access Macro(Little Endian) */
#define	LD_UINT16(ptr)		(uint16_t)(((uint16_t)*(uint8_t*)((ptr)+1)<<8)|(uint16_t)*(uint8_t*)(ptr))
#define	LD_UINT32(ptr)		(uint32_t)(((uint32_t)*(uint8_t*)((ptr)+3)<<24)|((uint32_t)*(uint8_t*)((ptr)+2)<<16)|((uint16_t)*(uint8_t*)((ptr)+1)<<8)|*(uint8_t*)(ptr))

//======================================
extern void Oled_init();
extern void Write_command(uint8_t command);
extern void Write_data(uint8_t date);
extern void full_clr (uint8_t state);
extern void FontSet(uint8_t Font_NUM);
extern void PutChar(uint8_t X_axis,uint8_t Y_axis,char offset);
//======================================
extern void SSP0_Send_Request(uint8_t dat);
extern void SSP0_Send_Request16(uint16_t dat);
