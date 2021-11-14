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

static OS_STK TaskStartStk[TASK_START_STK_SIZE];
static OS_STK TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE];
static OS_STK TaskLED0Stk[TASK_LED0_STK_SIZE];
static OS_STK TaskLED1Stk[TASK_LED1_STK_SIZE];

static void TaskStart(void *arg);
static void TaskFloatCalc(void *arg);
static void TaskLED0(void *arg);
static void TaskLED1(void *arg);

void printTaskStat(OS_TCB *tcb)
{
    OS_ERR err;
    OS_STK_DATA stk_data;
    err = OSTaskStkChk(tcb->OSTCBPrio, &stk_data);
    HANDLE_ERROR(err);
    printf("%2d\t%3d\t%3d\t%02d%%\t%s\r\n", tcb->OSTCBPrio, stk_data.OSUsed, stk_data.OSFree,
        (stk_data.OSUsed * 100) / (stk_data.OSUsed + stk_data.OSFree), tcb->OSTCBTaskName);
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
    OSStart();
    return 0;
}

static void TaskStart(void *arg)
{
    OS_ERR err;
    OS_CPU_SR cpu_sr = 0u;
    
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
                         (void*  ) 0,
                         (OS_STK*) &TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE - 1],
                         (INT8U  ) TASK_FLOAT_CALC_PRIO,
                         (INT16U ) TASK_FLOAT_CALC_PRIO,
                         (OS_STK*) &TaskFloatCalcStk[0],
                         (INT32U ) TASK_FLOAT_CALC_STK_SIZE,
                         (void*  ) 0,
                         (INT16U ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    // err = OSTaskCreate(TaskLED0, (void*) 0, (OS_STK*) &TaskLED0Stk[TASK_LED0_STK_SIZE - 1], TASK_LED0_PRIO);
    err = OSTaskCreateExt(TaskLED0,
                         (void*  ) 0,
                         (OS_STK*) &TaskLED0Stk[TASK_LED0_STK_SIZE - 1],
                         (INT8U  ) TASK_LED0_PRIO,
                         (INT16U ) TASK_LED0_PRIO,
                         (OS_STK*) &TaskLED0Stk[0],
                         (INT32U ) TASK_LED0_STK_SIZE,
                         (void*  ) 0,
                         (INT16U ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);
    // err = OSTaskCreate(TaskLED1, (void*) 0, (OS_STK*) &TaskLED1Stk[TASK_LED1_STK_SIZE - 1], TASK_LED1_PRIO);
    err = OSTaskCreateExt(TaskLED1,
                         (void*  ) 0,
                         (OS_STK*) &TaskLED1Stk[TASK_LED1_STK_SIZE - 1],
                         (INT8U  ) TASK_LED1_PRIO,
                         (INT16U ) TASK_LED1_PRIO,
                         (OS_STK*) &TaskLED1Stk[0],
                         (INT32U ) TASK_LED1_STK_SIZE,
                         (void*  ) 0,
                         (INT16U ) (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    HANDLE_ERROR(err);

    while (DEF_TRUE)
    {
        // OSTaskSuspend(TASK_START_PRIO);
        OSTimeDly(3000u);
        OS_ENTER_CRITICAL();
        printf("CPU Usage: %d%%\r\n", OSCPUUsage);
        printf("Prio\tUsed\tFree\tPer\tTaskName\r\n");
        OS_TCB *tcb = OSTCBList;
        while (tcb != NULL)
        {
            printTaskStat(tcb);
            tcb = tcb->OSTCBNext;
        }
        OS_EXIT_CRITICAL();
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
