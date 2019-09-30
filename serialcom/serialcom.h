#ifndef __SERIALCOM_H__
#define __SERIALCOM_H__

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L      //System frequency
#define BAUD 9600           //UART baudrate

/*Define UART parity mode*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT EVEN_PARITY   //Testing even parity

void InitCom();
void SendData(BYTE dat);
void SendString(char *str);

#endif
