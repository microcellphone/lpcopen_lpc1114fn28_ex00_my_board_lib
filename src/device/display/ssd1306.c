#include <math.h>
#include <stdlib.h>
#include "ssd1306.h"
#include "common_string.h"

// Screenbuffer
uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

// Screen object
SSD1306_t SSD1306;

void ssd1306_Reset(void)
{
 	SSD1306_CS_SET();		/* CS=H		*/
 	SSD1306_CS_CLR();		/* CS=L		*/
 	SSD1306_RES_CLR();	/* RES=L	*/
 	Delay(20);
 	SSD1306_RES_SET();	/* RES=H	*/
 	Delay(50);
}

// Send a byte to the command register
void ssd1306_SendCommand(uint8_t command)
{
	SSD1306_CS_CLR();		/* CS=L		     */
	SSD1306_DC_CLR();		/* DC=L		     */
	SSP0_Send_Request(command);
	SSD1306_CS_SET();
}

void ssd1306_SendDataByte(uint8_t date)
{
	SSD1306_CS_CLR();		/* CS=L		     */
	SSD1306_DC_SET();		/* DC=H		     */
	SSP0_Send_Request(date);
	SSD1306_CS_SET();		/* CS=H		     */
}

// Send data
void ssd1306_SendData(uint8_t* buffer, size_t buff_size)
{
	size_t cnt;
	for(cnt = 0 ; cnt < buff_size ; cnt++)
		ssd1306_SendDataByte(buffer[cnt]);
}


/* Fills the Screenbuffer with values from a given buffer of a fixed length */
SSD1306_Error_t ssd1306_FillBuffer(uint8_t* buf, uint32_t len)
{
    SSD1306_Error_t ret = SSD1306_ERR;
    if (len <= SSD1306_BUFFER_SIZE) {
        common_memcpy(SSD1306_Buffer,buf,len);
        ret = SSD1306_OK;
    }
    return ret;
}

/* Initialize the oled screen */
void ssd1306_Init(void)
{
    // Reset OLED
    ssd1306_Reset();

    // Init OLED
    ssd1306_SendCommand(SSD1306_COMMAND_SET_DISPLAY_OFF);

    ssd1306_SendCommand(SSD1306_COMMAND_SET_MEMORY_MODE);
    ssd1306_SendCommand(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid

    ssd1306_SendCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd1306_SendCommand(SSD1306_COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL);
#else
    ssd1306_SendCommand(SSD1306_COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED);
#endif

    ssd1306_SendCommand(0x00); //---set low column address
    ssd1306_SendCommand(0x10); //---set high column address

    ssd1306_SendCommand(SSD1306_COMMAND_SET_START_LINE);

    ssd1306_SetContrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_SendCommand(SSD1306_COMMAND_SET_SEG_REMAP0); // Mirror horizontally
#else
    ssd1306_SendCommand(SSD1306_COMMAND_SET_SEG_REMAP1); //--set segment re-map 0 to 127
#endif

#ifdef SSD1306_INVERSE_COLOR
    ssd1306_SendCommand(SSD1306_COMMAND_SET_INVERT_DISPLAY); //--set inverse color
#else
    ssd1306_SendCommand(SSD1306_COMMAND_SET_NORMAL_DISPLAY); //--set normal color
#endif

// Set multiplex ratio.
#if (SSD1306_HEIGHT == 128)
    // Found in the Luma Python lib for SH1106.
    ssd1306_SendCommand(0xFF);
#else
    ssd1306_SendCommand(SSD1306_COMMAND_SET_MULTIPLEX_RATIO);
#endif

#if (SSD1306_HEIGHT == 32)
    ssd1306_SendCommand(0x1F); //
#elif (SSD1306_HEIGHT == 64)
    ssd1306_SendCommand(0x3F); //
#elif (SSD1306_HEIGHT == 128)
    ssd1306_SendCommand(0x3F); // Seems to work for 128px high displays too.
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_SendCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    ssd1306_SendCommand(SSD1306_COMMAND_SET_DISPLAY_OFFSET);
    ssd1306_SendCommand(0x00); //-not offset

    ssd1306_SendCommand(SSD1306_COMMAND_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
    ssd1306_SendCommand(0xF0); //--set divide ratio

    ssd1306_SendCommand(SSD1306_COMMAND_SET_PRECHARGE_PERIOD);
    ssd1306_SendCommand(0x22); //

    ssd1306_SendCommand(SSD1306_COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION);
#if (SSD1306_HEIGHT == 32)
    ssd1306_SendCommand(0x02);
#elif (SSD1306_HEIGHT == 64)
    ssd1306_SendCommand(0x12);
#elif (SSD1306_HEIGHT == 128)
    ssd1306_SendCommand(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_SendCommand(SSD1306_COMMAND_SET_VCOMH_DESELECT_LEVEL); //--set vcomh
    ssd1306_SendCommand(0x20); //0x20,0.77xVcc

    ssd1306_SendCommand(SSD1306_COMMAND_CHARGE_PUMP); //--set DC-DC enable
    ssd1306_SendCommand(0x14); //

    //--turn on SSD1306 panel
    ssd1306_SendCommand(SSD1306_COMMAND_SET_DISPLAY_ON); //display on

    // Clear screen
    ssd1306_Fill(Black);
    
    // Flush buffer to screen
    ssd1306_RefreshScreen();
    
    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
    
    SSD1306.Initialized = 1;
}

/* Fill the whole screen with the given color */
void ssd1306_Fill(SSD1306_COLOR color) {
    uint32_t i;

    for(i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
    }
}

/* Write the screenbuffer with changed to the screen */
void ssd1306_RefreshScreen(void) {
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_SendCommand(0xB0 + i); // Set the current RAM page address.
        ssd1306_SendCommand(0x00 + SSD1306_X_OFFSET_LOWER);
        ssd1306_SendCommand(0x10 + SSD1306_X_OFFSET_UPPER);
        ssd1306_SendData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

/*
 * Draw one pixel in the screenbuffer
 * X => X Coordinate
 * Y => Y Coordinate
 * color => Pixel color
 */
void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        // Don't write outside the buffer
        return;
    }
   
    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else { 
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void ssd1306_SetContrast(const uint8_t value)
{
    const uint8_t kSetContrastControlRegister = 0x81;
    ssd1306_SendCommand(kSetContrastControlRegister);
    ssd1306_SendCommand(value);
}
