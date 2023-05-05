#ifndef __RTC8564_H__
#define __RTC8564_H__

#include "my_lpc1114fn28.h"

#define I2C2_DEV_S_OWN_ADDR  0x51

typedef struct
{
  uint8_t  bRTC_year; // RTC year
  uint8_t  bRTC_mon;  // RTC month
  uint8_t  bRTC_day;  // RTC day
  uint8_t  bRTC_week; // RTC week
  uint8_t  bRTC_hour; // RTC hour
  uint8_t  bRTC_min;  // RTC minute
  uint8_t  bRTC_sec;  // RTC second
} TS_RTC_INFO;

enum WEEK {
	SUN = 0,
	MON,
	TUE,
	WED,
	THU,
	FRI,
	SAT
};

extern uint8_t BCD_INT(uint8_t num);
extern uint8_t INT_BCD(uint8_t bcd);
extern uint8_t *Get_Week_String(uint32_t week);
extern void RTC8564_Write_Reg(uint32_t addr, uint32_t data);
extern uint32_t RTC8564_Read_Reg(uint32_t addr);
extern void RTC8564_Config_Request ( void );
extern void RTC8564_Initialize_Request(uint32_t do_adj, uint8_t year, uint8_t month,  uint8_t day,
              uint8_t week,    uint8_t hour, uint8_t minute, uint8_t second);
extern void RTC8564_Get_Current_Time_Request(TS_RTC_INFO *pTS_RTC_INFO);

#endif // __RTC8564_H__
