/* Arduino电子钟 */
#include <stdint.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

enum key_type
{
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_SELECT,
  KEY_COUNT
};
const int KEY_ADC_VALUES[KEY_COUNT] = {30, 150, 360, 535, 760};

const int KEY = 0;
const int RS = 8, RW = 11, EN = 9 , D4 = 4, D5 = 5, D6 = 6, D7 = 7;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

int8_t key = -1;
int8_t lastKey = -1;

uint16_t year = 2021;
int8_t month = 1;
int8_t day = 1;
int8_t hour = 0;
int8_t minute = 0;
int8_t second = 0;
int8_t select = 0; // 当前正在设置哪一项
bool blank = false; // 设置项是显示还是隐藏
long blink = 0; // 设置项上次闪烁的时间

const int8_t DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
bool isLeapYear()
{
  if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
    return true;
  return false;
}
int8_t getDaysInMonth()
{
    int8_t dayCount = DAYS_IN_MONTH[month - 1];
    if (month == 2) dayCount += isLeapYear() ? 1 : 0;
    return dayCount;
}

int8_t getKey()
{
  uint32_t input = analogRead(KEY);
  for (int8_t k = 0; k < KEY_COUNT; k++)
  {
    if (input < KEY_ADC_VALUES[k])
    {
      return k;
    }
  }
  return -1;
}

void displayNumber(int num, int length, bool isHide)
{
  char str[length + 1];
  if (!isHide)
  {
    if (length == 2)
      sprintf(str, "%02d", num);
    else
      sprintf(str, "%d", num);
  }
  else
  {
    for (int i = 0; i < length; i++)
      str[i] = ' ';
  }
  str[length] = '\0';
  lcd.print(str);
}

void drawClock(bool isBlank = false)
{
  int x1 = 3, x2 = 4;
  lcd.setCursor(x1, 0);
  displayNumber(year, 4, select == 1 && isBlank);
  lcd.print("/");
  displayNumber(month, 2, select == 2 && isBlank);
  lcd.print("/");
  displayNumber(day, 2, select == 3 && isBlank);
  lcd.setCursor(x2, 1);
  displayNumber(hour, 2, select == 4 && isBlank);
  lcd.print(":");
  displayNumber(minute, 2, select == 5 && isBlank);
  lcd.print(":");
  displayNumber(second, 2, select == 6 && isBlank);
}

void fixDay()
{
  uint8_t daysInMonth = getDaysInMonth();
  if (day > daysInMonth) day = daysInMonth;
}

void onKeyPressed(key_type key)
{
  if (key == KEY_SELECT)
  {
    select = !select;
    if (select)
    {
      blank = false;
      blink = millis();
    }
    drawClock();
  }
  else if (select)
  {
    if (key == KEY_RIGHT)
    {
      select++;
      if (select > 6) select -= 6;
    }
    else if (key == KEY_LEFT)
    {
      select--;
      if (select <= 0) select += 6;
    }
    else if (key == KEY_DOWN)
    {
      switch (select)
      {
        case 1:
        {
          year++;
          fixDay();
          break;
        }
        case 2:
        {
          month++;
          if (month > 12) month = 1;
          fixDay();
          break;
        }
        case 3:
        {
          day++;
          if (day > getDaysInMonth()) day = 1;
          break;
        }
        case 4:
        {
          hour++;
          if (hour >= 24) hour = 0;
          break;
        }
        case 5:
        {
          minute++;
          if (minute >= 60) minute = 0;
          break;
        }
        case 6:
        {
          second++;
          if (second >= 60) second = 0;
          break;
        }
        default: break;
      }
    }
    else if (key == KEY_UP)
    {
      switch (select)
      {
        case 1:
        {
          year--;
          if (year < 1970) year = 1970;
          fixDay();
          break;
        }
        case 2:
        {
          month--;
          if (month < 1) month = 12;
          fixDay();
          break;
        }
        case 3:
        {
          day--;
          if (day < 1) day = getDaysInMonth();
          break;
        }
        case 4:
        {
          hour--;
          if (hour < 0) hour = 23;
          break;
        }
        case 5:
        {
          minute--;
          if (minute < 0) minute = 59;
          break;
        }
        case 6:
        {
          second--;
          if (second < 0) second = 59;
          break;
        }
        default: break;
      }
    }
    blank = false;
    blink = millis();
    drawClock();
  }
}

void onTimer1Interrupted()
{
  if (!select)
  {
    second++;
    if (second >= 60)
    {
      second = 0;
      minute++;
      if (minute >= 60)
      {
        minute = 0;
        hour++;
        if (hour >= 24)
        {
          hour = 0;
          day++;
          if (day > getDaysInMonth())
          {
            day = 1;
            month++;
            if (month > 12)
            {
              month = 1;
              year++;
            }
          }
        }
      }
    }
    drawClock();
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Timer1.initialize(1000000); // 设置定时器间隔1s
  Timer1.attachInterrupt(onTimer1Interrupted); // 关联中断函数
  lcd.begin(16, 2);
  lcd.clear();
  interrupts(); // 开启中断
  drawClock();
}

void loop()
{
  key = getKey();
  if(key != lastKey)
  {
    delay(50); // 消抖
    key = getKey();
    if(key != lastKey)
    {
      lastKey = key;
      if (key >= 0)
      {
        onKeyPressed((key_type) key);
      }
    }
  }
  if (select && millis() - blink > 500)
  {
    blank = !blank;
    drawClock(blank);
    blink = millis();
  }
}
