/*
 GPIO4(D2) 红外发射管正极
 GPIO14(D5) 红外接收头数据脚
 串口波特率 115200
 接收到的红外数据会在串口输出
 若为NEC协议, 则向串口发送's'会发送上一次收到的按键
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

IRsend irsend(4);
IRrecv irrecv(14);
decode_results results;
uint32_t last_value = 0x00F700FFUL;

void setup()
{
  irsend.begin();
  Serial.begin(115200);
  irrecv.enableIRIn();
  Serial.println();
  Serial.println("Waiting for IR message on Pin 14, and sending on Pin 4");
}

void loop()
{
  if (Serial.available() > 0)
  {
    char c = char(Serial.read());
    if (c == 's') {
      Serial.printf("Send NEC %#x", last_value);
      Serial.println();
      irsend.sendNEC(last_value);
      delay(100);
    }
  }
  if (irrecv.decode(&results)) {
    Serial.print(resultToHumanReadableBasic(&results));
    yield();
    if (results.decode_type > -1) {
      Serial.print(resultToSourceCode(&results));
      yield();
    }
    irrecv.resume();
  }
  delay(100);
}
