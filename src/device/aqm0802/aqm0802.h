#ifndef __AQM0802_H__
#define __AQM0802_H__

#include "my_lpc1114fn28.h"
#include "st7032.h"

/* EEPROM SLAVE data */
#define I2C_SLAVE_BUF_SIZE       6
#define I2C_SLAVE_AQM0802_ADDR   0x7c

//=============
// Prototypes
//=============
//
void AQM0802_Config_Request(void);
int AQM0802_cleardisplay(void);
int AQM0802_setcursor(int , int);
int AQM0802_puts( char * );
int AQM0802_init( int );
int AQM0802_deinit(void);


#endif // __GPSRTC_H__

//=========================================================
// End of Program
//=========================================================
