#include "am2302.h"
#include "delay.h"
#include "gpio.h"
#include <stdio.h>

#define DHT_MODE_IN()  {DHT_GPIO_Port->MODER &= ~(3 << 3 * 2); DHT_GPIO_Port->MODER |= 0 << 3 * 2;}
#define DHT_MODE_OUT() {DHT_GPIO_Port->MODER &= ~(3 << 3 * 2); DHT_GPIO_Port->MODER |= 1 << 3 * 2;}
#define DHT_WRITE_HIGH HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET)
#define DHT_WRITE_LOW HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET)
#define DHT_READ (HAL_GPIO_ReadPin(DHT_GPIO_Port, DHT_Pin) == GPIO_PIN_SET)

bool am2302_init(void)
{
    // AM2302无需初始化. 只要第一次读取数据在上电两秒后即可
    return true;
}

// 向传感器发送起始信号
void am2302_reset(void)
{
    DHT_MODE_OUT(); // 设置输出
    DHT_WRITE_LOW; // 主机拉低
    delay_ms(1); // DHT11需要拉低18~30ms, DHT22需要1~20ms
    DHT_WRITE_HIGH; // 主机拉高, 释放总线
    delay_us(30); // DHT11需要拉高10~20us, DHT22需要10~200us
}

// 检查传感器响应信号
bool am2302_check(void)
{
    DHT_MODE_IN(); // 设置输入
    uint8_t timeout = 100;
    while (DHT_READ && --timeout) // 等待传感器拉低
    {
        delay_us(1);
    }
    if (timeout == 0) return false;
    timeout = 100;
    while (!DHT_READ && --timeout) // 等待传感器拉高
    {
        delay_us(1);
    }
    if (timeout == 0) return false;
    return true;
}

uint8_t am2302_read_bit(void)
{
    uint8_t timeout = 100;
    while (DHT_READ && --timeout) // 等待低电平
    {
        delay_us(1);
    }
    timeout = 100;
    while (!DHT_READ && --timeout) // 等待高电平
    {
        delay_us(1);
    }
    delay_us(40); // 等待40us
    return DHT_READ;
}

uint8_t am2302_read_byte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= am2302_read_bit();
    }
    return byte;
}

bool am2302_get_data(float *temperature, float *humidity)
{
    uint8_t buffer[5];
    am2302_reset();
    if (am2302_check())
    {
        for (uint8_t i = 0; i < 5; i++) // 读取40位数据
        {
            buffer[i] = am2302_read_byte();
        }
        if ((uint8_t) (buffer[0] + buffer[1] + buffer[2] + buffer[3]) == buffer[4]) // 校验, 和有可能超过uint8_t
        {
            // DHT11
            // *humidity = buffer[0];
			// *temperature = buffer[2];
            // DHT22(AM2302)
            *humidity = (buffer[0] << 8 | buffer[1]) * 0.1f;
            int sign = 1;
            if (buffer[2] & 0x80)
            {
                buffer[2] &= ~0x80;
                sign = -1;
            }
            *temperature = (buffer[2] << 8 | buffer[3]) * 0.1f * sign;
            return true;
        }
    }
    return false;
}
