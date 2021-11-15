#include "includes.h"
#include "usart.h"

#define HANDLE_ERROR(x) ((x != OS_ERR_NONE) ? (void) printf("Error: %d\r\n", (int) x) : (void) 0)

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

static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASK_START_STK_SIZE];
static OS_TCB TaskFloatCalcTCB;
static CPU_STK TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE];
static OS_TCB TaskLED0TCB;
static CPU_STK TaskLED0Stk[TASK_LED0_STK_SIZE];
static OS_TCB TaskLED1TCB;
static CPU_STK TaskLED1Stk[TASK_LED1_STK_SIZE];
static OS_TCB TaskKeyTCB;
static CPU_STK TaskKeyStk[TASK_KEY_STK_SIZE];

static void TaskStart(void *arg);
static void TaskFloatCalc(void *arg);
static void TaskLED0(void *arg);
static void TaskLED1(void *arg);
static void TaskKey(void *arg);

void printTaskStats()
{
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OS_TCB *tcb = OSTaskDbgListPtr;
    printf("==================================================\r\n");
    printf("CPU Usage: %.2f%%\r\n", (float) OSStatTaskCPUUsage / 100);
    printf("Prio\tCPU\tUsed\tFree\tPer\tTaskName\r\n");
    while (tcb != NULL)
    {
        printf("%2d\t%.2f%%\t%3d\t%3d\t%02d%%\t%s\r\n", tcb->Prio, (float) tcb->CPUUsageMax / 100,
            tcb->StkUsed, tcb->StkFree, tcb->StkUsed * 100 / tcb->StkSize, tcb->NamePtr);
        tcb = tcb->DbgNextPtr;
    }
    printf("==================================================\r\n");
    CPU_CRITICAL_EXIT();
}

int main(void)
{
    OS_ERR err;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(9600);
    printf("STM32F407 uCOSIII Test\r\n");
    OSInit(&err);
    HANDLE_ERROR(err);
    printf("Creating Start Task\r\n");
    OSTaskCreate((OS_TCB      *) &TaskStartTCB,
                 (CPU_CHAR    *) "Task Start",
                 (OS_TASK_PTR  ) TaskStart,
                 (void        *) 0u,
                 (OS_PRIO      ) TASK_START_PRIO,
                 (CPU_STK     *) TaskStartStk,
                 (CPU_STK_SIZE ) TaskStartStk[TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) TASK_START_STK_SIZE,
                 (OS_MSG_QTY   ) 0u,
                 (OS_TICK      ) 0u,
                 (void        *) 0u,
                 (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *) &err);
    HANDLE_ERROR(err);
    OSStart(&err);
    HANDLE_ERROR(err);
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
    BSP_OS_TickInit();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);
    HANDLE_ERROR(err);
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    
    printf("Creating Application Kernel Objects\r\n");

    printf("Creating Application Tasks\r\n");
    OSTaskCreate((OS_TCB      *) &TaskFloatCalcTCB,
                 (CPU_CHAR    *) "Task Float Calc",
                 (OS_TASK_PTR  ) TaskFloatCalc, 
                 (void        *) 0,
                 (OS_PRIO      ) TASK_FLOAT_CALC_PRIO,
                 (CPU_STK     *) &TaskFloatCalcStk[0],
                 (CPU_STK_SIZE ) TaskFloatCalcStk[TASK_FLOAT_CALC_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) TASK_FLOAT_CALC_STK_SIZE,
                 (OS_MSG_QTY   ) 0u,
                 (OS_TICK      ) 0u,
                 (void        *) 0,
                 (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR      *) &err);
    HANDLE_ERROR(err);
    OSTaskCreate((OS_TCB      *) &TaskLED0TCB,
                 (CPU_CHAR    *) "Task LED0",
                 (OS_TASK_PTR  ) TaskLED0,
                 (void        *) 0u,
                 (OS_PRIO      ) TASK_LED0_PRIO,
                 (CPU_STK     *) TaskLED0Stk,
                 (CPU_STK_SIZE ) TaskLED0Stk[TASK_LED0_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) TASK_LED0_STK_SIZE,
                 (OS_MSG_QTY   ) 0u,
                 (OS_TICK      ) 0u,
                 (void        *) 0u,
                 (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *) &err);
    HANDLE_ERROR(err);
    OSTaskCreate((OS_TCB      *) &TaskLED1TCB,
                 (CPU_CHAR    *) "Task LED1",
                 (OS_TASK_PTR  ) TaskLED1,
                 (void        *) 0u,
                 (OS_PRIO      ) TASK_LED1_PRIO,
                 (CPU_STK     *) TaskLED1Stk,
                 (CPU_STK_SIZE ) TaskLED1Stk[TASK_LED1_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) TASK_LED1_STK_SIZE,
                 (OS_MSG_QTY   ) 0u,
                 (OS_TICK      ) 0u,
                 (void        *) 0u,
                 (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *) &err);
    HANDLE_ERROR(err);
    OSTaskCreate((OS_TCB      *) &TaskKeyTCB,
                 (CPU_CHAR    *) "Task Key",
                 (OS_TASK_PTR  ) TaskKey,
                 (void        *) 0u,
                 (OS_PRIO      ) TASK_KEY_PRIO,
                 (CPU_STK     *) TaskKeyStk,
                 (CPU_STK_SIZE ) TaskKeyStk[TASK_KEY_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) TASK_KEY_STK_SIZE,
                 (OS_MSG_QTY   ) 0u,
                 (OS_TICK      ) 0u,
                 (void        *) 0u,
                 (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *) &err);
    HANDLE_ERROR(err);
    
    while (DEF_TRUE)
    {
        OSTaskSuspend(&TaskStartTCB, &err);
    }
}

static void TaskFloatCalc(void *arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    float num = 0.0f;
    
    while (DEF_TRUE)
    {
        num += 0.01f;
        CPU_CRITICAL_ENTER();
        printf("Number: %f\r\n", num);
        CPU_CRITICAL_EXIT();
        OSTimeDly(200u, OS_OPT_TIME_DLY, &err);
        HANDLE_ERROR(err);
    }
}

static void TaskLED0(void *arg)
{
    OS_ERR err;
    
    while (DEF_TRUE)
    {
        BSP_LED_Toggle(1u);
        OSTimeDly(500u, OS_OPT_TIME_DLY, &err);
        HANDLE_ERROR(err);
    }
}

static void TaskLED1(void *arg)
{
    OS_ERR err;
    
    while (DEF_TRUE)
    {
        BSP_LED_Toggle(2u);
        OSTimeDly(1000u, OS_OPT_TIME_DLY, &err);
        HANDLE_ERROR(err);
    }
}

static void TaskKey(void *arg)
{
    OS_ERR err;
    uint8_t lastkey = 0;
    
    while (DEF_TRUE)
    {
        uint8_t key = 0;
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            OSTimeDly(20u, OS_OPT_TIME_DLY, &err);
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
        OSTimeDly(10u, OS_OPT_TIME_DLY, &err);
        HANDLE_ERROR(err);
    }
}
