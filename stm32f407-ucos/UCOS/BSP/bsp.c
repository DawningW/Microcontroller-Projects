/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       IAR Development Kits
*                                              on the
*
*                                       STM32F407ZGT6 中景园
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*                 YS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include  <bsp.h>
#include  <os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_GPIOF_LED1                        DEF_BIT_09
#define  BSP_GPIOF_LED2                        DEF_BIT_10


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DBGMCU_CR                        (*(CPU_REG32 *)0xE0042004)


/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_LED_Init  (void);
static  void  BSP_Key_Init  (void);


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Asynchronous      :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    BSP_LED_Init();                                             /* Initialize user LEDs                                 */
    BSP_Key_Init();

#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_IOEN_MASK;    /* Enable tracing (see Note #2).                        */
    BSP_CPU_REG_DBGMCU_CR &= ~BSP_DBGMCU_CR_TRACE_MODE_MASK;    /* Clr trace mode sel bits.                             */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04; /* Cfg trace mode to synch 4-bit.                       */
#endif
}


/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initialize any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    initialize ALL  LEDs
*                       1    initialize user LED1
*                       2    initialize user LED2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : The LED pins are active-low on the STM32F429II-SK board
*********************************************************************************************************
*/

static void  BSP_LED_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
                                                                /* Configure GPIOE for LED1 and LED2                    */
    GPIO_InitStructure.GPIO_Pin = (BSP_GPIOF_LED1 | BSP_GPIOF_LED2);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOF, &GPIO_InitStructure);
}


/*
*********************************************************************************************************
*                                              BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns OFF ALL the LEDs
*                       1    turns OFF user LED1
*                       2    turns OFF user LED2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U  led)
{
    switch (led) {
        case 0u:
             GPIO_SetBits(GPIOA, BSP_GPIOF_LED1);
             GPIO_SetBits(GPIOG, BSP_GPIOF_LED2);
             break;


        case 1u:
             GPIO_SetBits(GPIOA, BSP_GPIOF_LED1);
             break;


        case 2u:
             GPIO_SetBits(GPIOG, BSP_GPIOF_LED2);
             break;


        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns ON ALL  LEDs
*                       1    turns ON user LED1
*                       2    turns ON user LED2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U led)
{
    switch (led) {
        case 0u:
             GPIO_ResetBits(GPIOA, BSP_GPIOF_LED1);
             GPIO_ResetBits(GPIOG, BSP_GPIOF_LED2);
             break;


        case 1u:
             GPIO_ResetBits(GPIOA, BSP_GPIOF_LED1);
             break;


        case 2u:
             GPIO_ResetBits(GPIOG, BSP_GPIOF_LED2);
             break;


        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                            BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE ALL the LEDs
*                       1    TOGGLE user LED1
*                       2    TOGGLE user LED2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U  led)
{
    CPU_INT32U  pins;


    switch (led) {
        case 0u:
             pins  =  GPIO_ReadOutputData(GPIOF);               /* Read data (current state of output pins) in GPIOA    */
             pins ^=  BSP_GPIOF_LED1;                           /* Tells which pins (other than pin for LED1) are ON    */
             GPIO_SetBits(  GPIOF,   pins  &  BSP_GPIOF_LED1);  /* If there are any,turn them OFF and leave LED1 ON     */
             GPIO_ResetBits(GPIOF, (~pins) &  BSP_GPIOF_LED1);

             pins  =  GPIO_ReadOutputData(GPIOF);
             pins ^=  BSP_GPIOF_LED2;
             GPIO_SetBits(  GPIOF,   pins  &  BSP_GPIOF_LED2);
             GPIO_ResetBits(GPIOF, (~pins) &  BSP_GPIOF_LED2);
             break;


        case 1u:
             pins  = GPIO_ReadOutputData(GPIOF);
             pins ^= BSP_GPIOF_LED1;
             GPIO_SetBits(  GPIOF,   pins   &  BSP_GPIOF_LED1);
             GPIO_ResetBits(GPIOF, (~pins)  &  BSP_GPIOF_LED1);
             break;


        case 2u:
             pins  = GPIO_ReadOutputData(GPIOF);
             pins ^= BSP_GPIOF_LED2;
             GPIO_SetBits(  GPIOF,   pins   &  BSP_GPIOF_LED2);
             GPIO_ResetBits(GPIOF, (~pins)  &  BSP_GPIOF_LED2);
             break;


        default:
             break;
    }
}

static void  BSP_Key_Init (void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /*
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
    
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    */
}

