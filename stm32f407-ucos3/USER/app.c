#include "includes.h"
#include "usart.h"

#define APP_TASK_EQ_0_ITERATION_NBR 16u

static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static OS_TCB AppTaskEq0FpTCB;
static CPU_STK AppTaskEq0FpStk[APP_CFG_TASK_EQ_STK_SIZE];

static void AppTaskStart(void *arg);
static void AppTaskEq0Fp(void *arg);

int main(void)
{
    OS_ERR err;
    
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(9600);
    printf("STM32F407 uCOSIII Test\r\n");
    OSInit(&err);
    HANDLE_ERROR(err);
    printf("Creating Start Task\r\n");
    OSTaskCreate((OS_TCB      *) &AppTaskStartTCB,
                 (CPU_CHAR    *) "App Task Start",
                 (OS_TASK_PTR  ) AppTaskStart,
                 (void        *) 0u,
                 (OS_PRIO      ) APP_CFG_TASK_START_PRIO,
                 (CPU_STK     *) &AppTaskStartStk[0],
                 (CPU_STK_SIZE ) AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) APP_CFG_TASK_START_STK_SIZE,
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

static void AppTaskStart(void *arg)
{
    OS_ERR err;
    
    BSP_TRACE_DBG(("Initializing Board Support Package\r\n"));
    BSP_Init();
    APP_TRACE_DBG(("Initializing System Services\r\n"));
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
    BSP_LED_Off(0);

    APP_TRACE_DBG(("Creating Application Kernel Objects\r\n"));

    APP_TRACE_DBG(("Creating Application Tasks\r\n"));
    OSTaskCreate((OS_TCB      *) &AppTaskEq0FpTCB,
                 (CPU_CHAR    *) "FP Equation 1",
                 (OS_TASK_PTR  ) AppTaskEq0Fp, 
                 (void        *) 0,
                 (OS_PRIO      ) APP_CFG_TASK_EQ_PRIO,
                 (CPU_STK     *) &AppTaskEq0FpStk[0],
                 (CPU_STK_SIZE ) AppTaskEq0FpStk[APP_CFG_TASK_EQ_STK_SIZE / 10u],
                 (CPU_STK_SIZE ) APP_CFG_TASK_EQ_STK_SIZE,
                 (OS_MSG_QTY   ) 0u,
                 (OS_TICK      ) 0u,
                 (void        *) 0,
                 (OS_OPT       ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR      *) &err);
    HANDLE_ERROR(err);

    while (DEF_TRUE) {
        BSP_LED_Toggle(1u);
        OSTimeDlyHMSM(0u, 0u, 0u, 500u, OS_OPT_TIME_HMSM_STRICT, &err);
        HANDLE_ERROR(err);
        BSP_LED_Toggle(2u);
        OSTimeDlyHMSM(0u, 0u, 0u, 500u, OS_OPT_TIME_HMSM_STRICT, &err);
        HANDLE_ERROR(err);
    }
}

/*
*                         App_TaskEq0Fp()
* Description : This task finds the root of the following equation.
*               f(x) =  e^-x(3.2 sin(x) - 0.5 cos(x)) using the bisection mehtod
* Argument(s) : p_arg   is the argument passed to 'App_TaskEq0Fp' by 'OSTaskCreate()'.
* Return(s)   : none.
* Note(s)     : none.
*/
void AppTaskEq0Fp(void *arg)
{
    CPU_FP32   a;
    CPU_FP32   b;
    CPU_FP32   c;
    CPU_FP32   eps;
    CPU_FP32   f_a;
    CPU_FP32   f_c;
    CPU_FP32   delta;
    CPU_INT08U iteration;
    RAND_NBR   wait_cycles;
    
    while (DEF_TRUE) {
        eps       = 0.00001;
        a         = 3.0; 
        b         = 4.0;
        delta     = a - b;
        iteration = 0u;
        if (delta < 0) {
            delta = delta * -1.0;
        }
        
        while (((2.00 * eps) < delta) || (iteration > 20u)) {
            c   = (a + b) / 2.00;
            f_a = (exp((-1.0) * a) * (3.2 * sin(a) - 0.5 * cos(a)));
            f_c = (exp((-1.0) * c) * (3.2 * sin(c) - 0.5 * cos(c)));
            
            if (((f_a > 0.0) && (f_c < 0.0)) || 
                ((f_a < 0.0) && (f_c > 0.0))) {
                b = c;
            } else if (((f_a > 0.0) && (f_c > 0.0)) || 
                       ((f_a < 0.0) && (f_c < 0.0))) {
                a = c;           
            } else {
                break;
            }
                
            delta = a - b;
            if (delta < 0) {
               delta = delta * -1.0;
            }
            iteration++;

            wait_cycles = Math_Rand();
            wait_cycles = wait_cycles % 1000;

            while (wait_cycles > 0u) {
                wait_cycles--;
            }

            if (iteration > APP_TASK_EQ_0_ITERATION_NBR) {
                APP_TRACE_INFO(("App_TaskEq0Fp() max # iteration reached\r\n"));
                break;
            }            
        }

        APP_TRACE_INFO(("Eq0 Task Running ....\r\n"));
        
        if (iteration == APP_TASK_EQ_0_ITERATION_NBR) {
            APP_TRACE_INFO(("Root = %f; f(c) = %f; #iterations : %d\r\n", c, f_c, iteration));
        }
    }
}
