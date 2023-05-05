#include "chip.h"
#include "rtc8564.h"
#include "my_i2c.h"

extern void Delay (unsigned long tick);

/************************** PRIVATE DEFINITIONS *************************/
//======================
// Time Zone Difference
//======================
#define TZD (+9) // Japan

//====================
// RTC Device Address
//====================
//#define RTC_DEV_ADDR 0xa2

//=======================
// RTC Register Address
//=======================
#define RTC_CONTROL1 0x00
#define RTC_CONTROL2 0x01
#define RTC_SECONDS  0x02
#define RTC_MINUTES  0x03
#define RTC_HOURS    0x04
#define RTC_DAYS     0x05
#define RTC_WEEKDAYS 0x06
#define RTC_C_MONTHS 0x07
#define RTC_YEARS    0x08
#define RTC_MINUTE_ALARM  0x09
#define RTC_HOUR_ALARM    0x0a
#define RTC_DAY_ALARM     0x0b
#define RTC_WEEKDAY_ALARM 0x0c
#define RTC_CLKOUT_FREQ   0x0d
#define RTC_TIMER_CONTROL 0x0e
#define RTC_TIMER         0x0f

//=====================
// BCD from Integer
//=====================
uint8_t BCD_INT(uint8_t num)
{
  return ((num / 10) << 4) + (num % 10);
}

//=====================
// Integer from BCD
//=====================
uint8_t INT_BCD(uint8_t bcd)
{
  return (((bcd >> 4) * 10) + (bcd & 0x0f));
}

//===================
// Get Week String
//===================
uint8_t *Get_Week_String(uint32_t week)
{
  static const char *WEEK[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

  return (uint8_t*) WEEK[week];
}


//=====================
// RTC Write Register
//=====================

void RTC8564_Write_Reg(uint32_t addr, uint32_t data)
{
  int write_count;
  uint8_t snd_data[2];

  snd_data[0] = (uint8_t)addr;
  snd_data[1] = (uint8_t)data;

  write_count = Chip_I2C_MasterSend(I2C0, I2C2_DEV_S_OWN_ADDR, snd_data, 2);
  if (write_count != 2) while(1);
}
//====================
// RTC Read Register
//====================

uint32_t RTC8564_Read_Reg(uint32_t addr)
{
  I2C_XFER_T  xfer;
  uint8_t snd_data, rcv_data;

  snd_data = (uint8_t)addr;

  xfer.slaveAddr = I2C2_DEV_S_OWN_ADDR;
  xfer.txBuff    = &snd_data;
  xfer.txSz      = 1;
  xfer.rxBuff    = &rcv_data;
  xfer.rxSz      = 1;
  Chip_I2C_MasterTransfer(I2C0, &xfer);
  if (xfer.status != I2C_STATUS_DONE) while(1);

  return (uint32_t)rcv_data;
}

void RTC8564_Config_Request ( void )
{
  TS_I2C_INFO i2c_data;

  i2c_data.id    = I2C0;
  i2c_data.mode  = I2C_INTERRUPT_MODE;
  i2c_data.speed = SPEED_100KHZ;
  i2c_Config_Request(&i2c_data);
}

//==================
// Initialize RTC
//==================
void RTC8564_Initialize_Request (uint32_t do_adj, uint8_t year, uint8_t month,  uint8_t day,
                                 uint8_t  week,   uint8_t hour, uint8_t minute, uint8_t second)
{
  // Initialize RTC
  if (do_adj) {
    	  //
    RTC8564_Write_Reg(RTC_CONTROL1, 0x20); // STOP
    RTC8564_Write_Reg(RTC_CONTROL2, 0x00);
        //
    RTC8564_Write_Reg(RTC_HOURS,   BCD_INT(hour));
    RTC8564_Write_Reg(RTC_MINUTES, BCD_INT(minute));
    RTC8564_Write_Reg(RTC_SECONDS, BCD_INT(second));
        //
    RTC8564_Write_Reg(RTC_YEARS,    BCD_INT(year));
    RTC8564_Write_Reg(RTC_C_MONTHS, BCD_INT(month));
    RTC8564_Write_Reg(RTC_DAYS,     BCD_INT(day));
    RTC8564_Write_Reg(RTC_WEEKDAYS, BCD_INT(week));
        //
    RTC8564_Write_Reg(RTC_MINUTE_ALARM,  0x00);
    RTC8564_Write_Reg(RTC_HOUR_ALARM,    0x00);
    RTC8564_Write_Reg(RTC_DAY_ALARM,     0x00);
    RTC8564_Write_Reg(RTC_WEEKDAY_ALARM, 0x00);
        //
    RTC8564_Write_Reg(RTC_CLKOUT_FREQ,  0x00);
    RTC8564_Write_Reg(RTC_TIMER_CONTROL,0x00);
    RTC8564_Write_Reg(RTC_TIMER,        0x00);
        //
    RTC8564_Write_Reg(RTC_CONTROL1, 0x00); // START
  }
}


//====================
// Get RTC Data
//====================
void RTC8564_Get_Current_Time_Request(TS_RTC_INFO *pTS_RTC_INFO)
{
  pTS_RTC_INFO->bRTC_year = INT_BCD(RTC8564_Read_Reg(RTC_YEARS));
  pTS_RTC_INFO->bRTC_mon  = INT_BCD(RTC8564_Read_Reg(RTC_C_MONTHS) & 0x1f);
  pTS_RTC_INFO->bRTC_day  = INT_BCD(RTC8564_Read_Reg(RTC_DAYS) & 0x3f);
  pTS_RTC_INFO->bRTC_week = RTC8564_Read_Reg(RTC_WEEKDAYS) & 0x07;
  pTS_RTC_INFO->bRTC_hour = INT_BCD(RTC8564_Read_Reg(RTC_HOURS) & 0x3f);
  pTS_RTC_INFO->bRTC_min  = INT_BCD(RTC8564_Read_Reg(RTC_MINUTES) & 0x7f);
  pTS_RTC_INFO->bRTC_sec  = INT_BCD(RTC8564_Read_Reg(RTC_SECONDS) & 0x7f);
}
