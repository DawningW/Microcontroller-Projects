/* Arduino工具人测试例程 */
#include <stdint.h>
#include <LiquidCrystal.h>
 
const char msgs[5][15]={
  "Right Key OK ",
  "Up Key OK    ",
  "Down Key OK  ",
  "Left Key OK  ",
  "Select Key OK"
};

const int rs = 8, rw = 11, en = 9 , d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int adc_key_val[5]={30,150,360,535,760};
int NUM_KEYS=5;
int adc_key_in;
int key=-1;
int old_key=-1;

int get_key(uint32_t input)
{
  int k;
  for(k=0;k<NUM_KEYS;k++)
  {
    if(input<adc_key_val[k])
    {
      return k;  
    }
  }
  if(k>=NUM_KEYS)
  {
    k=-1;
  }
  return k;
}

void setup()
{
  pinMode(13, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("LCD testing");
  lcd.setCursor(0, 1);
  lcd.print("Press any key");
}

void loop()
{
  adc_key_in=analogRead(0);
  digitalWrite(13, HIGH);
  key=get_key(adc_key_in);

  if(key!=old_key)
  {
    delay(50); // 消抖
    adc_key_in=analogRead(0);
    key=get_key(adc_key_in);
    if(key!=old_key)
    {
      old_key=key;
      if(key>=0)
      {
        lcd.setCursor(0, 1);
        lcd.print(msgs[key]);
      }
    }
  }
  digitalWrite(13, LOW);
}
