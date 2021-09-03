#include "rtc.h"
#include "delay.h"
#include "usart.h"

/**
 * 初始化内部实时时钟
 * @return 0 初始化成功
 *         1 低速外部晶振(LSE)开启失败
 *         2 进入初始化模式失败
 */
uint8_t rtc_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5050)
    {
        // 启用低速外部晶振(LSE)
        RCC_LSEConfig(RCC_LSE_ON);
        uint16_t retry = 0x1FFF;
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            retry++;
            delay_ms(10);
        }
        if (retry == 0) return 1;
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        
        RTC_InitTypeDef RTC_InitStructure;
        RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
        RTC_InitStructure.RTC_SynchPrediv = 0xFF;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
        RTC_Init(&RTC_InitStructure);
        
        RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5050);
    }
    
    rtc_set_time(0, 0, 0, RTC_H12_AM);
    rtc_set_date(21, 9, 1, 1);
    return 0;
}

/**
 * 设置时间
 * ampm: 是上午还是下午(12小时制)
 */
ErrorStatus rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm)
{
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;
    RTC_TimeTypeInitStructure.RTC_Hours = hour;
    RTC_TimeTypeInitStructure.RTC_Minutes = min;
    RTC_TimeTypeInitStructure.RTC_Seconds = sec;
    RTC_TimeTypeInitStructure.RTC_H12 = ampm;
    return RTC_SetTime(RTC_Format_BIN, &RTC_TimeTypeInitStructure);
}

/**
 * 设置日期
 * year, month, day: 年(0~99), 月(1~12), 日(0~31)
 * dayofweek: 星期几(1~7)
 */
ErrorStatus rtc_set_date(uint8_t year, uint8_t month, uint8_t day, uint8_t dayofweek)
{
    RTC_DateTypeDef RTC_DateTypeInitStructure;
    RTC_DateTypeInitStructure.RTC_Year = year;
    RTC_DateTypeInitStructure.RTC_Month = month;
    RTC_DateTypeInitStructure.RTC_Date = day;
    RTC_DateTypeInitStructure.RTC_WeekDay = dayofweek;
    return RTC_SetDate(RTC_Format_BIN, &RTC_DateTypeInitStructure);
}

/**
 * 设置闹钟(按星期, 24小时制)
 * week: 星期几(1~7) @ref RTC_Alarm_Definitions
 * hour, min, sec: 小时, 分钟, 秒钟
 */
void rtc_set_alarm_a(uint8_t dayofweek, uint8_t hour, uint8_t min, uint8_t sec)
{
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

    RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
    RTC_TimeTypeDef *RTC_TimeTypeInitStructure = &RTC_AlarmTypeInitStructure.RTC_AlarmTime;
    RTC_TimeTypeInitStructure->RTC_Hours = hour;
    RTC_TimeTypeInitStructure->RTC_Minutes = min;
    RTC_TimeTypeInitStructure->RTC_Seconds = sec;
    RTC_TimeTypeInitStructure->RTC_H12 = RTC_H12_AM;
    RTC_AlarmTypeInitStructure.RTC_AlarmMask = RTC_AlarmMask_None;
    RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
    RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay = dayofweek;
    RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A, &RTC_AlarmTypeInitStructure);

    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
}

/**
 * 设置周期性唤醒定时器
 * wksel: @ref RTC_Wakeup_Timer_Definitions
 * cnt: 自动重装载值, 减到0时产生中断
 */
void rtc_set_wakeup(uint32_t wksel, uint16_t cnt)
{
    RTC_WakeUpCmd(DISABLE);
    RTC_WakeUpClockConfig(wksel);
    RTC_SetWakeUpCounter(cnt);
    
    RTC_ITConfig(RTC_IT_WUT, ENABLE);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line22);
    RTC_WakeUpCmd(ENABLE);
}

void RTC_Alarm_IRQHandler(void)
{
    if (RTC_GetFlagStatus(RTC_FLAG_ALRAF) == SET)
    {
        RTC_ClearFlag(RTC_FLAG_ALRAF);
        // 处理闹钟
    }
    EXTI_ClearITPendingBit(EXTI_Line17);
}

void RTC_WKUP_IRQHandler(void)
{
    if (RTC_GetFlagStatus(RTC_FLAG_WUTF) == SET)
    {
        RTC_ClearFlag(RTC_FLAG_WUTF);
        // 处理周期唤醒
    }
    EXTI_ClearITPendingBit(EXTI_Line22);
}
