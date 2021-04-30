开发板中景园
单片机STM32F407ZGT6
晶振8.000MHZ

SPL库默认HSI_VALUE为26MHz, 需修改为8MHz
不要忘记修改PLL_M为8

BOOT0接地时执行用户程序, 接3.3V时执行Bootloader
上传程序时的波特率建议设置为76800
FlyMcu的串口控制选项需选择"DTR的低电平复位,RTS高电平进BootLoader"

使用串口时需要将开发板上面的PA10与TX用跳线帽短接, PA9与RX短接

backup.bin为出厂自带测试程序
