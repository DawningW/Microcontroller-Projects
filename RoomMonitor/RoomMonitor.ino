#include <TZ.h>
#include <time.h>
#include <user_interface.h>
#include <coredecls.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

#define KEY_PIN 0 // D3
#define IR_PIN 14 // D5
#define SDA_PIN 4 // D2
#define SCL_PIN 5 // D1

uint16_t code_open[] = {
  4400, 4400,
  550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 550,
  550, 550, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 1660,
  550, 550, 550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 1660, 550, 550, 550, 1660, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 1660, 550, 1660, 550, 550, 550, 1660, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 550, 550, 550, 550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 5220,
  4400, 4400,
  550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 550,
  550, 550, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 1660,
  550, 550, 550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 1660, 550, 550, 550, 1660, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 1660, 550, 1660, 550, 550, 550, 1660, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 550, 550, 550, 550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 8000
};
const uint16_t code_close[] = {
  4400, 4400,
  550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 550,
  550, 550, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 1660,
  550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 550, 550, 1660, 550, 1660,
  550, 1660, 550, 550, 550, 550, 550, 550, 550, 550, 550, 1660, 550, 550, 550, 550,
  550, 1660, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 550, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 5220,
  4400, 4400,
  550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 550,
  550, 550, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 1660,
  550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 550, 550, 1660, 550, 1660,
  550, 1660, 550, 550, 550, 550, 550, 550, 550, 550, 550, 1660, 550, 550, 550, 550,
  550, 1660, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 550, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 5220,
  4400, 4400,
  550, 1660, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 550,
  550, 550, 550, 1660, 550, 550, 550, 550, 550, 1660, 550, 1660, 550, 550, 550, 1660,
  550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550,
  550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660, 550, 1660,
  550, 8000
};

Adafruit_AHTX0 aht;
sensors_event_t humi, temp;
IRsend irsend(IR_PIN);
// IRMideaAC ac(IR_PIN);
WiFiClient client;
HTTPClient http;
uint8_t count = 0;
uint8_t status = 0;
time_t lastReportTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Start to monitor room");

  pinMode(KEY_PIN, INPUT_PULLUP);
  Wire.begin(SDA_PIN, SCL_PIN);
  aht.begin();
  irsend.begin();
  // ac.begin();

  configTime(TZ_Asia_Shanghai, "cn.ntp.org.cn", "time.pool.aliyun.com", "pool.ntp.org");
  Serial.print(F("WiFi connecting"));
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint8_t retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (++retry > 20) {
      Serial.println();
      Serial.print("Can't connect to wifi.");
      break;
    }
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    WiFi.setHostname(HOSTNAME);
  }
}

void loop() {
  if (Serial.available() > 0) {
    char c = char(Serial.read());
    if (c == 'o') {
      Serial.println("Open Midea AC");
      openAC();
    } else if (c == 'c') {
      Serial.println("Close Midea AC");
      closeAC();
    } else if (c == 'r') {
      Serial.println("Get temperature");
      aht.getEvent(&humi, &temp);
      Serial.printf("Temperature: %f°C\n", temp.temperature);
      Serial.printf("Humidity: %f%%\n", humi.relative_humidity);
    } else {
      Serial.printf("Unknown command %c\n", c);
    }
  }
  if (digitalRead(KEY_PIN) == LOW) {
    delay(10);
    if (digitalRead(KEY_PIN) == LOW) {
      Serial.println("Key pressed");
      if (status == 0) {
        openAC();
        status = 1;
      } else {
        closeAC();
        status = 0;
      }
    }
  }
  if (++count % 10 == 0) {
    count = 0;
    time_t now = time(nullptr);
    struct tm *tm = localtime(&now);
    // 上报温度
    if (now - lastReportTime >= 300) {
      lastReportTime = now;
      aht.getEvent(&humi, &temp);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Report sensor data to server: ");

        DynamicJsonDocument doc(512);
        doc["time"] = now;
        doc["temperature"] = temp.temperature;
        doc["humidity"] = humi.relative_humidity;
        String body;
        serializeJson(doc, body);
        Serial.println(body);

        http.begin(client, API);
        http.addHeader("Content-Type", "application/json");
        int httpCode = http.POST(body);
        if (httpCode > 0) {
          String payload = http.getString();
          Serial.println(payload);
        } else {
          Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        Serial.println("WiFi disconnected, cannot report");
      }
    }
    // 开关空调
    if (status == 0) {
      if (tm->tm_hour == 5 && tm->tm_min == 0) {
        openAC();
        status = 1;
      }
    } else if (status == 1) {
      if (tm->tm_hour == 7 && tm->tm_min == 0) {
        closeAC();
        status = 0;
      }
    }
  }
  delay(100);
}

// https://blog.csdn.net/jiaoqiao123/article/details/99334766
void getcode(uint16_t *buf, unsigned char A, unsigned char B, unsigned char C) {
  unsigned char mask = 0x80;
  unsigned int cnt = 2;

  buf[0] = 4400;
  buf[1] = 4400;

  //发A
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & A)
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & (~A))
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  //发B
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & B)
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & (~B))
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  //发C
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & C)
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & (~C))
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }

  buf[cnt++] = 550;
  buf[cnt++] = 5220;
  buf[cnt++] = 4400;
  buf[cnt++] = 4400;

  //发A
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & A)
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & (~A))
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  //发B
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & B)
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & (~B))
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  //发C
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & C)
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }
  for (mask = 0x80; mask != 0; mask >>= 1) {
    buf[cnt++] = 550;
    if (mask & (~C))
      buf[cnt++] = 1660;
    else
      buf[cnt++] = 550;
  }

  buf[cnt++] = 550;
  buf[cnt++] = 8800;
}

void openAC() {
  getcode(code_open, 0xB2, 0xBF, 0x2C);
  irsend.sendRaw(code_open, 200, 38);
  // ac.on();
  // ac.setMode(kMideaACHeat);
  // ac.setFan(kMideaACFanAuto);
  // ac.setTemp(20);
  // ac.send();
}

void closeAC() {
  irsend.sendRaw(code_close, 300, 38);
  // ac.off();
  // ac.send();
}
