/*
 以下为ESP8266接线, 其他单片机请到MFRC522库主页查看
 GPIO12(D6) MISO
 GPIO13(D7) MOSI
 GPIO14(D5) SCK
 GPIO15(D8) SDA/SS
 GPIO2(D4) RST
 串口波特率 115200
 接收到的卡数据会在串口输出
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 2
#define SDA_PIN 15

MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return;
  if (!mfrc522.PICC_ReadCardSerial())
    return;
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
