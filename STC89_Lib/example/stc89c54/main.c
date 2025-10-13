#include <stdio.h>
#include "system.h"
#include "gpio.h"
#include "exti.h"
#include "uart.h"
#include "isp.h"
#include "soft_pwm.h"

void uart_received(char *dat, size_t length)
{
    UNUSED(length);
    puts(dat);
}

void init()
{
    UART_CONFIG uart = {0};
    
    pwm_init();
    pwm_set_duty(25);
    
    uart.baudrate = 9600;
    uart.baud_generator = TIM_1;
    uart.parity = UART_Parity_None;
    uart.priority = NVIC_Priority_1;
    uart.callback = uart_received;
    uart_init(&uart);
    
    nvic_enable();
}

void main()
{
    init();
    pwm_cmd(true);
    delay(1000);
    printf("Hello from STC89C54 by WC\n");
 
    while (true)
    {
        // gpio_toggle_bits(GPIO_1, GPIO_PIN_0);
        delay(1000);
    }
}
