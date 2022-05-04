#include "config.h"
#include <intrins.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h"
#include "LED.h"
#include "MKEY.h"
#include "LCDTFT.h"
#include "STEPMOTO.h"

#define BACKGROUND_COLOR 0x0000
#define MAX_LENGTH 6

enum {
    SET,
    INPUT,
    UNLOCK
} status = SET;

char password[MAX_LENGTH + 1] = {0};
char input[MAX_LENGTH + 1] = {0};
char *pInput = input;
char lastkey = -1;
u16 lcd_x = 0;

int key_scan() {
    /*
    char key, key2;
    if ((key = Mkey_Read()) >= 0) {
        delay_ms(10);
        if ((key2 = Mkey_Read()) == key) {
            if (key != lastkey) {
                lastkey = key;
                return key;
            }
        }
    }
    lastkey = -1;
    return -1;
    */
    // ���治��Ҫ����
    return Mkey_Read();
}

void lcd_disp(const char *str) {
    TFT_ShowStr(lcd_x, 0, 0xFFFF, BACKGROUND_COLOR, str);
    lcd_x += strlen(str) * 8;
}

void lcd_update(int id) {
    lcd_x = 0;
    TFT_FillRect(0, 0, 319, 15, BACKGROUND_COLOR);
    switch (id) {
        default:
        case 0: lcd_disp("Password entry: "); lcd_disp(input); break;
        case 1: lcd_disp("success!"); break;
        case 2: lcd_disp("Password input: "); lcd_disp(input); break;
        case 3: lcd_disp("Input password please!"); break;
        case 4: lcd_disp("Password ERR!"); break;
        case 5: lcd_disp("Password OK!"); break;
    }
}

void main() {
    u16 i;
    // ��ʼ��
    LED_GPIO_config();
    LED_Switch(RED);
    STEP_MOTO_GPIO_config();
    MKEY_GPIO_config();
    TFT_GPIO_config();
    TFT320_Clear(BACKGROUND_COLOR);
    lcd_update(0);
    // ҵ���߼�
    for (;;) {
        int key = key_scan();
        delay_ms(50);
        if (status != UNLOCK) {
            // �����������������������
            if (key >= 0 && key <= 9) {
                // �������ּ�
                if (pInput - input < MAX_LENGTH) {
                    *pInput++ = '0' + key;
                    lcd_update(status == SET ? 0 : 2);
                }
            } else if (key == 10) {
                // ����ȷ�ϼ�
                if (pInput > input) {
                    // �Ѿ�����������
                    if (status == SET) {
                        // ȷ����������
                        status = INPUT;
                        strcpy(password, input);
                        lcd_update(1);
                        delay_ms(2000);
                    } else if (status == INPUT) {
                        // ȷ����������
                        if (strcmp(input, password) == 0) {
                            // ������ȷ
                            status = UNLOCK;
                            LED_Switch(GREEN);
                            lcd_update(5);
                            for (i = 0; i < 100; i++)
                                STEP_MOTO_Run(200);
                            delay_ms(1000);
                        } else {
                            // �������
                            LED_Switch(YELLOW);
                            lcd_update(4);
                            delay_ms(2000);
                        }
                        // �ָ�����������״̬
                        status = INPUT;
                        LED_Switch(RED);
                    }
                    // ��������
                    memset(input, 0, MAX_LENGTH);
                    pInput = input;
                    lcd_update(2);
                } else {
                    // δ��������
                    lcd_update(3);
                    delay_ms(2000);
                    lcd_update(2);
                }
            } else if (key == 11) {
                // ����ȡ����
                if (pInput > input) {
                    *--pInput = '\0';
                    lcd_update(status == SET ? 0 : 2);
                }
            }
        } else {
            // ���ڿ���
        }
    }
}
