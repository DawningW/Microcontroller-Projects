#include "includes.h"
#include  "usart.h"

#define TASK_START_PRIO 2u
#define TASK_START_STK_SIZE 128u
#define TASK_FLOAT_CALC_PRIO 4u
#define TASK_FLOAT_CALC_STK_SIZE 512u
#define TASK_LED0_PRIO 5u
#define TASK_LED0_STK_SIZE 128u
#define TASK_LED1_PRIO 6u
#define TASK_LED1_STK_SIZE 128u
#define TASK_KEY_PRIO 10u
#define TASK_KEY_STK_SIZE 256u

static OS_STK TaskStartStk[TASK_START_STK_SIZE];
static OS_STK TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE];
static OS_STK TaskLED0Stk[TASK_LED0_STK_SIZE];
static OS_STK TaskLED1Stk[TASK_LED1_STK_SIZE];
static OS_STK TaskKeyStk[TASK_KEY_STK_SIZE];

static void TaskStart(void *arg);
static void TaskFloatCalc(void *arg);
static void TaskLED0(void *arg);
static void TaskLED1(void *arg);
static void TaskKey(void *arg);

void printTaskStats()
{
    OS_ERR err;
    OS_CPU_SR cpu_sr = 0u;
    OS_ENTER_CRITICAL();
    OS_TCB *tcb = OSTCBList;
    printf("=============================================\r\n");
    printf("CPU Usage: %d%%\r\n", OSCPUUsage);
    printf("Prio\tUsed\tFree\tPer\tTaskName\r\n");
    while (tcb != NULL)
    {
        OS_STK_DATA stk_data;
        err = OSTaskStkChk(tcb->OSTCBPrio, &stk_data);
        HANDLE_ERROR(err);
        printf("%2d\t%3d\t%3d\t%02d%%\t%s\r\n", tcb->OSTCBPrio, stk_data.OSUsed, stk_data.OSFree,
            stk_data.OSUsed * 100 / tcb->OSTCBStkSize, tcb->OSTCBTaskName);
        tcb = tcb->OSTCBNext;
    }
    printf("=============================================\r\n");
    OS_EXIT_CRITICAL();
}

int main(void)
{
    OS_ERR err;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(9600);
    printf("STM32F407 uCOSII Test\r\n");
    OSInit();
    printf("Creating Start Task\r\n");
    // err = OSTaskCreate(TaskStart, (void*) 0, (OS_STK*) &TaskStartStk[TASK_START_STK_SIZE - 1], TASK_START_PRIO);
    err = OSTaskCreateExt(TaskStart,
                         (void*  ) 0,
                         (OS_STK*) &TaskStartStk[TASK_START_STK_SIZE - 1],
                         (INT8U  ) TASK_START_PRIO,
                         (INT16U ) TASK_START_PRIO,
                         (OS_STK*) &TaskStartStk[0],
                         (INT32U ) TASK_START_STK_SIZE,
                         (void*  ) 0,
                         (INT16U ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    OSTaskNameSet(TASK_START_PRIO, (INT8U*) "Task Start", &err);
    HANDLE_ERROR(err);
    OSStart();
    return 0;
}

static void TaskStart(void *arg)
{
    OS_ERR err;
    
    printf("Initializing Board Support Package\r\n");
    BSP_Init();
    BSP_LED_Off(0);
    printf("Initializing System Services\r\n");
    CPU_Init();
    Mem_Init();
    Math_Init();
    OS_CPU_SysTickInitFreq(SystemCoreClock);
    OSStatInit();

    printf("Creating Application Tasks\r\n");
    // err = OSTaskCreate(TaskFloatCalc, (void*) 0, (OS_STK*) &TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE - 1], TASK_FLOAT_CALC_PRIO);
    err = OSTaskCreateExt(TaskFloatCalc,
                         (void   *) 0,
                         (OS_STK *) &TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE - 1],
                         (INT8U   ) TASK_FLOAT_CALC_PRIO,
                         (INT16U  ) TASK_FLOAT_CALC_PRIO,
                         (OS_STK *) &TaskFloatCalcStk[0],
                         (INT32U  ) TASK_FLOAT_CALC_STK_SIZE,
                         (void   *) 0,
                         (INT16U  ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    OSTaskNameSet(TASK_FLOAT_CALC_PRIO, (INT8U*) "Task Float Calc", &err);
    HANDLE_ERROR(err);
    // err = OSTaskCreate(TaskLED0, (void*) 0, (OS_STK*) &TaskLED0Stk[TASK_LED0_STK_SIZE - 1], TASK_LED0_PRIO);
    err = OSTaskCreateExt(TaskLED0,
                         (void   *) 0,
                         (OS_STK *) &TaskLED0Stk[TASK_LED0_STK_SIZE - 1],
                         (INT8U   ) TASK_LED0_PRIO,
                         (INT16U  ) TASK_LED0_PRIO,
                         (OS_STK *) &TaskLED0Stk[0],
                         (INT32U  ) TASK_LED0_STK_SIZE,
                         (void   *) 0,
                         (INT16U  ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    OSTaskNameSet(TASK_LED0_PRIO, (INT8U*) "Task LED0", &err);
    HANDLE_ERROR(err);
    // err = OSTaskCreate(TaskLED1, (void*) 0, (OS_STK*) &TaskLED1Stk[TASK_LED1_STK_SIZE - 1], TASK_LED1_PRIO);
    err = OSTaskCreateExt(TaskLED1,
                         (void   *) 0,
                         (OS_STK *) &TaskLED1Stk[TASK_LED1_STK_SIZE - 1],
                         (INT8U   ) TASK_LED1_PRIO,
                         (INT16U  ) TASK_LED1_PRIO,
                         (OS_STK *) &TaskLED1Stk[0],
                         (INT32U  ) TASK_LED1_STK_SIZE,
                         (void   *) 0,
                         (INT16U  ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    OSTaskNameSet(TASK_LED1_PRIO, (INT8U*) "Task LED1", &err);
    HANDLE_ERROR(err);
    // err = OSTaskCreate(TaskKey, (void*) 0, (OS_STK*) &TaskKeyStk[TASK_KEY_STK_SIZE - 1], TASK_KEY_PRIO);
    err = OSTaskCreateExt(TaskKey,
                         (void   *) 0,
                         (OS_STK *) &TaskKeyStk[TASK_KEY_STK_SIZE - 1],
                         (INT8U   ) TASK_KEY_PRIO,
                         (INT16U  ) TASK_KEY_PRIO,
                         (OS_STK *) &TaskKeyStk[0],
                         (INT32U  ) TASK_KEY_STK_SIZE,
                         (void   *) 0,
                         (INT16U  ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    OSTaskNameSet(TASK_KEY_PRIO, (INT8U*) "Task Key", &err);
    HANDLE_ERROR(err);
    
    while (DEF_TRUE)
    {
        OSTaskSuspend(TASK_START_PRIO);
    }
}

static void TaskFloatCalc(void *arg)
{
    OS_CPU_SR cpu_sr = 0u;
    float num = 0.0f;
    
    while (DEF_TRUE)
    {
        num += 0.01f;
        OS_ENTER_CRITICAL();
        printf("Number: %f\r\n", num);
        OS_EXIT_CRITICAL();
        OSTimeDly(200u);
    }
}

static void TaskLED0(void *arg)
{
    while (DEF_TRUE)
    {
        BSP_LED_Toggle(1u);
        OSTimeDly(500u);
    }
}

static void TaskLED1(void *arg)
{
    while (DEF_TRUE)
    {
        BSP_LED_Toggle(2u);
        OSTimeDly(1000u);
    }
}

static void TaskKey(void *arg)
{
    uint8_t lastkey = 0;
    
    while (DEF_TRUE)
    {
        uint8_t key = 0;
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            OSTimeDly(20u);
            key = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
        }
        if (lastkey != key)
        {
            lastkey = key;
            switch (key)
            {
                default:
                case 0: break;
                case 1: printTaskStats(); break;
            }
        }
        OSTimeDly(10u);
    }
}
