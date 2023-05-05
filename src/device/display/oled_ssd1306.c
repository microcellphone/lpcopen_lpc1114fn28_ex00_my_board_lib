#include "oled_ssd1306.h"

/************************** PRIVATE DEFINTIONS ***********************/
uint8_t X_Witch;
uint8_t Y_Witch;
uint32_t Font_Wrod;

uint8_t Char_Color;
uint8_t Font_type;

//======================================
void Oled_init();
void Write_command(uint8_t command);
void Write_data(uint8_t date);
void full_clr (uint8_t state);
void FontSet(uint8_t Font_NUM);
void PutChar(uint8_t X_axis,uint8_t Y_axis,char offset);

//===========clear oled=========================
uint8_t page = 0;

void full_clr (uint8_t state)
{
  uint8_t k,i;

  for(k=0;k<8;k++) {
    Write_command(0xb0+k);
    Write_command(0x10);
    Write_command(0x00);
    for(i=0;i<128;i++) {
      Write_data(state);
    }
  }
}

//===============================================
void Oled_init(void)
{
 	SSD1306_CS_SET();		/* CS=H		*/
 	SSD1306_CS_CLR();		/* CS=L		*/
 	SSD1306_RES_CLR();	/* RES=L	*/
  Delay(20);
  SSD1306_RES_SET();	/* RES=H	*/
  Delay(50);

  Write_command(0xAE);     //Set Display Off
  Write_command(0xd5);     //display divide ratio/osc. freq. mode
  Write_command(0x80);     //
  Write_command(0xA8);     //multiplex ration mode:63
  Write_command(0x3F);
  Write_command(0xD3);     //Set Display Offset
  Write_command(0x00);
  Write_command(0x40);     //Set Display Start Line
  Write_command(0x8D);     //Set Display Offset
  // Write_command(0x10);
  Write_command(0x14);
  Write_command(0xA1);     //Segment Remap
  Write_command(0xC8);     //Sst COM Output Scan Direction
  //Write_command(0xC8);
  Write_command(0xDA);     //common pads hardware: alternative
  Write_command(0x12);
  Write_command(0x81);     //contrast control
  //Write_command(0x9F);     //
  Write_command(0x66);
  Write_command(0xD9);	   //set pre-charge period
  //Write_command(0x22);	    //set period 1:1;period 2:15
  Write_command(0xF1);
  Write_command(0xDB);     //VCOM deselect level mode
  Write_command(0x40);	   //set Vvcomh=0.83*Vcc
  Write_command(0xA4);     //Set Entire Display On/Off
  Write_command(0xA6);     //Set Normal Display
  Write_command(0xAF);     //Set Display On
}

//===========write command=========================
void Write_command(uint8_t command)
{
  SSD1306_CS_CLR();		/* CS=L		     */
  SSD1306_DC_CLR();		/* DC=L		     */
  SSP0_Send_Request(command);
  SSD1306_CS_SET();
}

//===========write data=================================
void Write_data(uint8_t date)
{
  SSD1306_CS_CLR();		/* CS=L		     */
  SSD1306_DC_SET();		/* DC=H		     */
  SSP0_Send_Request(date);
  SSD1306_CS_SET();		/* CS=H		     */
}

extern uint8_t zimo_me_ASCII[];
//extern uint8_t gImage_12w[];
const uint8_t  *Char_TAB;

void FontSet(uint8_t Font_NUM)
{
  switch(Font_NUM) {
    case 0: Font_Wrod = 690;
            X_Witch = 120;
            Y_Witch = 5;
            Char_Color = 1;
            Font_type = 0;
            Char_TAB = zimo_me_ASCII;
//            Char_TAB = gImage_12w;
    break;
    default: break;
  }
}

void PutChar(uint8_t X_axis,uint8_t Y_axis,char offset)
{
	uint8_t i,j;
	const uint8_t *p_data;

	if(Font_type==1) {
		p_data = Char_TAB + (offset-32)*Font_Wrod;
	}	else	{
		p_data = Char_TAB + offset*Font_Wrod;
	}
	for(j=0; j<Y_Witch; j++) {
		if((Y_axis+j) < (OLED_Y_MAX))	{
			Write_command(((Y_axis/8+j))+0xb0);
			Write_command(((X_axis&0xf0) >> 4)|0x10);
			Write_command(X_axis&0x0f);
			for(i=0;i<X_Witch;i++)	{
				if(((X_axis+i) < OLED_X_MAX))	{
					Write_data(*p_data++);
				}
			}
		}
	}
}
