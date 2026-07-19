#ifndef RTC_H
#define RTC_H

#include "stm32f407.h"

typedef struct
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} RTC_TimeTypeDef;

typedef struct
{
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t weekday;
} RTC_DateTypeDef;

void RTC_Init(void);

void RTC_SetTime(RTC_TimeTypeDef *time);
void RTC_GetTime(RTC_TimeTypeDef *time);

void RTC_SetDate(RTC_DateTypeDef *date);
void RTC_GetDate(RTC_DateTypeDef *date);

#endif
