#ifndef __RTC_H
#define __RTC_H
#include "system.h"

uint8_t rtc_init(void);
ErrorStatus rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm);
ErrorStatus rtc_set_date(uint8_t year, uint8_t month, uint8_t day, uint8_t dayofweek);
void rtc_set_alarm_a(uint8_t dayofweek, uint8_t hour, uint8_t min, uint8_t sec);
void rtc_set_wakeup(uint32_t wksel, uint16_t cnt);

#endif
