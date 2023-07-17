# Stellar-XL 电子墨水屏驱动

## 项目简介

## 硬件配置

MCU: MSP430G2553 28pin tssop 封装

EPD: HINK E042A03-A1, 4.2 寸黑白, 分辨率 400*300, fpc 24pin, 主控 SSD1608

|EPD|fpc|MCU|备注|
|---|---|---|----|
|CS1|12|P3.4|左半屏|
|CS2|01|P3.7|右半屏, 在其他价签中是空脚|
|SDA|14|P2.4||
|SCL|13|P2.3||
|BUSY|09|P2.5||
|RST|10|P3.6||
|D/C|11|P3.5||
|BS|08|P3.1||
|power|15/16|P2.6|H=off; L=on|

RF: A7106 20pin qfn 封装

|RF|MCU|备注|
|--|---|----|
|CS|P1.3||
|DAT|P1.2||
|CLK|P1.4||
|RFIO01|P1.1||
|RFIO02|P1.0||
|power|P2.7|H=off; L=on|

SPI Flash: 25VQ21B 256Kb 8pin sop 封装

|Flash|MCU|备注|
|-----|---|----|
|CS|P3.0||
|SO|P1.6||
|SI|P1.7||
|CLK|P1.5||

## 参考资料

- https://blog.csdn.net/gonepoo/article/details/107993507
- https://www.mydigit.cn/thread-163976-1-1.html
- https://www.mydigit.cn/thread-243908-1-1.html
- https://www.mydigit.cn/thread-274369-1-1.html
- https://www.mydigit.cn/thread-275704-1-1.html
- https://github.com/muyuchl/epaper_price_tag_mod
- https://github.com/reiyawea/HanshowClock
